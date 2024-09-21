/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef PRODUCERTEST_HPP
#define PRODUCERTEST_HPP

#include <mutex>

#include "NetworkMessage.hpp"
#include "Producer.hpp"

/**
 * @brief A productor class example
 * Produces network messages and push them to the queue
 */
class ProducerTest : public Producer<NetworkMessage> {
  DISABLE_COPY(ProducerTest);

 protected:
  /// My producer number
  const size_t rank;
  /// Number of producers
  const size_t& producerCount;
  /// Number of packages to be produced
  const size_t& packageCount;
  /// Delay of producer to create a package, negative for max random
  const int& productorDelay;
  /// Number of consumer threads
  const size_t& consumerCount;
  /// Shared count of packets currently created by producers
  size_t& producedPackets;
  /// Protects the incremental packet counter
  std::mutex& canAccessProducedPackets;
  /// Number of messages that I produced
  size_t myProducedCount = 0;

 public:
  /// Constructor
  ProducerTest(const size_t rank, const size_t& producerCount,
    const size_t& packageCount, const int& productorDelay,
    const size_t& consumerCount, size_t& producedPackets,
    std::mutex& canAccessProducedPackets);
  /// Do the message production in its own execution thread
  int run() override;
  /// Creates a simulation message to be sent by the network
  NetworkMessage createMessage(size_t index) const;
};

#endif  // PRODUCERTEST_HPP
