// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include "BufferedRuntimeExecutor.h"
#include <cxxreact/MessageQueueThread.h>
#include <algorithm>

namespace facebook {
namespace react {

BufferedRuntimeExecutor::BufferedRuntimeExecutor(
    RuntimeExecutor runtimeExecutor)
    : runtimeExecutor_(runtimeExecutor),
      isBufferingEnabled_(true),
      lastIndex_(0) {}

void BufferedRuntimeExecutor::execute(Work &&callback) {
  if (!isBufferingEnabled_) {
    // Fast path: Schedule directly to RuntimeExecutor, without locking
    runtimeExecutor_(std::move(callback));
    return;
  }

  /**
   * Note: std::mutex doesn't have a FIFO ordering.
   * To preserve the order of the buffered work, use a priority queue and
   * track the last known work index.
   */
  uint64_t newIndex = lastIndex_++;
  std::lock_guard<std::mutex> guard(lock_);
  if (isBufferingEnabled_) {
    queue_.push({.index_ = newIndex, .work_ = std::move(callback)});
    return;
  }

  // Force flush the queue to maintain the execution order.
  unsafeFlush();

  runtimeExecutor_(std::move(callback));
}

void BufferedRuntimeExecutor::flush() {
  std::lock_guard<std::mutex> guard(lock_);
  unsafeFlush();
  isBufferingEnabled_ = false;
}

void BufferedRuntimeExecutor::unsafeFlush() {
  while (queue_.size() > 0) {
    BufferedWork const &bufferedWork = queue_.top();
    Work work = std::move(bufferedWork.work_);
    runtimeExecutor_(std::move(work));
    queue_.pop();
  }
}

} // namespace react
} // namespace facebook
