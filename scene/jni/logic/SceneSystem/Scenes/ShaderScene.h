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

#ifndef SHADER_SCREEN_STATE_H
#define SHADER_SCREEN_STATE_H

#include "../SceneState.h"
#include "../SceneStateMachine.h"

#include "../../../render/scene/Node.h"
#include "../../../render/inputSystem/Input.h"
#include <string>

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
#include "render/shader/lightning/DirectionalLightTexturedShadowMapShader.h"
#include "render/shader/shadow/DepthMapShader.h"
#include "render/texture/TextureManager.h"

class ShaderScene : public SceneState {
public:
	ShaderScene();
    ~ShaderScene();
    virtual int update(SceneStateMachine* machine);
private:
    Node* coreNode;
    Mesh* coreMesh;
    Shader* shaderA;
    Shader* shaderB;

    std::vector<Node*> eNodes;
    std::vector<Mesh*> eMeshes;
    std::vector<Node*> eCenterNodes;

    Node* cameraParentNode;
    Node* cameraNode;
    Camera* mainCamera;
    Camera* altCamera;

    RectGUI* rectShader;
    RectGUI* rectCamera;
    RectGUI* rectLight;

    bool changeShader;
    bool changeCamera;
    bool rotateLight;
};

#endif

