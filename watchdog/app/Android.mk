# Copyright 2006 The Android Open Source Project

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	watchdog-unit-test.c	

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= watchdog-unit-test
include $(BUILD_EXECUTABLE)

