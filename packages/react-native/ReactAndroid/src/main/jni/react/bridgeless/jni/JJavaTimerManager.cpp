// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include "JJavaTimerManager.h"

#include <fb/fbjni.h>
#include <jni.h>

namespace facebook {
namespace react {

void JJavaTimerManager::createTimer(
    uint32_t timerID,
    double duration,
    bool repeat) {
  static const auto method =
      javaClassStatic()->getMethod<void(jint, jlong, jboolean)>("createTimer");
  method(self(), timerID, (long)duration, static_cast<unsigned char>(repeat));
}

void JJavaTimerManager::deleteTimer(uint32_t timerID) {
  static const auto method =
      javaClassStatic()->getMethod<void(jint)>("deleteTimer");
  method(self(), timerID);
}

} // namespace react
} // namespace facebook
