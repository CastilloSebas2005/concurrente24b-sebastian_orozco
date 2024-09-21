/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef NETWORKMESSAGE_HPP
#define NETWORKMESSAGE_HPP

#include <cstdint>

/**
 * @brief The message to be transmitted by the simulated network
 */
struct NetworkMessage {
 public:
  /// The target consumer
  uint16_t target = 0;
  /// The source producer
  uint16_t source = 0;
  /// The contents of the message (body or payload)
  uint64_t messageNumber = 0;

 public:
  /// Convenience constructor
  NetworkMessage(uint16_t target = 0, uint16_t source = 0
      , uint64_t messageNumber = 0)
    : target(target)
    , source(source)
    , messageNumber(messageNumber) {
  }

  /// Returns true if this message is equals to the given
  inline bool operator==(const NetworkMessage& other) const {
    return this->target == other.target
      && this->source == other.source
      && this->messageNumber == other.messageNumber;
  }
};

#endif  // NETWORKMESSAGE_HPP
