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

#include "SplashScreenScene.h"
#include "../../../render/GUI/Rect.h"
#include "../../../render/GUI/TextManager.h"
#include "IntroductionScene.h"

SplashScreenScene::SplashScreenScene(){
    stateName = "SplashScreenScene";
    GlobalData::getInstance()->scene->closeScene();
    TextureManager::freeInstance();
    MeshManager::freeInstance();
    TextManager::freeInstance();
    FileSystem::freeInstance();

	float h = (float)GlobalData::getInstance()->screenHeight;
	float w = (float)GlobalData::getInstance()->screenWidth;

    RectGUI* rect = GlobalData::getInstance()->scene->createRectangleGUI(0.25f*w, (1.0f - 0.25f)*h, 0.25f*w, 0.25f*h);
    rect->setText("Thin Render Samples", "FreeSans.ttf", 48);

    rect = GlobalData::getInstance()->scene->createRectangleBackground();
    GlobalData::getInstance()->scene->setColorClean(glm::vec3(1.0f,1.0f,1.0f));

    firstExecution = true;
    internalTimer = 0;
}

SplashScreenScene::~SplashScreenScene(){

}

int SplashScreenScene::update(SceneStateMachine *machine){
    if(firstExecution){
        firstExecution = false;
        return NOTHING;
    }

    internalTimer += Timer::getInstance()->getDeltaTime();
    if(internalTimer > 2000){
        machine->currentState = new IntroductionScene();
        delete this;
    }
    return NOTHING;
}
