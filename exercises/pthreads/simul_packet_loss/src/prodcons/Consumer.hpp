/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef CONSUMER_HPP
#define CONSUMER_HPP

#include <cassert>
#include <climits>

#include "Queue.hpp"
#include "Thread.hpp"

/**
 * @brief A template that generates abstract base classes for Consumers
 * Consumers are execution threads. They extract elements from a queue
 * These elements were pushed to the queue by a producer or dispatcher thread.
 * @remark For this software, the consumer is the owner of its queue
 */
template <typename DataType>
class Consumer : public virtual Thread {
  /// Objects of this class cannot be copied
  DISABLE_COPY(Consumer);

 protected:
  /// This thread will consume from its queue
  Queue<DataType>* consumingQueue;
  /// This data will be used to represent that the Consumer must stop the
  /// consumption, and finish its work. It is used for cleaning purposes.
  const DataType stopCondition;
  /// True if this consumer owns the queue and it must be deleted in destructor
  bool ownsQueue;

 public:
  /// Creates a consumer that uses an existing queue or creates its own
  /// @see stopCondition
  explicit Consumer(Queue<DataType>* consumingQueue = nullptr
    , const DataType& stopCondition = DataType()
    , bool createOwnQueue = false
    , const unsigned queueCapacity = SEM_VALUE_MAX)
    : consumingQueue(consumingQueue)
    , stopCondition(stopCondition)
    , ownsQueue(createOwnQueue) {
    // Error if asked to create own queue and provided an existing one to use
    assert(!(createOwnQueue && consumingQueue));
    if (createOwnQueue) {
      this->createOwnQueue(queueCapacity);
    }
  }

  /// Destructor
  virtual ~Consumer() {
    if (this->ownsQueue) {
      delete this->consumingQueue;
    }
  }

  /// Get access to the queue where this thread will consume
  inline Queue<DataType>* getConsumingQueue() {
    return this->consumingQueue;
  }

  /// Set the queue where this thread will consume elements
  inline void setConsumingQueue(Queue<DataType>* consumingQueue) {
    this->consumingQueue = consumingQueue;
  }

  /// Creates a new empty queue owned by this consumer
  void createOwnQueue(const unsigned capacity = SEM_VALUE_MAX) {
    assert(this->consumingQueue == nullptr);
    this->consumingQueue = new Queue<DataType>(capacity);
    this->ownsQueue = true;
  }

  /// Consumes from its queue, util the stop condition is popped
  /// For each data consumed, the @a consume method will be called
  virtual void consumeForever() {
    assert(this->consumingQueue);
    while (true) {
      // Get the next data to consume, or block while queue is empty
      const DataType& data = this->consumingQueue->dequeue();
      // If data is the stop condition, stop the loop
      if ( data == this->stopCondition ) {
        break;
      }
      // Process this data
      this->consume(data);
    }
  }

  /// Override this method to process any data extracted from the queue
  virtual void consume(DataType data) = 0;
};

#endif  // CONSUMER_HPP
