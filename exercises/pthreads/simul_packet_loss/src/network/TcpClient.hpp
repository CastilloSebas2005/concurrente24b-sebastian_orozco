// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <netdb.h>

#include "common.hpp"
#include "Socket.hpp"

class NetworkAddress;

/**
 * @brief Implements a TCP client
 * A client is a process that connects with a server process that is listening
 * at some known address and port. After establishing a TCP connection, both
 * can share information using a socket. Example of a minimal web client:
 * 
 * @code
    #include <iostream>
    #include <stdexcept>
    #include <string>

    #include "TcpClient.hpp"

    int main(int argc, char* argv[]) {
      try {
        if (argc == 4) {
          TcpClient client;
          Socket& socket = client.connect(argv[1], argv[2]);
          socket << "GET " << argv[3] << " HTTP/1.0\r\n";
          socket << "Host: " << argv[1] << "\r\n\r\n";
          socket.send();

          std::string line;
          while (socket.readLine(line)) {
            std::cout << line << std::endl;
          }
        } else {
          std::cout << "usage: client SERVER PORT URI" << std::endl;
        }
      } catch (std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
      }
    }
 * @endcode
 */
class TcpClient {
  DISABLE_COPY(TcpClient);

 protected:
  /// Lookup criteria for searching network information about the server
  struct addrinfo hints;
  /// Available network addresses to connect to the server
  struct addrinfo* availableAddresses = nullptr;
  /// Selected network address to connect to the server
  const struct addrinfo* selectedAddress = nullptr;
  /// This object is used to send and receive information with the peer after
  /// the connection has been established with the server. It stores the file
  /// descriptor that identifies the socket in the operating system.
  Socket socket;

 public:
  /// Constructor
  TcpClient();
  /// Destructor
  ~TcpClient();
  /// Tries to establish a TCP connection with a server
  /// @param server The server's network address, such as a IP (eg: 1.2.3.4) or
  /// a domain name (eg: www.example.net)
  /// @param port The port number where the server is listening for connections
  /// (eg: "8080") or its protocol name (eg: "http")
  /// @return A reference to the Socket object that can be used to send and
  /// receive information with the server
  /// @throw std::runtime_error if could not connect to the server
  Socket& connect(const char* server, const char* port);
  /// Close the connection with the server
  void close();
  /// Get the network address (IP and port) of the server
  NetworkAddress getServerAddress() const;

 protected:
  /// Fetch all available network addresses where we can listen with this port
  void fetchAvailableAddresses(const char* server, const char* port);
  /// Get a socket for the first available address that works
  /// @throw std::runtime_error if no network address works
  void openSocketWithServer(const char* server, const char* port);
};

#endif  // TCPCLIENT_H
