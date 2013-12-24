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

#ifndef SPLASH_SCREEN_SCREEN_STATE_H
#define SPLASH_SCREEN_SCREEN_STATE_H

#include "../SceneState.h"
#include "../SceneStateMachine.h"

class SplashScreenScene : public SceneState {
public:
    SplashScreenScene();
    ~SplashScreenScene();
    virtual int update(SceneStateMachine* machine);
private:
    //only used because the data loading is sequential
    //used to render the background and then block it (without cleaning buffers)
    bool firstExecution;
};

#endif
