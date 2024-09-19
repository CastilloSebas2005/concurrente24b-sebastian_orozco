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

#include "TcpServer.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"

TcpServer::TcpServer() {
  // Set initial values for searching for a suitable network address
  ::memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_PASSIVE;  // Fill with my local IP
  hints.ai_family = AF_UNSPEC;  // Use IPv4 or IPv6, whichever
  hints.ai_socktype = SOCK_STREAM;  // Use TCP
}

TcpServer::~TcpServer() {
  this->stopListening();
  ::freeaddrinfo(this->availableAddresses);
}

void TcpServer::listenForever(const char* port) {
  this->listenForConnections(port);
  this->acceptAllConnections();
}

void TcpServer::stopListening() {
  // An ugly way to stop incoming connections is closing the socket
  if (this->connectionRequestSocket >= 0) {
    ::close(this->connectionRequestSocket);
  }
}

void TcpServer::listenForConnections(const char* port) {
  // Fetch all available network addresses where we can listen with this port
  this->fetchAvailableAddresses(port);
  // Get a socket for the first available address that works for the given port
  this->openConnectionRequestSocket(port);
  // Ask the operating system to enqueue connection requests on this socket
  int error = ::listen(this->connectionRequestSocket
    , this->connectionQueueCapacity);
  // If OS was unable to start listening for connections, raise an exception
  if (error) {
    throw std::runtime_error(std::string("could not listen port ") + port);
  }
}

void TcpServer::fetchAvailableAddresses(const char* port) {
  // This function is designed to be called once
  assert(this->availableAddresses == nullptr);
  // Fetch all available addresses and store results in object attributes
  int error = ::getaddrinfo(nullptr, port, &this->hints
    , &this->availableAddresses);
  // If getaddrinfo failed, raise an exception
  if (error) {
    throw std::runtime_error(std::string("getaddrinfo: ")
       + ::gai_strerror(error));
  }
}

void TcpServer::openConnectionRequestSocket(const char* port) {
  assert(this->connectionRequestSocket == -1);
  // Traverse the available addresses and select the first one that works
  for (const struct addrinfo* address = this->availableAddresses; address;
      address = address->ai_next) {
    // Try to open a socket using this address result
    this->connectionRequestSocket = ::socket(address->ai_family
      , address->ai_socktype, address->ai_protocol);

    // If we could open the socket
    if (this->connectionRequestSocket >= 0) {
      // Allow the socket to reuse the local IP for other connections
      int yes = 1;
      int error = ::setsockopt(this->connectionRequestSocket, SOL_SOCKET
        , SO_REUSEADDR, &yes, sizeof yes);
      if (error == 0) {
        // Bind the socket to the port we passed in to getaddrinfo()
        error = ::bind(this->connectionRequestSocket, address->ai_addr
          , address->ai_addrlen);
        if (error == 0) {
          // This address is OK, stop searching and use its socket
          this->selectedAddress = address;
          return;
        }
      } else {
        throw std::runtime_error("could not set reusing socket");
      }

      // We could not bind a port for this address, continue searching
      ::close(this->connectionRequestSocket);
    } else {
      // We could not open a socket for this address, continue searching
    }
  }

  throw std::runtime_error(std::string("no available addresses for port ")
      + port);
}

void TcpServer::acceptAllConnections() {
  while (true) {
    this->acceptConnectionRequest();
  }
}

void TcpServer::acceptConnectionRequest() {
  assert(this->connectionRequestSocket >= 0);
  // This object represents the communication with the peer
  Socket client;

  // Wait for and accept only one connection request
  // TODO(jhc): stop accept() and clean exit when signals (e.g. Ctrl+C) are sent
  // e.g: https://stackoverflow.com/a/35755340
  socklen_t clientAddressSize = sizeof(struct sockaddr_storage);
  int file = ::accept(this->connectionRequestSocket
    , client.getSockAddr(), &clientAddressSize);

  // If connection failed, raise an exception
  if (file == -1) {
    throw std::runtime_error("could not accept client connection");
  }

  client.setSocketFileDescriptor(file);
  this->handleClientConnection(client);
}

NetworkAddress TcpServer::getNetworkAddress() const {
  assert(this->selectedAddress);
  return NetworkAddress(this->selectedAddress->ai_addr);
}
