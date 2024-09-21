/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>

#include "common.hpp"

/**
 * @brief Base and wrapper class for managing execution threads
 */
class Thread {
  DISABLE_COPY(Thread);

 protected:
  /// Object that controls the thread
  std::thread* thread = nullptr;

 public:
  /// Constructor
  Thread();
  /// Destructor
  virtual ~Thread();
  /// Create a new thread, and then, executes run()
  /// @return Error code, 0 for success
  int startThread();
  /// Stop execution of this server/daemon, called by Ctrl+C or signal
  /// @return Error code, 0 for success
  int waitToFinish();

 protected:
  /// This is the first method to be called on the stack of the new thread
  /// @return Error code, 0 for success
  virtual int run() = 0;
};

#endif  // THREAD_HPP
