/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include "ProducerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"

ProducerTest::ProducerTest(const size_t rank, const size_t& producerCount,
    const size_t& packageCount, const int& productorDelay,
    const size_t& consumerCount, size_t& producedPackets,
    std::mutex& canAccessProducedPackets)
  : rank(rank)
  , producerCount(producerCount)
  , packageCount(packageCount)
  , productorDelay(productorDelay)
  , consumerCount(consumerCount)
  , producedPackets(producedPackets)
  , canAccessProducedPackets(canAccessProducedPackets) {
}

int ProducerTest::run() {
  // Produce each asked message
  size_t myPacketNumber = 0;
  while (true) {
    this->canAccessProducedPackets.lock();
      myPacketNumber = ++this->producedPackets;
    this->canAccessProducedPackets.unlock();
    if (myPacketNumber > this->packageCount) {
      break;
    }
    this->produce(this->createMessage(myPacketNumber));
    ++this->myProducedCount;
  }

  // Produce an empty message to communicate we finished
  if (myPacketNumber == this->packageCount + this->producerCount) {
    this->produce(NetworkMessage());
  }

  // Report production is done
  Log::append(Log::INFO, "Producer", std::to_string(this->myProducedCount)
    + " messages sent");
  return EXIT_SUCCESS;
}

NetworkMessage ProducerTest::createMessage(size_t index) const {
  // Source is me, this producer
  const uint16_t source = this->rank + 1;
  // Target is selected by random
  const uint16_t target = 1 + Util::random(0
    , static_cast<int>(this->consumerCount));
  // IMPORTANT: This simulation uses sleep() to mimics the process of
  // producing a message. However, you must NEVER use sleep() for real projects
  Util::sleepFor(this->productorDelay);
  // Create and return a copy of the network message
  return NetworkMessage(target, source, index);
}
