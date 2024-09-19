// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include "TcpClient.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"

TcpClient::TcpClient() {
  // Set initial values for searching for a suitable network address
  ::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;  // Use IPv4 or IPv6, whichever
  hints.ai_socktype = SOCK_STREAM;  // Use TCP
}

TcpClient::~TcpClient() {
  this->close();
}

void TcpClient::close() {
  // Close the connection with the server
  this->socket.close();
  // Release the resources required by the address
  if (this->availableAddresses) {
    ::freeaddrinfo(this->availableAddresses);
    this->availableAddresses = nullptr;
  }
}

Socket& TcpClient::connect(const char* server, const char* port) {
  // If there is an active connection, close it
  this->close();
  // Fetch all available network addresses where we can listen with this port
  this->fetchAvailableAddresses(server, port);
  // Get a socket for the first available address that works
  this->openSocketWithServer(server, port);
  // Ask the operating system to establish a connection on this socket
  int error = ::connect(this->socket.getSocketFileDescriptor()
    , this->selectedAddress->ai_addr, this->selectedAddress->ai_addrlen);
  // If no connection was established, raise an exception
  if (error) {
    this->close();
    throw std::runtime_error(std::string("could not connect to ")
      + server + ':' + port);
  }
  // This object is used to send and receive information with the peer
  return this->socket;
}

void TcpClient::fetchAvailableAddresses(const char* server, const char* port) {
  // This function is designed to be called when there is no active connection
  assert(this->availableAddresses == nullptr);
  // Fetch all available addresses and store results in object attributes
  int error = ::getaddrinfo(server, port, &this->hints
    , &this->availableAddresses);
  // If getaddrinfo failed, raise an exception
  if (error) {
    throw std::runtime_error(std::string("getaddrinfo: ")
       + ::gai_strerror(error));
  }
}

void TcpClient::openSocketWithServer(const char* server, const char* port) {
  assert(this->socket.getSocketFileDescriptor() == -1);
  // Traverse the available addresses and select the first one that works
  for (const struct addrinfo* address = this->availableAddresses; address;
      address = address->ai_next) {
    // Try to open a socket using this address result
    int socketFileDescriptor = ::socket(address->ai_family
      , address->ai_socktype, address->ai_protocol);

    // If we could open the socket
    if (socketFileDescriptor >= 0) {
      // This address is OK, stop searching and use this socket
      this->selectedAddress = address;
      this->socket.setSocketFileDescriptor(socketFileDescriptor);
      this->socket.setNetworkAddress(this->selectedAddress);
      return;
    } else {
      // We could not open a socket for this address, continue searching
    }
  }

  throw std::runtime_error(std::string("could not get address for ")
    + server + ':' + port);
}

NetworkAddress TcpClient::getServerAddress() const {
  assert(this->selectedAddress);
  return NetworkAddress(this->selectedAddress->ai_addr);
}
