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

#include "SunScene.h"
#include "../../../render/GUI/Rect.h"
#include "../../../render/GUI/TextManager.h"
#include "../../../render/shader/lightning/SphereShader.h"
#include "../../../render/shader/lightning/DirectionalLightShader.h"
#include "../../../render/inputSystem/Input.h"
#include "IntroductionScene.h"
#include "ShaderScene.h"
#include "SunScene.h"
#include "utils/StaticGUI.h"

SunScene::SunScene(){
    stateName = "SunScene";
    GlobalData::getInstance()->scene->closeScene();
    TextureManager::freeInstance();
    MeshManager::freeInstance();
    TextManager::freeInstance();

    StaticGUI::initGUIButtons();
    GlobalData::getInstance()->scene->setColorClean(glm::vec3(0.0f,0.0f,0.0f));

    logInf("SunScene scene constructor!");
    Mesh* auxMesh;
	Node* auxNode;
	Shader* auxShader;

	Camera* camera = GlobalData::getInstance()->scene->createCamera();
	camera->setAspectRatio((float)GlobalData::getInstance()->screenWidth/(float)GlobalData::getInstance()->screenHeight);
	auxNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
	auxNode->attachSceneObject(camera);
	auxNode->setPosition(glm::vec3(5.0f,5.0f,10.0f));
	auxNode->lookAt(glm::vec3(0.0f,0.0f,0.0f));

	auxMesh = GlobalData::getInstance()->scene->createMesh("sphere.obj");
	auxShader = new SphereShader();
	auxMesh->setShader(auxShader);
	auxNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
	auxNode->attachSceneObject(auxMesh);
	auxMesh->options = auxMesh->options | Mesh::ALPHA_BLENDING_MASK;

	auxShader = new DirectionalLightShader();

	glm::vec3 positions[6] = {
			glm::vec3(2.0f, 0.0f, 0.0f),
			glm::vec3(3.0f, 0.0f, 1.0f),
			glm::vec3(4.0f, 0.0f, -1.0f),
			glm::vec3(5.0f, 0.0f, 2.0f),
			glm::vec3(6.0f, 0.0f, -2.0f),
			glm::vec3(7.0f, 0.0f, 3.0f)
	};

	for(int i = 0; i < 6; i++){
		auxMesh = GlobalData::getInstance()->scene->createMesh("sphere.obj");
		auxNode = GlobalData::getInstance()->scene->getRootNode()->createChild();
		nodes.push_back(auxNode);
		auxNode = auxNode->createChild();
		auxNode->setPosition(positions[i]);
		auxNode->setScale(glm::vec3(0.02f,0.02f,0.02f)*(float)(i%4+1));
		auxNode->attachSceneObject(auxMesh);
	}
}

SunScene::~SunScene(){

}

int SunScene::update(SceneStateMachine *machine){
	float angle = Timer::getInstance()->getDeltaTime() * 0.09f * Maths::PI/180.0f;
	for(int i = 0; i < nodes.size(); i++){
		nodes[i]->rotate(glm::vec3(0.0f,1.0f,0.0f), angle * (float)(i%3+1));
	}

	std::vector<event> events = Input::getInstance()->getEventsNotLooked();
	if(events.size() == 0) return NOTHING;

    RectGUI* rect = GlobalData::getInstance()->scene->getRectTouch(events.at(0).x/(float)GlobalData::getInstance()->screenWidth, 1.0f - events.at(0).y/(float)GlobalData::getInstance()->screenHeight);
	if(rect == StaticGUI::nextButton) {
        Input::getInstance()->clearEvents();
        machine->currentState = new IntroductionScene();
        delete this;
    } else if(rect == StaticGUI::previousButton) {
    	Input::getInstance()->clearEvents();
		machine->currentState = new ShaderScene();
		delete this;
    } else if(rect == StaticGUI::homeButton) {
    	Input::getInstance()->clearEvents();
		machine->currentState = new IntroductionScene();
		delete this;
    }
}
