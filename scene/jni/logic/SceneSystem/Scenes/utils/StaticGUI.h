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

/**
 * This namespace is used to set the static gui functions shared in the most number of states, it is really dummy
 */
#include "../../../../render/globalData/GlobalData.h"
#include "../../../../render/GUI/Rect.h"
#include "../../../../render/texture/TextureManager.h"

namespace StaticGUI {
	static RectGUI* homeButton;
	static RectGUI* nextButton;
	static RectGUI* previousButton;

	static void initGUIButtons(){
		float w = (float)GlobalData::getInstance()->screenWidth;
		float h = (float)GlobalData::getInstance()->screenHeight;

		homeButton = GlobalData::getInstance()->scene->createRectangleGUI(0.0f*w, 1.0f*h, 0.2f*w, 0.2f*h);
		nextButton = GlobalData::getInstance()->scene->createRectangleGUI(0.0f*w, (1.0f - 0.225f)*h, 0.2f*w, 0.2f*h);
		previousButton = GlobalData::getInstance()->scene->createRectangleGUI(0.0f*w, (1.0f - 0.45f)*h, 0.2f*w, 0.2f*h);

		homeButton->setTexture(TextureManager::getInstance()->getTexture("blueSquare.png"));
		nextButton->setTexture(TextureManager::getInstance()->getTexture("blueSquare.png"));
		previousButton->setTexture(TextureManager::getInstance()->getTexture("blueSquare.png"));

		homeButton->setText("Home", "FreeSans.ttf", 48);
		nextButton->setText("Next", "FreeSans.ttf", 48);
		previousButton->setText("Previous", "FreeSans.ttf", 48);

		homeButton->setClickable(true);
		nextButton->setClickable(true);
		previousButton->setClickable(true);
	}

}
