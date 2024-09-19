// Copyright 2020-2024 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

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
    + " messages consumed");
  return EXIT_SUCCESS;
}

void ConsumerTest::consume(NetworkMessage data) {
  (void)data;
  ++this->receivedMessages;
}
