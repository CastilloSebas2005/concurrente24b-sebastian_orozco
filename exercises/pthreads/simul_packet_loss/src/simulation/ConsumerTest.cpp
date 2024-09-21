/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include "ConsumerTest.hpp"
#include "Log.hpp"

ConsumerTest::ConsumerTest(int consumerDelay)
  : consumerDelay(consumerDelay) {
}

int ConsumerTest::run() {
  // Start the forever loop to consume all the messages that arrive
  this->consumeForever();

  // If the forever loop finished, no more messages will arrive
  // Print statistics
  Log::append(Log::INFO, "Consumer", std::to_string(this->receivedMessages)
    + " menssages consumed");
  return EXIT_SUCCESS;
}

void ConsumerTest::consume(NetworkMessage data) {
  (void)data;
  ++this->receivedMessages;
}
