// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include <fb/fbjni.h>
#include <fb/xplat_init.h>
#include <react/jni/JReactMarker.h>

#include "JJSTimerExecutor.h"
#include "JReactInstance.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * /*unused*/) {
  return facebook::xplat::initialize(vm, [] {
    facebook::react::JReactMarker::setLogPerfMarkerIfNeeded();
    facebook::react::JReactInstance::registerNatives();
    facebook::react::JJSTimerExecutor::registerNatives();
  });
}
