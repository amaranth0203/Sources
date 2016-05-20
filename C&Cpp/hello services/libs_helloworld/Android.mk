LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    IHelloWorldService.cpp \
    BnHelloWorldService.cpp \
    BpHelloWorldService.cpp \
    HelloWorldService.cpp

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libbinder \

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libhelloworld

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
