// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include "HttpRequest.hpp"
#include "Socket.hpp"

HttpRequest::HttpRequest(const Socket& socket)
  : HttpMessage(socket) {
}

HttpRequest::~HttpRequest() {
}

bool HttpRequest::parse() {
  return this->parseRequestLine()
    && this->parseHeader()
    && this->parseBody();
}

bool HttpRequest::parseRequestLine() {
  // Try to read a line from the socket
  std::string line;
  if (!this->socket.readLine(line)) {
    return false;
  }

  // Extract the request fields from the line
  std::istringstream requestLine(line);
  if (!(requestLine >> this->method >> this->uri >> this->httpVersion)) {
    return false;
  }

  return true;
}

bool HttpRequest::parseHeader() {
  while (true) {
    // Try to read a line from the socket
    std::string line;
    if (!this->socket.readLine(line)) {
      return false;
    }

    // Remove trailing '\r' if any
    if (line.length() > 0 && line[line.length() - 1] == '\r') {
      line.erase(line.length() - 1);
    }

    // If empty line is found, the header is completely loaded
    if (line.empty()) {
      return true;
    }

    // A header is a pair "Key: value"
    size_t pos = line.find(':');
    if (pos == line.npos) {
      // Header has a non-valid entry
      return false;
    }

    // Get the key
    const std::string& key = line.substr(0, pos);

    // Ignore whitespace after colon such as "Key: value"
    if (pos + 1 < line.length() && ::isspace(line[pos + 1])) {
      ++pos;
    }

    // Get the value
    const std::string& value = line.substr(pos + 1);
    // Insert the (key, value) pair in the headers associative array
    // REMARK: if a key is repeated, the new value overwrites the older one
    this->headers[key] = value;
  }
}

bool HttpRequest::parseBody() {
  // TODO(any): Multi-part requests are not supported. See Transfer-Encoding
  // https://tools.ietf.org/html/rfc7230#section-3.3

  // String conversions may fail
  try {
    // If client provided a Content-length field, read the body
    const std::string& value = this->getHeader("Content-Length");
    if (value.length() > 0) {
      // Convert the Content-Length value, an exception is raised on error
      size_t size = std::stoull(value);
      // Create a buffer, an exception is raised if no enough memory
      std::vector<char> buffer(size);
      // Read the body from the socket
      if (!this->socket.read(buffer.data(), size)) {
        return false;
      }
      // Use the data loaded into the buffer as the body of the message
      // TODO(any): stringstream is not suitable to manage large/binary data
      this->body().str(buffer.data());
    }
  } catch (const std::exception& error) {
    return false;
  }

  return true;
}
