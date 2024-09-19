// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <stdexcept>
#include <string>

#include "HttpResponse.hpp"
#include "Socket.hpp"

HttpResponse::HttpResponse(const Socket& socket)
  : HttpMessage{socket} {
}

HttpResponse::~HttpResponse() {
}

bool HttpResponse::setStatusCode(int statusCode,
  const std::string& reasonPhrase) {
  // Accept all given reason phrases
  if (reasonPhrase.length() > 0) {
    this->statusCode = statusCode;
    this->reasonPhrase = reasonPhrase;
    return true;
  }

  // No reason phrase was given, use a standard one
  const ReasonPhrases::const_iterator& itr
    = HttpResponse::reasonPhrases.find(statusCode);

  // If status code is standard
  if (itr != HttpResponse::reasonPhrases.end()) {
    // Use the standard reason phrase
    this->statusCode = statusCode;
    this->reasonPhrase = itr->second;
    return true;
  }

  // Status code is not standard and no reason phrase was provided, reject it
  return false;
}

bool HttpResponse::send() {
  const std::string& sep = HttpMessage::lineSeparator;

  // Send status line, e.g: "HTTP/1.0 200 Tuanis\r\n"
  this->socket << this->buildStatusLine() << sep;

  // Send message header
  for (HttpMessage::Headers::const_iterator itr = this->headers.begin();
    itr != this->headers.end(); ++itr) {
    // Send a metadata pair of key: value, e.g: "Server: My Web Server"
    this->socket << itr->first << ": " << itr->second << sep;
  }

  // Build and send Content-Type and Content-Length from body
  if (!this->sendBodyMetadata()) {
    return false;
  }

  // HTTP Messages must separate header and body by an empty line
  this->socket << sep;

  // Send the body contents
  // TODO(any): body must be skipped in responses to HEAD/CONNECT request
  // methods and for 1xx, 204, and 304 responses. See RFC-7230
  // https://tools.ietf.org/html/rfc7230#section-3.3
  // TODO(any): stringstream is not suitable for binary data
  this->socket << this->body().str();

  // Flush de bytes to the peer
  return this->socket.send();
}

bool HttpResponse::sendBodyMetadata() {
  const std::string& sep = HttpMessage::lineSeparator;

  // Check Content-Type was provided
  const std::string& contentType = this->getHeader("Content-Type");
  if (contentType.empty()) {
    // No Content-Type was set, guess one and send it to client
    const std::string& guess = this->guessContentType();
    if (guess.length() > 0 && !(this->socket << guess << sep)) {
      // return false;
    }
  }

  // Check Content-length was provided
  const std::string& contentLength = this->getHeader("Content-Length");
  if (contentLength.empty()) {
    // No Content-length was set, send the body length
    if (!(this->socket << "Content-Length: " << this->getBodyLength() << sep)) {
      // return false;
    }
  }

  return true;
}

std::string HttpResponse::buildStatusLine() const {
  return this->httpVersion + ' ' + std::to_string(this->statusCode) + ' '
    + this->reasonPhrase;
}

// {Code, "Reason-Phrase"}
const HttpResponse::ReasonPhrases HttpResponse::reasonPhrases = {
  {100, "Continue"},
  {101, "Switching Protocols"},
  {200, "OK"},
  {201, "Created"},
  {202, "Accepted"},
  {203, "Non-Authoritative Information"},
  {204, "No Content"},
  {205, "Reset Content"},
  {206, "Partial Content"},
  {300, "Multiple Choices"},
  {301, "Moved Permanently"},
  {302, "Found"},
  {303, "See Other"},
  {304, "Not Modified"},
  {305, "Use Proxy"},
  {307, "Temporary Redirect"},
  {400, "Bad Request"},
  {401, "Unauthorized"},
  {402, "Payment Required"},
  {403, "Forbidden"},
  {404, "Not Found"},
  {405, "Method Not Allowed"},
  {406, "Not Acceptable"},
  {407, "Proxy Authentication Required"},
  {408, "Request Timeout"},
  {409, "Conflict"},
  {410, "Gone"},
  {411, "Length Required"},
  {412, "Precondition Failed"},
  {413, "Payload Too Large"},
  {414, "URI Too Long"},
  {415, "Unsupported Media Type"},
  {416, "Range Not Satisfiable"},
  {417, "Expectation Failed"},
  {426, "Upgrade Required"},
  {500, "Internal Server Error"},
  {501, "Not Implemented"},
  {502, "Bad Gateway"},
  {503, "Service Unavailable"},
  {504, "Gateway Timeout"},
  {505, "HTTP Version Not Supported"},
};
