// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <stdexcept>
#include <vector>

#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"

// Shared Socket ---------------------------------------------------------------

/// Copies of a Socket object share the same connection (socket)
struct SharedSocket {
  /// SharedSockets cannot be copied
  DISABLE_COPY(SharedSocket);

 public:
  /// IPv4 or IPv6 address of the peer on the other side of the connection
  struct sockaddr_storage peerAddress;
  /// Socket file descriptor given by OS to communicate with the peer
  int socketFileDescriptor = -1;
  /// Buffer to extract data before a read of received data
  std::ostringstream output;
  /// Buffer to store data in memory before sending to the peer
  /// TODO(any): istringstream is not suitable to manage large/binary data
  std::istringstream input;

 public:
  /// Constructor
  SharedSocket() {
    ::memset(&this->peerAddress, 0, sizeof(this->peerAddress));
  }

  /// Destructor. Closes the socket file descriptor
  ~SharedSocket() {
    this->close();
  }

  /// Closes the socket file descriptor
  void close() {
    if (this->socketFileDescriptor >= 0) {
      // Close connection with client
      ::close(this->socketFileDescriptor);

      const NetworkAddress& address = this->getNetworkAddress();
      Log::append(Log::INFO, "socket", std::string("connection -----closed")
        + address.getIP() + " port " + std::to_string(address.getPort()));

      ::memset(&this->peerAddress, 0, sizeof(this->peerAddress));
      this->socketFileDescriptor = -1;
    }
  }

  /// Get read-and-write access to the storage address record
  struct sockaddr_storage& getSockAddrStorage() {
    return this->peerAddress;
  }

  /// Get read-and-write access of the storage address as a sockaddr record
  struct sockaddr* getSockAddr() {
    return reinterpret_cast<struct sockaddr*>(&this->peerAddress);
  }

  /// Get read-only access of the storage address as a sockaddr record
  const struct sockaddr* getSockAddr() const {
    return reinterpret_cast<const struct sockaddr*>(&this->peerAddress);
  }

  /// Build a human-readable network address object of the peer
  NetworkAddress getNetworkAddress() const {
    return NetworkAddress(this->getSockAddr());
  }
};

// Socket class ---------------------------------------------------------------

Socket::Socket()
  : sharedSocket{new SharedSocket()} {
}

void Socket::close() {
  this->sharedSocket->close();
}

NetworkAddress Socket::getNetworkAddress() const {
  return this->sharedSocket->getNetworkAddress();
}

int Socket::getSocketFileDescriptor() const {
  return this->sharedSocket->socketFileDescriptor;
}

void Socket::setSocketFileDescriptor(int socketFileDescriptor) {
  this->sharedSocket->socketFileDescriptor = socketFileDescriptor;
}

struct sockaddr* Socket::getSockAddr() {
  return this->sharedSocket->getSockAddr();
}

void Socket::setNetworkAddress(const struct addrinfo* address) {
  std::memcpy(&this->sharedSocket->peerAddress, address->ai_addr
    , address->ai_addrlen);
}

bool Socket::operator==(const Socket& other) const {
  return this->sharedSocket->socketFileDescriptor
    == other.sharedSocket->socketFileDescriptor;
}

bool Socket::operator<(const Socket& other) const {
  return this->sharedSocket->socketFileDescriptor
    < other.sharedSocket->socketFileDescriptor;
}

bool Socket::readLine(std::string& line, char separator) {
  while (!std::getline(this->sharedSocket->input, line, separator)) {
    if (!this->receive()) {
      return false;  // error
    }
    // receive() clears stream error flags, and retry
  }
  return true;  // success read
}

bool Socket::receive() {
  const size_t capacity = 65535;
  std::vector<char> buffer(capacity);
  ssize_t res = this->readAvailable(buffer.data(), buffer.size());
  if (res > 0) {
    this->sharedSocket->input.str(buffer.data());
    this->sharedSocket->input.clear();
    return true;
  }
  return false;
}

ssize_t Socket::readAvailable(char* buffer, size_t capacity) {
  return ::recv(this->sharedSocket->socketFileDescriptor, buffer, capacity, 0);
}

ssize_t Socket::read(char* buffer, size_t size) {
  ssize_t received = 0;
  while (size_t(received) < size) {
    const ssize_t result =
      this->readAvailable(buffer + received, size - received);

    // If error happened, stop
    if (result <= 0) {
      return result;
    }

    // Success
    received += result;
    assert(size_t(received) <= size);
  }

  return received;
}

bool Socket::send() {
  const std::string& text = this->sharedSocket->output.str();
  const char* buffer = text.c_str();
  const size_t length = text.length();
  ssize_t sent = 0;

  // Try and retry to send the data until all data is sent to peer
  while (true) {
    const ssize_t result =
      ::send(this->sharedSocket->socketFileDescriptor, buffer + sent
        , length - sent, 0);

    // If an error happened
    if (result < 0) {
      return false;
    }

    sent += result;
    if (size_t(sent) >= length) {
      this->sharedSocket->output.str("");
      return true;  // success: all data was sent
    }
  }
}

Socket::operator bool() const {
  return this->sharedSocket->input.good() && this->sharedSocket->output.good();
}

/// Expands the extraction operator>> with the given data type
#define IMPL_EXTRACT_OP(type) \
  Socket& Socket::operator>>(type value) { \
    while ( !(this->sharedSocket->input >> value) ) { \
      if ( !this->receive() ) { return *this; } \
    } \
    return *this; \
  }

IMPL_EXTRACT_OP(bool&)
IMPL_EXTRACT_OP(short&)  // NOLINT(runtime/int)
IMPL_EXTRACT_OP(unsigned short&)  // NOLINT(runtime/int)
IMPL_EXTRACT_OP(int&)  // NOLINT(runtime/int)
IMPL_EXTRACT_OP(unsigned int&)  // NOLINT(runtime/int)
IMPL_EXTRACT_OP(long&)  // NOLINT(runtime/int)
IMPL_EXTRACT_OP(unsigned long&)  // NOLINT(runtime/int)
IMPL_EXTRACT_OP(long long&)  // NOLINT(runtime/int)
IMPL_EXTRACT_OP(unsigned long long&)  // NOLINT(runtime/int)
IMPL_EXTRACT_OP(float&)
IMPL_EXTRACT_OP(double&)
IMPL_EXTRACT_OP(long double&)
IMPL_EXTRACT_OP(void*&)
IMPL_EXTRACT_OP(std::string&)

/// Expands the insertion operator<< with the given data type
#define IMPL_INSERT_OP(type) \
  Socket& Socket::operator<<(type value) { \
    this->sharedSocket->output << value; \
    return *this; \
  }

IMPL_INSERT_OP(bool)
IMPL_INSERT_OP(short)  // NOLINT(runtime/int)
IMPL_INSERT_OP(unsigned short)  // NOLINT(runtime/int)
IMPL_INSERT_OP(int)  // NOLINT(runtime/int)
IMPL_INSERT_OP(unsigned int)  // NOLINT(runtime/int)
IMPL_INSERT_OP(long)  // NOLINT(runtime/int)
IMPL_INSERT_OP(unsigned long)  // NOLINT(runtime/int)
IMPL_INSERT_OP(long long)  // NOLINT(runtime/int)
IMPL_INSERT_OP(unsigned long long)  // NOLINT(runtime/int)
IMPL_INSERT_OP(float)
IMPL_INSERT_OP(double)
IMPL_INSERT_OP(long double)
IMPL_INSERT_OP(void*)
IMPL_INSERT_OP(const char*)
IMPL_INSERT_OP(const std::string&)
