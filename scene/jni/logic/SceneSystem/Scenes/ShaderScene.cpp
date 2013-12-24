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

#include "IntroductionScene.h"
#include "ShaderScene.h"
#include "AtomScene.h"
#include "SunScene.h"
#include "utils/StaticGUI.h"
#include "../../../render/GUI/Rect.h"
#include "../../../render/GUI/TextManager.h"
#include "../../../render/shader/basic/TextureShader.h"

ShaderScene::ShaderScene(){

    stateName = "ShaderScene";
    GlobalData::getInstance()->scene->closeScene();
    TextureManager::freeInstance();
    MeshManager::freeInstance();
    TextManager::freeInstance();

    StaticGUI::initGUIButtons();
    GlobalData::getInstance()->scene->setColorClean(glm::vec3(1.0f,1.0f,1.0f));

    changeShader = false;
    changeCamera = false;
    rotateLight = false;

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

	rect = GlobalData::getInstance()->scene->createRectangleGUI(0.75f, 1.0f, 0.25f, 0.1);
	rect->setTexture(TextureManager::getInstance()->getTexture("blueSquare.png"));
	rect->setText("Shader", "FreeSans.ttf", 48);
	rect->setClickable(true);
	rectShader = rect;

	rect = GlobalData::getInstance()->scene->createRectangleGUI(0.75f, 1.0f - 0.1f, 0.25f, 0.1f);
	rect->setTexture(TextureManager::getInstance()->getTexture("blueSquare.png"));
	rect->setText("Camera", "FreeSans.ttf", 48);
	rect->setClickable(true);
	rectCamera = rect;

	rect = GlobalData::getInstance()->scene->createRectangleGUI(0.75f, 1.0f - 2.0f*0.1f, 0.25f, 0.1f);
	rect->setTexture(TextureManager::getInstance()->getTexture("blueSquare.png"));
	rect->setText("Light", "FreeSans.ttf", 48);
	rect->setClickable(true);
	rectLight = rect;
}

ShaderScene::~ShaderScene(){

}

int ShaderScene::update(SceneStateMachine *machine){
	float eAngle = Timer::getInstance()->getDeltaTime() * 0.1f * Maths::PI/180.0f;
	float cameraAngle = Timer::getInstance()->getDeltaTime() * 0.05f * Maths::PI/180.0f;

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

	// Process events
	std::vector<event> events = Input::getInstance()->getEventsNotLooked();
	if(events.size() != 0) {
		RectGUI* rect = GlobalData::getInstance()->scene->getRectTouch(events.at(0).x/(float)GlobalData::getInstance()->screenWidth,
																   1.0f - events.at(0).y/(float)GlobalData::getInstance()->screenHeight);
		if (rect) {
			Input::getInstance()->clearEvents();
			if (rect == rectLight) {
				logInf("Light click!");
				rotateLight = !rotateLight;
			} else if (rect == rectCamera) {
				changeCamera = !changeCamera;
				logInf("Camera click!");
			} else if (rect == rectShader) {
				changeShader = !changeShader;
				logInf("Shader click!");
			} else if(rect == StaticGUI::nextButton){
				machine->currentState = new SunScene();
				delete this;
			}else if(rect == StaticGUI::previousButton){
				machine->currentState = new AtomScene();
				delete this;
			}else if(rect == StaticGUI::homeButton){
				machine->currentState = new IntroductionScene();
				delete this;
			}
		}
	}
	return NOTHING;
}
