/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <exception>
#include <map>

#include "Consumer.hpp"

/**
 * @brief A dispatcher is both, a consumer of its own queue, and a producer
 * for many queues. For each data that is pushed to its queue, the dispatcher
 * distributes the data to another queues, using key-values
 */
template <typename KeyType, typename DataType>
class Dispatcher : public Consumer<DataType> {
  /// Objects of this class cannot be copied
  DISABLE_COPY(Dispatcher);

 protected:
  /// Alias to the inherited queue for a more meaninguful identifier
  Queue<DataType>*& fromQueue = Consumer<DataType>::consumingQueue;

  /// This thread will distribute elements to the following queues
  std::map<KeyType, Queue<DataType>*> toQueues;

 public:
  /// Constructor
  explicit Dispatcher(Queue<DataType>* consumingQueue = nullptr
    , const DataType& stopCondition = DataType()
    , bool createOwnQueue = false)
    : Consumer<DataType>(consumingQueue, stopCondition, createOwnQueue) {
  }

  /// Destructor
  virtual ~Dispatcher() {
  }

  /// Register a map. When the data to be consumed has this key, it will be
  /// redirected to the the given queue
  inline void registerRedirect(const KeyType& key, Queue<DataType>* toQueue) {
    this->toQueues[key] = toQueue;
  }

  /// Override this method to process any data extracted from the queue
  void consume(DataType data) override {
    const KeyType& key = this->extractKey(data);
    const auto& itr = this->toQueues.find(key);
    if ( itr == this->toQueues.end() ) {
      throw std::runtime_error("dispatcher: queue's key not found");
    }
    itr->second->enqueue(data);
  }

  /// Override this method to extract the key from a data stored in fromQueue
  virtual KeyType extractKey(const DataType& data) const = 0;
};

#endif  // DISPATCHER_HPP
