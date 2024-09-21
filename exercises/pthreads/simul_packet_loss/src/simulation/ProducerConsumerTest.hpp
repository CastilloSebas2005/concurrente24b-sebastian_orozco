/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef PRODUCERCONSUMERTEST_HPP
#define PRODUCERCONSUMERTEST_HPP

#include <climits>
#include <cstddef>
#include <mutex>
#include <vector>

#include "common.hpp"

// Forward declarations
class AssemblerTest;
class ConsumerTest;
class DispatcherTest;
class ProducerTest;

/**
 * @brief Controller class that simulates the message passing between
 * producer and cosumers through a dispatcher
 */
class ProducerConsumerTest {
  DISABLE_COPY(ProducerConsumerTest);

 private:
  /// Number of packages to be produced
  size_t packageCount = 0;
  /// Number of consumer threads
  size_t producerCount = 0;
  /// Number of consumer threads
  size_t consumerCount = 0;
  /// Delay of producer to create a package, negative for max random
  int productorDelay = 0;
  /// Delay of dispatcher to dispatch a package, negative for max random
  int dispatcherDelay = 0;
  /// Delay of consumer to consume a package, negative for max random
  int consumerDelay = 0;
  /// Package loss percent
  double packageLossPercent = 0.0;
  /// Maximum capacity of queues in messages
  unsigned queueCapacity = SEM_VALUE_MAX;

 private:
  /// Producer of the simulated network messages
  std::vector<ProducerTest*> producers;
  /// A dispatcher of the of the simulated network messages
  DispatcherTest* dispatcher = nullptr;
  /// Consumers of the simulated network messages
  std::vector<ConsumerTest*> consumers;
  /// Simulate loss of network messages
  AssemblerTest* assembler = nullptr;
  /// Shared count of packets currently created by producers
  size_t producedPackets = 0;
  /// Protects the incremental packet counter
  std::mutex canAccessProducedPackets;

 public:
  /// Constructor
  ProducerConsumerTest() = default;
  /// Destructor
  ~ProducerConsumerTest();
  /// Start the simulation
  int start(int argc, char* argv[]);

 private:
  /// Analyze the command line arguments
  int analyzeArguments(int argc, char* argv[]);
};

#endif  // PRODUCERCONSUMERTEST_HPP
