LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog

LOCAL_SRC_FILES:= \
    main_helloworldservice.cpp

LOCAL_SHARED_LIBRARIES := \
    libutils \
    libbinder \
    libhelloworld
    base := $(LOCAL_PATH)/../..

LOCAL_MODULE:= helloworldservice

LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)



include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog

LOCAL_SRC_FILES:= \
    main_helloworldclient.cpp

LOCAL_SHARED_LIBRARIES := \
    libutils \
    libbinder \
    libhelloworld
    base := $(LOCAL_PATH)/../..
    
LOCAL_MODULE:= helloworldclient

LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
