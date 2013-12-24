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
#include "render/modules/audio/AudioPlayer.h"
#include "render/shader/lightning/DirectionalLightShadowMapShader.h"
#include "render/shader/lightning/DirectionalLightTexturedShadowMapShader.h"

Node* cameraParentNode = 0;
Node* cameraNode = 0;
Camera* mainCamera = 0;
Camera* altCamera = 0;

RectGUI* rectShader = 0;
RectGUI* rectCamera = 0;
RectGUI* rectLight = 0;

bool changeShader = false;
bool changeCamera = false;
bool rotateLight = false;

Node* coreNode = 0;
Mesh* coreMesh = 0;
Shader* shaderA = 0;
Shader* shaderB = 0;

std::vector<Node*> eNodes;
std::vector<Mesh*> eMeshes;
std::vector<Node*> eCenterNodes;

static void init_resources() {
	logInf("creating new instance of scene manager");
	GlobalData::getInstance()->scene = new BasicSceneManager();

    // Creating the core
    coreNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
    coreMesh = GlobalData::getInstance()->scene->createMesh("sphere.obj");
    coreNode->attachSceneObject(coreMesh);
    coreNode->setScale(glm::vec3(2.0f, 2.0f, 2.0f));

    // Saving the original shader (DirectionalLightShadowMapShader)
    shaderA = coreMesh->shader;

    Texture* texture = TextureManager::getInstance()->getTexture("BoxTexture.png");
    coreMesh->subMeshes[0]->textureSubMesh = texture;

    // Creating another shader (DirectionalLightShadowMapShadowMapShader)
    shaderB = new DirectionalLightTexturedShadowMapShader();

    glm::vec3 ePositions[4] = {
        glm::vec3(-4.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -4.0f),
        glm::vec3(+4.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, +4.0f)
    };

    // Creating the electrons
    for (int i = 0; i < 4; i++) {
        Node* coreAuxNode = 0;
        Node* eAuxNode = 0;
        Mesh* eAuxMesh = 0;

        coreAuxNode = GlobalData::getInstance()->scene->getRootNode()->createChild();

        eAuxNode = coreAuxNode->createChild();
        eAuxMesh = GlobalData::getInstance()->scene->createMesh("sphere.obj");
        eAuxNode->attachSceneObject(eAuxMesh);
        eAuxNode->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
        eAuxNode->setPosition(ePositions[i]);
        eNodes.push_back(eAuxNode);
        eMeshes.push_back(eAuxMesh);

        eAuxNode = coreAuxNode->createChild();
        eAuxMesh = GlobalData::getInstance()->scene->createMesh("sphere.obj");
        eAuxNode->attachSceneObject(eAuxMesh);
        eAuxNode->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
        eAuxNode->setPosition(-ePositions[i]);
        eNodes.push_back(eAuxNode);
        eMeshes.push_back(eAuxMesh);

        coreAuxNode->rotate(glm::vec3(0.0f, 0.0f, 1.0f), Maths::PI/4.0f * (float)i);
        eCenterNodes.push_back(coreAuxNode);
    }

    Node* cameraNode = 0;

    // Second camera disabled, to create shadow map
	Camera* camera = 0;
    cameraParentNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
	cameraNode = cameraParentNode->createChild();
	camera = GlobalData::getInstance()->scene->createCamera();
	cameraNode->attachSceneObject(camera);
    cameraNode->attachSceneObject(GlobalData::getInstance()->scene->createLight()); // TODO: not working. Ignored
	camera->setAspectRatio((float)GlobalData::getInstance()->screenWidth/(float)GlobalData::getInstance()->screenHeight);
	cameraNode->setPosition(glm::vec3(-17.0f, 0.0f,0.0f));
    camera->setFarClipDistance(30.0f);
	camera->setEnabled(false);
	cameraNode->lookAt(glm::vec3(0.0f,0.0f,0.0f));

	cameraNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
	mainCamera = GlobalData::getInstance()->scene->createCamera();
	cameraNode->attachSceneObject(mainCamera);
	mainCamera->setAspectRatio((float)GlobalData::getInstance()->screenWidth/(float)GlobalData::getInstance()->screenHeight);
	cameraNode->setPosition(glm::vec3(-14.0f, 10.0f,8.0f));
	cameraNode->lookAt(glm::vec3(0.0f,0.0f,0.0f));

    // Alternative camera
    altCamera = GlobalData::getInstance()->scene->createCamera();
    eNodes[0]->attachSceneObject(altCamera);
	altCamera->setAspectRatio((float)GlobalData::getInstance()->screenWidth/(float)GlobalData::getInstance()->screenHeight);
    altCamera->setEnabled(false);
    altCamera->setNearClipDistance(0.1);

    // GUI
    float aspectRatio = (float)GlobalData::getInstance()->screenHeight/(float)GlobalData::getInstance()->screenWidth;
    RectGUI* rect = 0;

    rect = GlobalData::getInstance()->scene->createRectangleGUI(0.0f, 1.0f, 0.25f, 0.1);
    rect->setTexture(TextureManager::getInstance()->getTexture("blueSquare.png"));
    rect->setText("Shader", "FreeSans.ttf", 48);
    rect->setClickable(true);
    rectShader = rect;

    rect = GlobalData::getInstance()->scene->createRectangleGUI(0.0f, 1.0f - 0.1f, 0.25f, 0.1f);
    rect->setTexture(TextureManager::getInstance()->getTexture("blueSquare.png"));
    rect->setText("Camera", "FreeSans.ttf", 48);
    rect->setClickable(true);
    rectCamera = rect;

    rect = GlobalData::getInstance()->scene->createRectangleGUI(0.0f, 1.0f - 2.0f*0.1f, 0.25f, 0.1f);
    rect->setTexture(TextureManager::getInstance()->getTexture("blueSquare.png"));
    rect->setText("Light", "FreeSans.ttf", 48);
    rect->setClickable(true);
    rectLight = rect;
}

static void free_resources(){
	if(GlobalData::getInstance()->scene != 0){
		delete GlobalData::getInstance()->scene;
	}
	GlobalData::getInstance()->scene = 0;
	TextureManager::freeInstance();
	MeshManager::freeInstance();
	TextManager::freeInstance();
	ContextControllerEGL::getInstance()->endDisplay();
	FileSystem::freeInstance();
}

static void engine_draw_frame(){
	if(ContextControllerEGL::getInstance()->display == NULL){
		return;
	}
	Timer::getInstance()->calculeCurrentTime();

	float eAngle = Timer::getInstance()->getDeltaTime() * 0.1f * Maths::PI/180.0f;
    float cameraAngle = Timer::getInstance()->getDeltaTime() * 0.05f * Maths::PI/180.0f;

    // Process events. See onTouchEvent function.
    std::vector<event> events = Input::getInstance()->getEventsNotLooked();
    if(events.size() != 0) {
        RectGUI* rect = GlobalData::getInstance()->scene->getRectTouch(events.at(0).x/(float)GlobalData::getInstance()->screenWidth,
                                                                       1.0f - events.at(0).y/(float)GlobalData::getInstance()->screenHeight);
        if (rect) {
            if (rect == rectLight) {
                logInf("Light click!");
                rotateLight = !rotateLight;
            } else if (rect == rectCamera) {
                changeCamera = !changeCamera;
                logInf("Camera click!");
            } else if (rect == rectShader) {
                changeShader = !changeShader;
                logInf("Shader click!");
            }
            Input::getInstance()->clearEvents();
        }
    }

    if (rotateLight) {
        cameraParentNode->rotate(glm::vec3(0.0f, 0.0f, 1.0f), cameraAngle);
    }

    if (changeCamera) {
        mainCamera->setEnabled(!mainCamera->getEnabled());
        altCamera->setEnabled(!altCamera->getEnabled());
        changeCamera = false;
    }

    coreMesh->shader = changeShader ? shaderA : shaderB;
    for (int i = 0; i < eMeshes.size(); i++) {
        eMeshes[i]->shader = changeShader ? shaderA : shaderB;
    }

    for (int i = 0; i < eCenterNodes.size(); i++) {
        eCenterNodes[i]->rotate(glm::vec3(0.0f, 1.0f, 0.0f), eAngle);
        eNodes[i]->rotate(glm::vec3(0.0f, 1.0f, 0.0f), -eAngle);
    }

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
				logInf("newEvent!");
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
