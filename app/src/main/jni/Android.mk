LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCVROOT:= C:\Program Files (x86)\OpenCV-android-sdk
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include C:\OpenCV-android-sdk\sdk\native\jni\OpenCV.mk

LOCAL_MODULE := MyLibrary
LOCAL_SRC_FILES := MyLibrary.cpp
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)