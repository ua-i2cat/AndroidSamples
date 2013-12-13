/*
 *  thin render - Mobile render engine based on OpenGL ES 2.0
 *  Copyright (C) 2013  Fundació i2CAT, Internet i Innovació digital a Catalunya
 *
 *  This file is part of thin render.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author:         Marc Fernandez Vanaclocha <marc.fernandez@i2cat.net>
 */

#include <jni.h>
#include <errno.h>
#include <android_native_app_glue.h>
#include <android/input.h>

#include "render/Platform.h"
#include "render/OpenGLPlatform.h"
#include "render/controllerEGL/ContextControllerEGL.h"
#include "render/log/Log.h"
#include "render/fileSystem/FileSystem.h"
#include "render/fileSystem/AndroidFileSystem.h"
#include "render/globalData/GlobalData.h"
#include "render/shader/Shader.h"
#include "render/utils/Maths.h"
#include "render/utils/Timer.h"
#include "render/scene/BasicSceneManager.h"
#include "render/sceneObjects/mesh/Mesh.h"
#include "render/sceneObjects/mesh/MeshManager.h"
#include "render/sceneObjects/camera/Camera.h"
#include "render/sceneObjects/light/Light.h"
#include "render/inputSystem/Input.h"
#include "render/GUI/TextManager.h"
#include "render/GUI/Rect.h"

static void init_resources() {
	logInf("creating new instance of scene manager");
	GlobalData::getInstance()->scene = new BasicSceneManager();
    // TODO: initialize scene
}

static void free_resources() {
    if(GlobalData::getInstance()->scene != 0)
		delete GlobalData::getInstance()->scene;
	GlobalData::getInstance()->scene = 0;
	TextureManager::freeInstance();
	MeshManager::freeInstance();
	TextManager::freeInstance();
	ContextControllerEGL::getInstance()->endDisplay();
	FileSystem::freeInstance();
}

static void engine_draw_frame() {
	if (ContextControllerEGL::getInstance()->display == NULL) {
		return;
	}
	Timer::getInstance()->calculeCurrentTime();
	
    // TODO: update scene
    
    GlobalData::getInstance()->scene->updateVariables();
	GlobalData::getInstance()->scene->createShadowMap();
    GlobalData::getInstance()->scene->cleanBuffers();
    GlobalData::getInstance()->scene->render();
    GlobalData::getInstance()->scene->renderBackground();
    GlobalData::getInstance()->scene->renderGUI();

    eglSwapBuffers(ContextControllerEGL::getInstance()->display, ContextControllerEGL::getInstance()->surface);
}

int onTouchEvent(AInputEvent* event){
	int h, historySize;
	const float TOUCH_MAX_RANGE = 65.0f; // In pixels.
	int actionType;
	int countPointers = AMotionEvent_getPointerCount(event);

	float xPositionTouch, yPositionTouch;
	int actionIndex;
	int pointerId;
	int idTouch;
	for(int i = 0; i < countPointers; i++){
		pointerId = AMotionEvent_getPointerId(event,i);
		actionType = AMotionEvent_getAction(event);
		actionIndex = (actionType&AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
		actionType = actionType & AMOTION_EVENT_ACTION_MASK;
		if(actionIndex != i)continue;
		switch(actionType){
		case AMOTION_EVENT_ACTION_DOWN:
		case AMOTION_EVENT_ACTION_POINTER_DOWN:
			xPositionTouch = AMotionEvent_getX(event,i);
			yPositionTouch = AMotionEvent_getY(event,i);
			idTouch = AMotionEvent_getPointerId(event,i);
			Input::getInstance()->newEvent(idTouch, xPositionTouch, (float)GlobalData::getInstance()->screenHeight - yPositionTouch);
			break;
		case AMOTION_EVENT_ACTION_MOVE:
			xPositionTouch = AMotionEvent_getX(event,i);
			yPositionTouch = AMotionEvent_getY(event,i);
			idTouch = AMotionEvent_getPointerId(event,i);
			Input::getInstance()->updateEvent(idTouch, xPositionTouch, (float)GlobalData::getInstance()->screenHeight - yPositionTouch);
			break;
		case AMOTION_EVENT_ACTION_UP:
		case AMOTION_EVENT_ACTION_CANCEL:
		case AMOTION_EVENT_ACTION_POINTER_UP:
			xPositionTouch = AMotionEvent_getX(event,i);
			yPositionTouch = AMotionEvent_getY(event,i);
			idTouch = AMotionEvent_getPointerId(event,i);
			Input::getInstance()->updateEventToEnd(idTouch);
			break;
		}
	}
	return true;
}

int processInputEvent(AInputEvent* event) {
	int eventType = AInputEvent_getType(event);
	switch (eventType) {
	case AINPUT_EVENT_TYPE_MOTION:
		switch (AInputEvent_getSource(event)) {
		case AINPUT_SOURCE_TOUCHSCREEN:
			return onTouchEvent(event);
			break;
		}
		break;
	}
	return 0;
}

static int32_t inputHandler(struct android_app* app, AInputEvent* event) {
	return processInputEvent(event);
}

bool running;

static void cmdHandler(struct android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (GlobalData::getInstance()->app->window != NULL) {
            	ContextControllerEGL::getInstance()->startDisplay();
                init_resources();
                engine_draw_frame();
                running = true;
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
        	free_resources();
        	ContextControllerEGL::getInstance()->endDisplay();
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            break;
        case APP_CMD_LOST_FOCUS:
            logInf("APP_CMD_LOST_FOCUS");
            free_resources();
            ContextControllerEGL::getInstance()->endDisplay();
            break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */

void android_main(struct android_app* state) {
	running = false;

	GlobalData::getInstance()->app = state;
	GlobalData::getInstance()->screenMode = GlobalData::VERTICAL_SCREEN;
	((AndroidFileSystem*)(FileSystem::getInstance()))->setAssetManager(state->activity);

    // Make sure glue isn't stripped.
    app_dummy();

    state->onAppCmd = cmdHandler;
    state->onInputEvent = inputHandler;

    // loop waiting for stuff to do.
    int ident;
	int events;
	struct android_poll_source* source;
	bool sensorData = false;
    while (1) {
        // Read all pending events.
        if(running){
			engine_draw_frame();
		}
        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
            // Process this event.
        	if (source != NULL) {
				source->process(state, source);
			}
            // Check if we are exiting.
			if (state->destroyRequested != 0) {
				free_resources();
				ContextControllerEGL::getInstance()->endDisplay();
				running = false;
				return;
			}
        }
    }
}
