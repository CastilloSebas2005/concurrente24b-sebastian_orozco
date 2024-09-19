// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <vector>

#include "common.hpp"

/**
 * @brief Container class for common utility methods.
 */
class Util {
  DISABLE_COPY(Util);
  /// Constructor
  Util() = delete;
  /// Destructor
  ~Util() = delete;

 public:  // Random number generation
  /// Generates a pseudo-random integer number in range [min, max[
  static int random(int min, int max);
  /// Generates a pseudo-random real number in range [min, max[
  static double random(double min, double max);

 public:  // Concurrency
  /**
   * Forces current execution thread to sleep for at least a given amount of
   * milliseconds.
   * 
   * Utility method to simulate workload using sleep. This method is for 
   * illustrative goals. Sleeps must be NEVER used for real projects.
   * @param milliseconds If positive, the thread that executes sleepFor() will
   * wait for CPU at least for the exact amount of milliseconds. If a negative
   * number is given, execution thread will sleep for a random number of
   * milliseconds generated in range [0, milliseconds[
   */
  static void sleepFor(int milliseconds);

 public:  // String operations
  /**
   * Splits a text where it contains a delimiter and place the splitted texts
   * into a container. Example:
   * 
   * @code {.cpp}
      std::vector<std::string> values;
      Util::tokenize("2.4; 3,4; ", values, ";", false);
      // values == ["2.4", " 3,4 ", " "]
      values.clear();
      Util::tokenize("2.4; 3,4; ", values, ";", true);
      // values == ["2.4", " 3,4 "]
   * @endcode
   * 
   * @a Container Type of container, e.g: std::vector or std::list.
   * @param text Text to be splitted.
   * @param tokens Reference to the container object where splitted text will
   * be stored.
   * @param delim In each occurrence of this text, a split is made.
   * @param trimEmpty Send true if you want to avoid generating empty splitted
   * text into the container
   */
  template <typename Container>
  static void tokenize(const std::string& text, Container& tokens,
      const std::string& delim = " ", bool trimEmpty = false) {
    // Adapted from https://stackoverflow.com/a/1493195
    using value_type = typename Container::value_type;
    using size_type  = typename Container::size_type;

    // Index where the delimiter is found
    const std::string::size_type length = text.length();
    std::string::size_type lastPos = 0;

    // While we have not reached the end of the text
    while (lastPos < length + 1) {
      // Find the delimiter
      // TODO(any): Pass the finder function by argument, e.g: find_first_of
      std::string::size_type pos = /*(text.*finder)*/text.find(delim, lastPos);
      // If no delimiter is found, the token ends at the last char of text
      if (pos == std::string::npos) {
        pos = length;
      }

      // If token has content or empty tokens are wanted
      if (pos != lastPos || !trimEmpty) {
        // Add the token to the container
        tokens.push_back(value_type(text.data() + lastPos
          , size_type(pos) - lastPos));
      }

      // Continue searching at the next character after the end of this token
      lastPos = pos + 1;
    }
  }

  /**
   * Splits a text string into array of subtexts. Example
   * 
   * @code {.cpp}
      const std::vector<std::string>& values1 = Util::split("2.4; 3,4; ", ";");
      // values1 == ["2.4", " 3,4 ", " "]
      Util::split("2.4; 3,4; ", ";", true);
      // returns ["2.4", " 3,4 "]
   * @endcode
   * @param text The text where the search is done.
   * @param delim In each occurrence of this text, a split is made.
   * @param trimEmpty Send true if you want to avoid generating empty splitted
   * texts into the vector
   * @return A vector of the splitted texts
   */
  static std::vector<std::string> split(const std::string& text,
    const std::string& delim = " ", bool trimEmpty = false);
};

#endif  // UTIL_HPP
