LOCAL_PATH := $(call my-dir)
#Save the local path
TMP_PATH := $(LOCAL_PATH)

include $(LOCAL_PATH)/render/libs/libpng/Android.mk

LOCAL_PATH := $(TMP_PATH)
include $(LOCAL_PATH)/render/libs/libjpeg/Android.mk

LOCAL_PATH := $(TMP_PATH)
include $(LOCAL_PATH)/render/libs/freetype/Android/jni/Android.mk

#Reset the local (root) path
LOCAL_PATH := $(TMP_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE    := main
LOCAL_C_INCLUDES := $(LOCAL_PATH)/render/libs/freetype/Android/jni/include
LOCAL_SRC_FILES := 	main.cpp \
					render/fileSystem/FileSystem.cpp \
					render/fileSystem/AndroidFileSystem.cpp \
					render/controllerEGL/ContextControllerEGL.cpp \
					render/shader/Shader.cpp \
					render/shader/lightning/DirectionalLightShader.cpp \
					render/shader/lightning/DirectionalLightNormalMapShader.cpp \
					render/shader/lightning/DirectionalLightNormalMapShadowMapShader.cpp \
					render/shader/lightning/DirectionalLightShadowMapShader.cpp \
					render/shader/lightning/DirectionalLightTexturedShadowMapShader.cpp \
					render/shader/lightning/EdgelShader.cpp \
					render/shader/lightning/SphereShader.cpp \
					render/shader/shadow/DepthMapShader.cpp \
					render/shader/basic/ColorShader.cpp \
					render/shader/basic/TextureShader.cpp \
					render/shader/gui/SimplePlaneShader.cpp \
					render/shader/gui/SimplePlaneDepthShader.cpp \
					render/shader/gui/AxisShader.cpp \
					render/utils/Timer.cpp \
					render/scene/Node.cpp \
					render/scene/BasicSceneManager.cpp \
					render/globalData/GlobalData.cpp \
					render/GUI/Text.cpp \
					render/GUI/TextManager.cpp \
					render/GUI/Rect.cpp \
					render/GUI/GUIUtils/SliderGUI.cpp \
					render/scene/RTT/BasicRTT.cpp \
					render/scene/RTT/DepthRTT.cpp \
					render/scene/RTT/PickRTT.cpp \
					render/texture/Texture.cpp \
					render/texture/TextureManager.cpp \
					render/sceneObjects/mesh/SubMesh.cpp \
					render/sceneObjects/mesh/Mesh.cpp \
					render/sceneObjects/mesh/MeshManager.cpp \
					render/sceneObjects/camera/Camera.cpp \
					render/sceneObjects/light/Light.cpp \
					render/inputSystem/Input.cpp \
					render/modules/audio/AudioPlayer.cpp \
					logic/SceneSystem/Scenes/IntroductionScene.cpp \
					logic/SceneSystem/Scenes/SunScene.cpp \
					logic/SceneSystem/Scenes/AtomScene.cpp \
					logic/SceneSystem/Scenes/ShaderScene.cpp \
					logic/SceneSystem/Scenes/SplashScreenScene.cpp \
					logic/SceneSystem/SceneStateMachine.cpp

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lz -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue libpng libjpeg libfreetype

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
