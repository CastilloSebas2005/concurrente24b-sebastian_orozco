/// @author Sebastian Venegas Brenes
/// @copyright Sebastian Venegas Brenes 2023

#include "AssemblerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"

AssemblerTest::AssemblerTest(int lossProbability, int consumerCount
  , int assemblerDelay, int id)
  : Assembler()
  , lossProbability(lossProbability)
  , consumerCount(consumerCount)
  , assemblerDelay(assemblerDelay)
  , id(id) {}

void AssemblerTest::consume(NetworkMessage data) {
  (void)data;

  double random = 1 + Util::random(0, 100);
  if (random > this->lossProbability) {
    produce(createMessageRand());
  } else {
    this->discardedMessages++;
  }
}

int AssemblerTest::run() {
  this->consumeForever();
  Log::append(Log::INFO, "Assembler", std::to_string(this->discardedMessages) + " messages lost");
  return EXIT_FAILURE;
}

Queue<NetworkMessage>*  AssemblerTest::getQueue() {
  return this->consumingQueue;
}

NetworkMessage AssemblerTest::createMessageRand() const {
  // Source is consumerCount + 1: this producer/assembler
  const uint16_t source = this->id;
  // Target is selected by random
  const uint16_t target = 1 + Util::random(0
    , static_cast<int>(this->consumerCount));

  return NetworkMessage(target, source);
}