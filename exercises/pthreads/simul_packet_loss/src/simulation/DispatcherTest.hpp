// Copyright 2020-2024 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

#ifndef DISPATCHERTEST_HPP
#define DISPATCHERTEST_HPP

#include "Dispatcher.hpp"
#include "NetworkMessage.hpp"

/**
 * @brief A Dispatcher class example
 */
class DispatcherTest : public Dispatcher<int, NetworkMessage> {
  DISABLE_COPY(DispatcherTest);

 protected:
  /// Delay of dispatcher to dispatch a package, negative for max random
  int dispatcherDelay = 0;

 public:
  /// Constructor
  explicit DispatcherTest(int dispatcherDelay);
  /// Start redirecting network messages
  int run() override;
  /// This method extracts the key from a data stored in the fromQueue
  int extractKey(const NetworkMessage& data) const override;
};

#endif  // DISPATCHERTEST_HPP
