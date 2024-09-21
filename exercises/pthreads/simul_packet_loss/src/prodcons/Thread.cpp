/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <cassert>
#include <cstdlib>

#include "Thread.hpp"

Thread::Thread() {
}

Thread::~Thread() {
  delete this->thread;
}

int Thread::startThread() {
  assert(this->thread == nullptr);
  this->thread = new std::thread( &Thread::run, this );
  return EXIT_SUCCESS;
}

int Thread::waitToFinish() {
  assert(this->thread);
  this->thread->join();

  delete this->thread;
  this->thread = nullptr;

  return EXIT_SUCCESS;
}
