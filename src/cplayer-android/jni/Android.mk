LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := cplayer
LOCAL_CXXFLAGS := -DSUPPORT_ANDROID -DUSE_GLES2 -DTRACE=\"triangle.inl\"
LOCAL_LDLIBS := -landroid -llog -lGLESv2 -lEGL
LOCAL_STATIC_LIBRARIES := android_native_app_glue

LOCAL_CPP_FEATURES += exceptions
LOCAL_CPP_FLAGS += -O0

LOCAL_SRC_FILES := ../androidplayer.cpp

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
