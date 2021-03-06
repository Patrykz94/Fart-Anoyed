/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "FrameTimer.h"
#include "Sound.h"
#include "SpriteCodex.h"
#include "Beveler.h"

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"


class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel( float dt );
	/********************************/
	/*  User Functions              */
	/********************************/
	void GenerateBricks();
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	/********************************/
	static constexpr float brickWidth = 45.0f;
	static constexpr float brickHeight = 25.0f;
	static constexpr int horizontalBricks = 12;
	static constexpr int verticalBricks = 5;
	static constexpr int nBricks = horizontalBricks * verticalBricks;
	Vec2 playArea = Vec2(540.0f, 560.0f);
	int playAreaBorder = 10;
	Vec2 topPos = Vec2(0.0f, 100.0f);
	Vec2 screenCenter = Vec2(gfx.ScreenWidth/2, gfx.ScreenHeight/2);
	Vec2 levelTextPos = screenCenter - Vec2(40.0f, -7.0f);
	Vec2 levelNumPos = levelTextPos + Vec2(65.0f, 0.0f);
	FrameTimer ft;
	Beveler frame = Colors::MakeRGB(30, 0, 200);
	Ball ball;
	Brick bricks[nBricks];
	RectF walls;
	Paddle pad;
	Sound soundPad;
	Sound soundBrick;
	bool dead;
	bool gameOver;
	bool gameStarted;
	bool gamePaused;
	bool levelStarted;
	int lives;
	int score;
	int level;
	bool escReleased;
};