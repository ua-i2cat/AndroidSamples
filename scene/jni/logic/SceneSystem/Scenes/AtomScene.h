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

#ifndef ATOM_SCREEN_STATE_H
#define ATOM_SCREEN_STATE_H

#include "../SceneState.h"
#include "../SceneStateMachine.h"
#include "../../../render/scene/Node.h"

class AtomScene : public SceneState {
public:
	AtomScene();
    ~AtomScene();
    virtual int update(SceneStateMachine* machine);
private:
    Node* cameraParentNode;
    Node* cameraNode;
    Node* coreNode;
    std::vector<Node*> eNodes;
    std::vector<Node*> eCenterNodes;
};

#endif
