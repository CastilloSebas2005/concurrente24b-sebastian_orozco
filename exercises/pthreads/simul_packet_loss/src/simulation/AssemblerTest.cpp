// Copyright 2024 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
#include "AssemblerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"

AssemblerTest::AssemblerTest(const double packageLossPercent,
    const size_t consumerCount)
  : packageLossPercent(packageLossPercent)
  , consumerCount(consumerCount) {
}

int AssemblerTest::run() {
  // Start the forever loop to consume all the messages that arrive
  this->consumeForever();

  // this->produce(NetworkMessage());
  this->produce(this->stopCondition);

  // If the forever loop finished, no more messages will arrive
  // Print statistics
  Log::append(Log::INFO, "Assembler", std::to_string(this->packageLossCount)
    + " menssages lost");
  return EXIT_SUCCESS;
}

void AssemblerTest::consume(NetworkMessage data) {
  if (Util::random(0.0, 100.0) < this->packageLossPercent) {
    ++this->packageLossCount;
  } else {
    // data.target = Util::random(1, this->consumerCount + 1);

    // Con esta linea se vuelven a poner los mensajes en la cola
    this->producingQueue->enqueue(data);
  }
}
