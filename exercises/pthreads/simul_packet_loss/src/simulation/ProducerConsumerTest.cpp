/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <cstdlib>
#include <iostream>

#include "ProducerConsumerTest.hpp"
#include "AssemblerTest.hpp"
#include "ConsumerTest.hpp"
#include "DispatcherTest.hpp"
#include "ProducerTest.hpp"

const char* const usage =
  "Usage: prodcons packets prod cons prod_delay disp_delay cons_delay loss"
  "capacity\n"
  "\n"
  "  packets     number of packages to be produced\n"
  "  producers   number of producer threads\n"
  "  consumers   number of consumer threads\n"
  "  prod_delay  delay of producer to create a package\n"
  "  disp_delay  delay of dispatcher to dispatch a package\n"
  "  cons_delay  delay of consumer to consume a package\n"
  "  loss_perc   percent of package loss (0-100)\n"
  "  capacity    queue capacity\n"
  "\n"
  "Delays are in millisenconds, negatives are maximums for random delays\n";

ProducerConsumerTest::~ProducerConsumerTest() {
  for (size_t index = 0; index < this->producerCount; ++index) {
    delete this->producers[index];
  }
  delete this->dispatcher;
  for (ConsumerTest* consumer : this->consumers) {
    delete consumer;
  }
  delete this->assembler;
}

int ProducerConsumerTest::start(int argc, char* argv[]) {
  // Parse arguments and store values in object members
  if ( int error = this->analyzeArguments(argc, argv) ) {
    return error;
  }

  // Create objects for the simulation
  this->producers.resize(this->producerCount);
  for (size_t index = 0; index < this->producerCount; ++index) {
    this->producers[index] = new ProducerTest(index, this->producerCount,
      this->packageCount, this->productorDelay, this->consumerCount,
      this->producedPackets, this->canAccessProducedPackets);
    assert(this->producers[index]);
  }
  this->dispatcher = new DispatcherTest(this->dispatcherDelay);
  this->dispatcher->createOwnQueue(this->queueCapacity);
  // Create each producer
  this->consumers.resize(this->consumerCount);
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index] = new ConsumerTest(this->consumerDelay);
    assert(this->consumers[index]);
    this->consumers[index]->createOwnQueue(this->queueCapacity);
  }
  this->assembler = new AssemblerTest(this->packageLossPercent,
      this->consumerCount);
  this->assembler->createOwnQueue(this->queueCapacity);

  // Communicate simulation objects
  // Producer push network messages to the dispatcher queue
  for (size_t index = 0; index < this->producerCount; ++index) {
    this->producers[index]->setProducingQueue(
        this->assembler->getConsumingQueue());
  }
  // Dispatcher delivers to each consumer, and they should be registered
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->dispatcher->registerRedirect(index + 1
      , this->consumers[index]->getConsumingQueue());
  }
  // this->dispatcher->registerRedirect(this->consumerCount + 1,
  //     this->assembler->getConsumingQueue());
  this->assembler->setProducingQueue(this->dispatcher->getConsumingQueue());

  // Start the simulation
  for (size_t index = 0; index < this->producerCount; ++index) {
    this->producers[index]->startThread();
  }
  this->dispatcher->startThread();
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->startThread();
  }
  this->assembler->startThread();

  // Wait for objets to finish the simulation
  for (size_t index = 0; index < this->producerCount; ++index) {
    this->producers[index]->waitToFinish();
  }
  this->dispatcher->waitToFinish();
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->waitToFinish();
  }
  this->assembler->waitToFinish();

  // Simulation finished
  return EXIT_SUCCESS;
}

int ProducerConsumerTest::analyzeArguments(int argc, char* argv[]) {
  // 7 + 1 arguments are mandatory
  if ( argc < 8 ) {
    std::cout << usage;
    return EXIT_FAILURE;
  }

  int index = 1;
  this->packageCount = std::strtoull(argv[index++], nullptr, 10);
  this->producerCount = std::strtoull(argv[index++], nullptr, 10);
  this->consumerCount = std::strtoull(argv[index++], nullptr, 10);
  this->productorDelay = std::atoi(argv[index++]);
  this->dispatcherDelay = std::atoi(argv[index++]);
  this->consumerDelay = std::atoi(argv[index++]);
  this->packageLossPercent = std::atof(argv[index++]);

  if (argc >= 9) {
    this->queueCapacity = std::strtoul(argv[index++], nullptr, 10);
  }

  // todo: Validate that given arguments are fine
  return EXIT_SUCCESS;
}
