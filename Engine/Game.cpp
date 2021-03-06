/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	walls((float(gfx.ScreenWidth) - playArea.x) / 2, float(gfx.ScreenWidth) - (float(gfx.ScreenWidth) - playArea.x) / 2, (float(gfx.ScreenHeight) - playArea.y) / 2, float(gfx.ScreenHeight) - (float(gfx.ScreenHeight) - playArea.y) / 2),
	pad(Vec2(screenCenter.x, gfx.ScreenHeight - 50)),
	ball(Vec2(screenCenter.x + 20, gfx.ScreenHeight - 66), Vec2(0.0f, 0.0f)),
	soundPad(L"Sounds\\arkpad.wav"),
	soundBrick(L"Sounds\\arkbrick.wav"),
	dead(false),
	gameOver(false),
	gameStarted(false),
	gamePaused(false),
	levelStarted(false),
	lives(3),
	score(0),
	level(1),
	escReleased(true)
{
	GenerateBricks();
}

void Game::GenerateBricks()
{
	Color c[5] = { { 140,140,140 },{ 0,0,230 },{ 0,230,0 },{ 230,0,0 },{ 230,230,0 } };

	int i = 0;
	for (int y = 0; y < verticalBricks; y++)
	{
		for (int x = 0; x < horizontalBricks; x++)
		{
			bricks[i] = Brick(RectF(walls.GetTopLeft() + topPos + Vec2(x * brickWidth, y * brickHeight), brickWidth, brickHeight), c[y]);
			i++;
		}
	}
}

void Game::Go()
{
	gfx.BeginFrame();
	float elapsedTime = ft.Mark();
	while (elapsedTime > 0.0f)
	{
		const float dt = std::min(0.0025f, elapsedTime);
		UpdateModel(dt);
		elapsedTime -= dt;
	}
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel(float dt)
{

	if (!gameOver)
	{
		if (dead)
		{
			if (wnd.kbd.KeyIsPressed(VK_RETURN))
			{
				lives--;
				pad.Reset(Vec2(screenCenter.x, gfx.ScreenHeight - 50));
				ball.Reset(pad.GetBallStartPos(), Vec2(0.0f, 0.0f));

				dead = false;
			}
		}
		else if (gameStarted)
		{
			if (gamePaused)
			{
				if (escReleased && wnd.kbd.KeyIsPressed(VK_ESCAPE))
				{
					gamePaused = false;
					escReleased = false;
				}
			}
			else if (!gamePaused)
			{
				if (escReleased && wnd.kbd.KeyIsPressed(VK_ESCAPE))
				{
					gamePaused = true;
					escReleased = false;
				}

				pad.Update(wnd.kbd, dt);
				pad.DoWallCollisions(walls);

				if (levelStarted)
				{
					ball.Update(dt);

					bool collisionHappened = false;
					float curColDistSq;
					int curColIndex;
					bool bricksRemaining = false;
					for (int i = 0; i < nBricks; i++)
					{
						if (bricks[i].CheckBallCollisions(ball))
						{
							const float newColDistSq = (ball.GetPosition() - bricks[i].GetCenter()).GetLengthSq();
							if (collisionHappened) {
								if (newColDistSq < curColDistSq)
								{
									curColDistSq = newColDistSq;
									curColIndex = i;
								}
							}
							else
							{
								curColDistSq = newColDistSq;
								curColIndex = i;
								collisionHappened = true;
							}
						}
						if (!bricks[i].IsDestroyed())
						{
							bricksRemaining = true;
						}
					}

					if (collisionHappened) {
						bricks[curColIndex].ExecuteBallCollisions(ball);
						ball.SpeedUp();
						soundBrick.Play();
						score += 50 * level;
					}

					if (pad.DoBallCollisions(ball))
					{
						soundPad.Play();
					}
					if (ball.DoWallCollisons(walls))
					{
						if (ball.HitBottomWall())
						{
							if (lives > 1)
							{
								dead = true;
								levelStarted = false;
							}
							else
							{
								gameOver = true;
								lives--;
							}
						}
						else
						{
							soundPad.Play();
						}
					}

					if (!bricksRemaining)
					{
						level++;
						levelStarted = false;

						pad.Reset(Vec2(screenCenter.x, gfx.ScreenHeight - 50));
						ball.Reset(pad.GetBallStartPos(), Vec2(0.0f, 0.0f));

						// Code for resetting the blocks
						GenerateBricks();
					}
				}
				else
				{
					ball.StickToPad(pad.GetVelocity());

					if (wnd.kbd.KeyIsPressed(VK_SPACE))
					{
						levelStarted = true;
					}
				}
			}

			if (!wnd.kbd.KeyIsPressed(VK_ESCAPE))
			{
				escReleased = true;
			}
		}
		else
		{
			// Game not started yet. Wait for user input
			if (wnd.kbd.KeyIsPressed(VK_RETURN))
			{
				gameStarted = true;
			}
		}

	}
}

void Game::ComposeFrame()
{

	frame.DrawBevelFrame(walls.GetExpanded(float(playAreaBorder)*2), playAreaBorder, gfx);

	for (int i = 0; i < lives; i++)
	{
		SpriteCodex::DrawHeart(Vec2(walls.GetTopLeft().x - playAreaBorder * 2 - 30.0f, 10.0f + i * 35.0f), gfx);
	}
	
	// drawing the score
	SpriteCodex::DrawTextScore(Vec2(walls.GetTopLeft().x + playArea.x + playAreaBorder * 2 + 5, 10.0f), gfx);
	SpriteCodex::DrawNumber(Vec2(walls.GetTopLeft().x + playArea.x + playAreaBorder * 2 + 5, 30.0f), gfx, score, 6);

	// drawing game objects
	for (Brick b : bricks)
	{
		b.Draw(gfx);
	}
	
	if (gameOver)
	{
		// drawing game over screen
		SpriteCodex::DrawGameOver(screenCenter, gfx);
		SpriteCodex::DrawTextScore(levelTextPos + Vec2(-20.0f, 45.0f), gfx);
		SpriteCodex::DrawNumber(levelNumPos + Vec2(-15.0f, 45.0f), gfx, score);
	}
	else
	{
		if (dead)
		{
			// drawing dead screen
			SpriteCodex::DrawDead(screenCenter, gfx);
		}
		else if (gameStarted)
		{
			if (!levelStarted)
			{
				// drawing next level screen
				SpriteCodex::DrawTextLevel(levelTextPos, gfx);
				SpriteCodex::DrawNumber(levelNumPos, gfx, level);
			}
		}
		else
		{
			// drawing title screen
			SpriteCodex::DrawTitle(screenCenter, gfx);
		}

		if (gameStarted)
		{
			// drawing ball and pad
			pad.Draw(gfx);
			ball.Draw(gfx);

			if (gamePaused)
			{
				// drawing paused message
				SpriteCodex::DrawTextPaused(screenCenter, gfx);
			}
		}
	}

}
