// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include <fb/fbjni.h>
#include <fb/xplat_init.h>

#include "JHermesInstance.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
  return facebook::xplat::initialize(
      vm, [] { facebook::react::JHermesInstance::registerNatives(); });
}
