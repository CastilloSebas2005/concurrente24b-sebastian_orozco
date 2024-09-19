// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>

#include <memory>
#include <string>
#include <sstream>

#include "common.hpp"

class NetworkAddress;
struct SharedSocket;

/// Expands the extraction operator>> with the given data type
#define DECL_EXTRACT_OP(type) Socket& operator>>(type value);

/// Expands the insertion operator<< with the given data type
#define DECL_INSERT_OP(type) Socket& operator<<(type value);

class Socket {
 public:
  /// Objects of this class can be copied, but avoid unnecessary copies
  DECLARE_RULE4(Socket, default);
  /// Other network classes require to access internal attributes
  friend class TcpServer;
  friend class TcpClient;

 protected:
  /// Copies of this object share the same socket file descriptor and buffers
  std::shared_ptr<SharedSocket> sharedSocket;

 public:
  /// Constructor
  Socket();
  /// Destructor. Closes the socket file descriptor if this is the last object
  /// using a shared socket
  ~Socket() = default;

 public:
  /// Build a human-readable network address object of the peer
  NetworkAddress getNetworkAddress() const;
  /// Closes the network connection with peer
  void close();
  /// Read a line of data received from peer
  /// @return true on success, false on error or connection closed by peer
  bool readLine(std::string& line, char separator = '\n');
  /// Send output buffer to the peer
  /// @return true on success, false on error or connection closed by peer
  bool send();
  /// Receive some data from peer. The read data is stored in the internal
  /// @a input buffer. This function blocks the caller
  /// @return true if a least a byte was read, false on error or connection
  /// closed by peer
  bool receive();
  /// Read the available data from peer into a buffer. The data is transferred
  /// from the socket to the buffer. No data is stored in the internal @a input
  /// buffer. This function may block/ the caller thread if socket is empty
  /// @return The amount of bytes read, 0 on connection closed by peer, -1 on
  /// error and global errno is set to the error code
  ssize_t readAvailable(char* buffer, size_t capacity);
  /// Read the requested amount of bytes from the socket to the given buffer.
  /// No data is stored in the internal @a input. This function blocks the
  /// caller thread until the total amount of bytes are read
  /// @return The amount of bytes read, 0 on connection closed by peer, -1 on
  /// error and global errno is set to the error code
  ssize_t read(char* buffer, size_t size);
  /// Evaluates this object within a boolean context as true if previous
  /// read and write operations were successful
  explicit operator bool() const;
  /// Returns true if this socket represents the same connection than the other
  bool operator==(const Socket& other) const;
  /// Returns true if this socket should be ordered before the other
  /// The sockets are compared by socket file descriptor number
  bool operator<(const Socket& other) const;

 public:
  /// Extraction operator >> used to read values from peer sent data.
  /// If you want to preload data, call @a receive() before this operator
  /// @return This socket object
  DECL_EXTRACT_OP(bool&)
  DECL_EXTRACT_OP(short&)  // NOLINT(runtime/int)
  DECL_EXTRACT_OP(unsigned short&)  // NOLINT(runtime/int)
  DECL_EXTRACT_OP(int&)  // NOLINT(runtime/int)
  DECL_EXTRACT_OP(unsigned int&)  // NOLINT(runtime/int)
  DECL_EXTRACT_OP(long&)  // NOLINT(runtime/int)
  DECL_EXTRACT_OP(unsigned long&)  // NOLINT(runtime/int)
  DECL_EXTRACT_OP(long long&)  // NOLINT(runtime/int)
  DECL_EXTRACT_OP(unsigned long long&)  // NOLINT(runtime/int)
  DECL_EXTRACT_OP(float&)
  DECL_EXTRACT_OP(double&)
  DECL_EXTRACT_OP(long double&)
  DECL_EXTRACT_OP(void*&)
  DECL_EXTRACT_OP(std::string&)
  /// Insertion operator << used to send values to the peer later.
  /// If you want to the send the data immediately to peer, call @a send()
  /// @return This socket object
  DECL_INSERT_OP(bool)
  DECL_INSERT_OP(short)  // NOLINT(runtime/int)
  DECL_INSERT_OP(unsigned short)  // NOLINT(runtime/int)
  DECL_INSERT_OP(int)  // NOLINT(runtime/int)
  DECL_INSERT_OP(unsigned int)  // NOLINT(runtime/int)
  DECL_INSERT_OP(long)  // NOLINT(runtime/int)
  DECL_INSERT_OP(unsigned long)  // NOLINT(runtime/int)
  DECL_INSERT_OP(long long)  // NOLINT(runtime/int)
  DECL_INSERT_OP(unsigned long long)  // NOLINT(runtime/int)
  DECL_INSERT_OP(float)
  DECL_INSERT_OP(double)
  DECL_INSERT_OP(long double)
  DECL_INSERT_OP(void*)
  DECL_INSERT_OP(const char*)
  DECL_INSERT_OP(const std::string&)

 protected:
  /// Get the socket file descriptor, the number that identifies the socket
  int getSocketFileDescriptor() const;
  /// Set the socket file descriptor. This operation is only made by network
  /// classes, such as TcpServer
  void setSocketFileDescriptor(int socketFileDescriptor);
  /// Copies the sockaddr record as the network address for this socket
  void setNetworkAddress(const struct addrinfo* address);
  /// Get read-only access of the storage address as a sockaddr record
  struct sockaddr* getSockAddr();
};

#endif  // SOCKET_H
