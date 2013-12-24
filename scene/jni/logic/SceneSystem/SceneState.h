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

#ifndef SCENE_STATE_H
#define SCENE_STATE_H

#include <string>
#include "../../render/utils/Timer.h"
#include "../../render/globalData/GlobalData.h"

//include conflict
class SceneStateMachine;

class SceneState {
public:
    enum {END, ERROR, NOTHING};
    SceneState(){
        stateName = "base state name";
        error = "this state does not make any errors...";
    }

    virtual ~SceneState(){ }

    virtual int update(SceneStateMachine* machine) = 0;

    std::string getError(){
        return error;
    }

    std::string getStateName(){
        return stateName;
    }

protected:
    //for sure many states will need a timer
    int internalTimer;
    std::string error;
    std::string stateName;
};

#endif
