#include "RectF.h"

RectF::RectF(float left_in, float right_in, float top_in, float bottom_in)
	:
	left(left_in),
	right(right_in),
	top(top_in),
	bottom(bottom_in)
{
}

RectF::RectF(Vec2& topLeft, Vec2& bottomRight)
	:
	RectF(topLeft.x, bottomRight.x, topLeft.y, bottomRight.y)
{
}

RectF::RectF(Vec2& topLeft, float width, float height)
	:
	RectF(topLeft, topLeft + Vec2(width, height))
{
}

RectF RectF::FromCenter(const Vec2 & center, float halfWidth, float halfHeight)
{
	Vec2 offset = { halfWidth, halfHeight };
	return RectF(center - offset, center + offset);
}

bool RectF::IsOverlappingWith(const RectF& other) const
{
	if (left < other.right && right > other.left && top < other.bottom && bottom > other.top) {
		return true;
	}
	return false;
}

RectF RectF::GetExpanded(float offset) const
{
	return RectF(left - offset, right + offset, top - offset, bottom + offset );
}

Vec2 RectF::GetCenter() const
{
	return Vec2((left + right)/2.0f, (top + bottom)/2.0f);
}

Vec2 RectF::GetTopLeft() const
{
	return Vec2(left, top);
}
