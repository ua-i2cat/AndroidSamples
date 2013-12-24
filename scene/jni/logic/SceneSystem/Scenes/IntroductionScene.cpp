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
#include "../../../render/GUI/Rect.h"
#include "../../../render/GUI/TextManager.h"
#include "../../../render/inputSystem/Input.h"
#include "ShaderScene.h"
#include "SunScene.h"
#include "AtomScene.h"
#include "utils/StaticGUI.h"

IntroductionScene::IntroductionScene(){
    stateName = "IntroductionScene";
    GlobalData::getInstance()->scene->closeScene();
    TextureManager::freeInstance();
    MeshManager::freeInstance();
    TextManager::freeInstance();

    RectGUI* rect = GlobalData::getInstance()->scene->createRectangleBackground();
    GlobalData::getInstance()->scene->setColorClean(glm::vec3(1.0f,1.0f,1.0f));
    StaticGUI::initGUIButtons();

    internalTimer = 0;

	Mesh* earthMesh = 0;
	Mesh* moonMesh = 0;
	Node* cameraParentNode = 0;
	Node* cameraNode = 0;

	centerNode = 0;
	earthNode = 0;
	moonNode = 0;

	// Creating the Earth
	earthNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
	earthMesh = GlobalData::getInstance()->scene->createMesh("sphere.obj");
	earthNode->attachSceneObject(earthMesh);
	earthNode->setScale(glm::vec3(2.0f, 2.0f, 2.0f));

	// Creating the Moon
	centerNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
	moonNode = centerNode->createChild();
	moonMesh = GlobalData::getInstance()->scene->createMesh("sphere.obj");
	moonNode->attachSceneObject(moonMesh);
	moonNode->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	moonNode->setPosition(glm::vec3(-4.0f, 0.0f, 0.0f));

	// Adding a textured Earth to the scene
	Node* texturedEarthNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
	Mesh* texturedEarthMesh = GlobalData::getInstance()->scene->createMesh("sphere.obj");
	// Creating the shader
	DirectionalLightShader* shader = new DirectionalLightShader();
	texturedEarthMesh->setShader(shader);
	// Assigning the the texture
	shader->texture = TextureManager::getInstance()->getTexture("blueSquare.png");
	texturedEarthNode->attachSceneObject(texturedEarthMesh);
	texturedEarthNode->setPosition(glm::vec3(0.0f, 4.0f, 0.0f));
	texturedEarthNode->setScale(glm::vec3(2.0f, 2.0f, 2.0f));

	cameraNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
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

IntroductionScene::~IntroductionScene(){

}

int IntroductionScene::update(SceneStateMachine *machine){
    internalTimer += Timer::getInstance()->getDeltaTime();

	float earthAngle = Timer::getInstance()->getDeltaTime() * (-0.02f) * Maths::PI/180.0f;
	float moonAngle = Timer::getInstance()->getDeltaTime() * 0.03f * Maths::PI/180.0f;
	float centerAngle = Timer::getInstance()->getDeltaTime() * 0.02f * Maths::PI/180.0f;

	// Rotate
	earthNode->rotate(glm::vec3(0.0f, 1.0f, 0.0f), earthAngle);
	centerNode->rotate(glm::vec3(0.0f, 1.0f, 0.0f), centerAngle);
	moonNode->rotate(glm::vec3(0.0f, 1.0f, 0.0f), moonAngle);

    std::vector<event> events = Input::getInstance()->getEventsNotLooked();
    if(events.size() == 0) return NOTHING;

    RectGUI* rect = GlobalData::getInstance()->scene->getRectTouch(events.at(0).x/(float)GlobalData::getInstance()->screenWidth, 1.0f - events.at(0).y/(float)GlobalData::getInstance()->screenHeight);
	if(rect == StaticGUI::nextButton) {
        Input::getInstance()->clearEvents();
        machine->currentState = new AtomScene();
        delete this;
    } else if(rect == StaticGUI::previousButton) {
    	Input::getInstance()->clearEvents();
		machine->currentState = new SunScene();
		delete this;
    	logInf("pau poi poi");
    } else if(rect == StaticGUI::homeButton) {
    	Input::getInstance()->clearEvents();
    	logInf("home poi poi");
    }

    return NOTHING;
}
