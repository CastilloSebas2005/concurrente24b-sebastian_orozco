/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef CONSUMERTEST_HPP
#define CONSUMERTEST_HPP

#include "Consumer.hpp"
#include "NetworkMessage.hpp"

/**
 * @brief A ConsumerTest class example
 */
class ConsumerTest : public Consumer<NetworkMessage> {
  DISABLE_COPY(ConsumerTest);

 protected:
  /// Delay of consumer to consume a package, negative for max random
  int consumerDelay = 0;
  /// Number of messages received
  size_t receivedMessages = 0;

 public:
  /// Constructor
  explicit ConsumerTest(int consumerDelay);
  /// Consume the messages in its own execution thread
  int run() override;
  /// Override this method to process any data extracted from the queue
  void consume(NetworkMessage data) override;
};

#endif  // CONSUMERTEST_HPP
