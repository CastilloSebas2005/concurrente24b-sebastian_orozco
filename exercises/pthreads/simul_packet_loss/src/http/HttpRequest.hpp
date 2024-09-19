// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>

#include "HttpMessage.hpp"

class Socket;

/**
 * @brief Manages request messages coming from clients.
 * @details See https://jeisson.ecci.ucr.ac.cr/appweb/material/#http_messages
 */
class HttpRequest : public HttpMessage {
 public:
  /// Objects of this class can be copied, but avoid innecesary copies
  DECLARE_RULE4(HttpRequest, default);

 protected:
  /// HTTP method: GET, POST, HEADERS, DELETE, UPDATE, ...
  std::string method;
  /// URI /path?query=string#fragment
  std::string uri;

 public:
  /// Constructor
  explicit HttpRequest(const Socket& socket);
  /// Destructor
  ~HttpRequest();
  /// Parses an HTTP request from the data sent from the socket
  /// @return true on success, false on error or connection closed by peer
  bool parse();
  /// Get access to the HTTP method used by client
  inline const std::string& getMethod() const { return this->method; }
  /// Get access to the resource address (URI) asked by client
  inline const std::string& getURI() const { return this->uri; }
  /// Get access to the HTTP version used by client
  inline const std::string& getHttpVersion() const { return this->httpVersion; }

 protected:
  /// Parse the request line from the socket
  /// @return true on success, false on error or connection closed by peer
  bool parseRequestLine();
  /// Parse the message header, containing pairs "key: value" of metadata
  /// @return true on success, false on error or connection closed by peer
  bool parseHeader();
  /// Parse the optional message body content, if any
  /// @return true on success, false on error or connection closed by peer
  bool parseBody();
};

#endif  // HTTPREQUEST_H
