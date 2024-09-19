// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <map>
#include <string>
#include <sstream>

#include "HttpMessage.hpp"

/**
 * @brief Manages response messages from server to clients.
 * @details See https://jeisson.ecci.ucr.ac.cr/appweb/material/#http_messages
 */
class HttpResponse : public HttpMessage {
 public:
  /// Objects of this class can be copied, but avoid unnecessary copies
  DECLARE_RULE4(HttpResponse, default);

 public:
  /// @brief Associate status codes with descriptive texts (reason phrases)
  typedef std::map<int, const char*> ReasonPhrases;
  /// Standard status codes and their reason phrases according to RFC 7231
  /// https://tools.ietf.org/html/rfc7231#section-6.1
  static const ReasonPhrases reasonPhrases;

 protected:
  /// Status code, e.g: 200 OK, 500 Internal server error, ...
  int statusCode = 200;
  /// Status text, e.g: "OK" for 200, and "Internal server error" for 500
  std::string reasonPhrase;

 public:
  /// Constructor
  explicit HttpResponse(const Socket& socket);
  /// Destructor
  ~HttpResponse();
  /// Set the status code, and optionally the status text (reason phrase)
  /// If no status text is provided, a default one will be used for the code.
  /// @param statusCode Standard three-digit integer that indicates if the
  /// response is information (1XX), success (2XX), redirection (3XX), user
  /// error (4XX), or server error (5XX). For example, 404 is used to report
  /// that user requested for an non-existing resource.
  /// @param reasonPhrase A human-readable text explaining the status code.
  /// This argument can be omitted if the code is standard (listed in RFC 7231).
  /// In that case, the reason phrase suggested in RFC 7231 will be used. If
  /// a non standard code is used and reasonPhrase is empty, the method will
  /// reject the status code and return false;
  /// @return true if the statusCode is accepted, false if it is rejected
  bool setStatusCode(int statusCode, const std::string& reasonPhrase = "");
  /// Build the status line, e.g: "HTTP/1.1 200 OK" or "HTTP/1.0 404 Not found"
  /// Text is built from values of the member attributes of this object
  std::string buildStatusLine() const;
  /// Send this response to the peer through the Socket
  /// @return true on success, false on error or connection closed by peer
  bool send();

 protected:
  /// Build the "Content-Type" and "Content-Length" metadata to the peer if
  /// there are already in the headers associative array. It tries to get these
  /// values by looking at the body object
  bool sendBodyMetadata();
};

#endif  // HTTPRESPONSE_H
