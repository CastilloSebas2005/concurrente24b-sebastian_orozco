// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef NETWORKADDRESS_H
#define NETWORKADDRESS_H

#include <sys/socket.h>

#include <cstdint>
#include <string>

#include "common.hpp"

/// Network ports are unsigned integers of 2 bytes
typedef uint16_t port_t;

class NetworkAddress {
 public:
  /// Objects of this class can be copied
  DECLARE_RULE4(NetworkAddress, default);

 protected:
  /// Pointer to IPv4 or IPv6 address record
  const struct sockaddr* address = nullptr;

 public:
  /// Constructor. A valid network address must be provided
  explicit NetworkAddress(const struct sockaddr* address);
  /// Destructor
  ~NetworkAddress() = default;
  /// Extracts the IP address in text form
  std::string getIP() const;
  /// Get the port number in host endianness
  port_t getPort() const;
};

#endif  // NETWORKADDRESS_H
