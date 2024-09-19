// Copyright 2020-2024 Jeisson Hidalgo-Cespedes. ECCI-UCR. CC BY 4.0

#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include <cassert>

#include "Queue.hpp"
#include "Thread.hpp"

/**
 * @brief A template that generates abstract base classes for Producers
 * Producers are execution threads. They create elements that are pushed
 * to a queue. These elements will be popped by a consumer thread.
 */
template <typename DataType>
class Producer : public virtual Thread {
  /// Objects of this class cannot be copied
  DISABLE_COPY(Producer);

 protected:
  /// This thread will produce for this queue
  Queue<DataType>* producingQueue;

 public:
  /// Constructor
  explicit Producer(Queue<DataType>* producingQueue = nullptr)
    : producingQueue(producingQueue) {
  }

  /// Destructor
  virtual ~Producer() {
  }

  /// Get access to the queue where this thread will produce
  inline Queue<DataType>* getProducingQueue() {
    return this->producingQueue;
  }

  /// Set the queue where this thread will produce elements
  inline void setProducingQueue(Queue<DataType>* producingQueue) {
    this->producingQueue = producingQueue;
  }

  /// Add to the queue the produced data unit
  virtual void produce(const DataType& data) {
    assert(this->producingQueue);
    this->producingQueue->enqueue(data);
  }
};

#endif  // PRODUCER_HPP
