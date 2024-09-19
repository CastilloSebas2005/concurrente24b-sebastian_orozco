// Copyright 2021,2023 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#include <cctype>

#include <map>
#include <memory>
#include <sstream>
#include <string>

#include "common.hpp"
#include "Socket.hpp"

/// @brief Case-insensitive comparing less-than functor
/// @details Adapted from https://stackoverflow.com/a/1801913
struct IgnoreCaseLess {
  /// @brief Functor for case insensitive less-than comparison of two chars
  struct CompareChar {
    /// @brief Return true if c1 < c2 ignoring case
    /// @param c1 First char to compare
    /// @param c2 Second char to compare
    /// @return true if c1 < c2 ignoring case
    inline bool operator()(unsigned char c1, unsigned char c2) const {
      return ::tolower(c1) < ::tolower(c2);
    }
  };
  /// @brief Return true if s1 < s2 ignoring case
  /// @param s1 First text to compare
  /// @param s2 Second text to compare
  /// @return true if s1 < s2 ignoring case
  inline bool operator()(const std::string& s1, const std::string& s2) const {
    return std::lexicographical_compare(s1.begin(), s1.end()
      , s2.begin(), s2.end(), CompareChar());
  }
};

/**
 * @brief Base class for HTTP request and HTTP response messages.
 * @details See https://jeisson.ecci.ucr.ac.cr/appweb/material/#http_messages
 */
class HttpMessage {
 public:
  /// Objects of this class can be copied, but avoid innecesary copies
  DECLARE_RULE4(HttpMessage, default);
  /// Header metadata is represented as an associative array, but keys are
  /// case-insensitive according to the HTTP standard
  typedef std::map<std::string, std::string, IgnoreCaseLess> Headers;
  /// Line separators for HTTP messages is "\r\n"
  static const char* const lineSeparator;

 protected:
  /// Connection with client to receive or send HTTP data
  Socket socket;
  /// @brief HTTP version, e.g: HTTP/1.0, HTTP/1.1
  /// @details This is the version that a client is using when sending a HTTP
  /// Request. This is the HTTP version that this server uses to answer the
  /// client request. Ideally this server should match the client version.
  /// However, only HTTP 1.0 and 1.1 are supported.
  /// TODO(you) Handle requests concurrently. Match HTTP version used by client
  /// TODO(any) HTTP/2.0 and newer versions are not supported
  std::string httpVersion = "HTTP/1.1";
  /// HTTP message headers (pairs key=value)
  Headers headers;
  /// Body contents is a shared buffer for all copies of this message object
  std::shared_ptr<std::stringstream> sharedBody;

 public:
  /// Constructor
  explicit HttpMessage(const Socket& socket);
  /// Destructor
  ~HttpMessage();
  /// Get access to the connection with the client
  Socket& getSocket();
  /// Get network address from the client
  NetworkAddress getNetworkAddress() const;
  /// @brief Set a key=value header pair.
  /// @details That is, if the key does not exist, it is added to the message
  /// header, otherwise it is overwritten.
  inline void setHeader(const std::string& key, const std::string& value) {
    this->headers[key] = value;
  }
  /// @brief Get the value for a message header key.
  /// @details If the key does not exist, the default value will be returned.
  std::string getHeader(const std::string& key
    , const std::string& defaultValue = "");
  /// Get read-only access to the body object
  inline const std::stringstream& body() const { return *this->sharedBody; }
  /// Get read/write access to the body contents
  inline std::stringstream& body() { return *this->sharedBody; }
  /// Return the length of the body in bytes
  inline size_t getBodyLength() const { return this->body().str().length(); }
  /// Tries to guess the type of content from the body
  /// @return A MIME type text, or empty string if body is empty
  std::string guessContentType() const;
  /// @return true if this object has the same message than the other's
  bool operator==(const HttpMessage& other) const;
};

#endif  // HTTPMESSAGE_H
