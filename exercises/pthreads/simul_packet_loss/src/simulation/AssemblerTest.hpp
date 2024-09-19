
#ifndef ASSEMBLERTEST_HPP
#define ASSEMBLERTEST_HPP

#include <iostream>

#include "Assembler.hpp"
#include "NetworkMessage.hpp"
#include "Queue.hpp"

class AssemblerTest
  :public Assembler<NetworkMessage, NetworkMessage> {
 protected:
  /// Number of messages received
  int lossProbability = 0;
  int consumerCount = 0;
  /// Delay of consumer to consume a package, negative for max random
  int assemblerDelay = 0;
  /// Number of messages received
  size_t discardedMessages = 0;
  int id = 0;
 public:
  /// Constructor
  AssemblerTest(int lossProbability, int consumerCount
      , int assemblerDelay, int id);

  /// Destructor
  virtual ~AssemblerTest() {}
  int run() override;
  /// Consume a product
  void consume(NetworkMessage data) override;
  NetworkMessage createMessageRand() const;
  Queue<NetworkMessage>* getQueue();
};
#endif  // ASSEMBLERTEST_HPP