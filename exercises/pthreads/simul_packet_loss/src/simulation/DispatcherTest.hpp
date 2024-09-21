/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

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
