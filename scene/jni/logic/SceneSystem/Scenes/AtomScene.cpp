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

#include "AtomScene.h"
#include "../../../render/GUI/Rect.h"
#include "../../../render/GUI/TextManager.h"
#include "../../../render/shader/lightning/SphereShader.h"
#include "../../../render/shader/lightning/DirectionalLightShader.h"
#include "../../../render/inputSystem/Input.h"
#include "ShaderScene.h"
#include "SunScene.h"
#include "IntroductionScene.h"
#include "utils/StaticGUI.h"

AtomScene::AtomScene(){
    stateName = "AtomScene";

    GlobalData::getInstance()->scene->closeScene();
    TextureManager::freeInstance();
    MeshManager::freeInstance();
    TextManager::freeInstance();

    StaticGUI::initGUIButtons();
    GlobalData::getInstance()->scene->setColorClean(glm::vec3(1.0f,1.0f,1.0f));

    logInf("AtomScene scene constructor!");
	Mesh* auxMesh = 0;

	// Creating the core
	coreNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
	auxMesh = GlobalData::getInstance()->scene->createMesh("sphere.obj");
	coreNode->attachSceneObject(auxMesh);
	coreNode->setScale(glm::vec3(2.0f, 2.0f, 2.0f));

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

		eAuxNode = coreAuxNode->createChild();
		eAuxMesh = GlobalData::getInstance()->scene->createMesh("sphere.obj");
		eAuxNode->attachSceneObject(eAuxMesh);
		eAuxNode->setScale(glm::vec3(0.2f, 0.2f, 0.2f));
		eAuxNode->setPosition(-ePositions[i]);
		eNodes.push_back(eAuxNode);

		coreAuxNode->rotate(glm::vec3(0.0f, 0.0f, 1.0f), Maths::PI/4.0f * (float)i);
		eCenterNodes.push_back(coreAuxNode);
	}

	Node* cameraNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
	Camera* camera = GlobalData::getInstance()->scene->createCamera();
	cameraNode->attachSceneObject(camera);
	camera->setAspectRatio((float)GlobalData::getInstance()->screenWidth/(float)GlobalData::getInstance()->screenHeight);
	cameraNode->setPosition(glm::vec3(-14.0f, 10.0f,8.0f));
	cameraNode->lookAt(glm::vec3(0.0f,0.0f,0.0f));

	//second camera disabled, to create shadow map
	cameraParentNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
	cameraNode = cameraParentNode->createChild();
	camera = GlobalData::getInstance()->scene->createCamera();
	cameraNode->attachSceneObject(camera);
	cameraNode->attachSceneObject(GlobalData::getInstance()->scene->createLight()); // TODO: not working. Ignored
	camera->setAspectRatio((float)GlobalData::getInstance()->screenWidth/(float)GlobalData::getInstance()->screenHeight);
	cameraNode->setPosition(glm::vec3(-17.0f, 0.0f,0.0f));
	camera->setEnabled(false);
	cameraNode->lookAt(glm::vec3(0.0f,0.0f,0.0f));
}

AtomScene::~AtomScene(){

}

int AtomScene::update(SceneStateMachine *machine){
	float eAngle = Timer::getInstance()->getDeltaTime() * 0.1f * Maths::PI/180.0f;
	float cameraAngle = Timer::getInstance()->getDeltaTime() * 0.05f * Maths::PI/180.0f;

	cameraParentNode->rotate(glm::vec3(0.0f, 0.0f, 1.0f), cameraAngle);

	for (int i = 0; i < eCenterNodes.size(); i++) {
		eCenterNodes[i]->rotate(glm::vec3(0.0f, 1.0f, 0.0f), eAngle);
		eNodes[i]->rotate(glm::vec3(0.0f, 1.0f, 0.0f), -eAngle);
	}

    std::vector<event> events = Input::getInstance()->getEventsNotLooked();
    if(events.size() == 0) return NOTHING;

    RectGUI* rect = GlobalData::getInstance()->scene->getRectTouch(events.at(0).x, events.at(0).y);
	if(rect == StaticGUI::nextButton){
        Input::getInstance()->clearEvents();
        machine->currentState = new ShaderScene();
        delete this;
    }else if(rect == StaticGUI::previousButton){
    	Input::getInstance()->clearEvents();
		machine->currentState = new IntroductionScene();
		delete this;
    }else if(rect == StaticGUI::homeButton){
    	Input::getInstance()->clearEvents();
    	machine->currentState = new IntroductionScene();
    	delete this;
    }

    return NOTHING;
}
