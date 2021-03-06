#include "Brick.h"
#include <assert.h>

Brick::Brick(RectF & rect_in, Color color_in)
	:
	rect(rect_in),
	color(color_in),
	bevel(color_in)
{
}

void Brick::Draw(Graphics & gfx)
{
	if (!destroyed)
	{
		bevel.DrawBeveledBrick(rect.GetExpanded(-padding), bevelSize, gfx);
	}
}

bool Brick::CheckBallCollisions(Ball & ball) const
{
	return !destroyed && rect.IsOverlappingWith(ball.GetRect());
}

void Brick::ExecuteBallCollisions(Ball & ball)
{
	assert(CheckBallCollisions(ball));
	const Vec2 ballPos = ball.GetPosition();
	if (std::signbit(ball.GetVelocity().x) == std::signbit((ballPos - GetCenter()).x))
	{
		ball.ReboundY();
	}
	else if (ballPos.x >= rect.left && ballPos.x <= rect.right)
	{
		ball.ReboundY();
	}
	else
	{
		ball.ReboundX();
	}
	destroyed = true;
}

Vec2 Brick::GetCenter() const
{
	return rect.GetCenter();
}

bool Brick::IsDestroyed() const
{
	return destroyed;
}
