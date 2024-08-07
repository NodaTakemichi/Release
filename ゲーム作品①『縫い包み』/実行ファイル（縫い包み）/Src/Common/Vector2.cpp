#include "Vector2.h"

// コンストラクタ
Vector2::Vector2(void)
{
	x = 0;
	y = 0;
}

// コンストラクタ
Vector2::Vector2(float vX, float vY)
{
	x = vX;
	y = vY;
}

Vector2::~Vector2(void)
{
}

Vector2& Vector2::operator=(const Vector2& vec)
{
	x = vec.x;
	y = vec.y;

	return *this;
}

Vector2& Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;

	return *this;
}

Vector2& Vector2::operator+=(const float& value)
{
	x += value;
	y += value;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& vec)
{
	x -= vec.x;
	y -= vec.y;

	return *this;
}

Vector2& Vector2::operator-=(const float& value)
{
	x -= value;
	y -= value;
	return *this;
}

Vector2& Vector2::operator*=(const Vector2& vec)
{
	x *= vec.x;
	y *= vec.y;

	return *this;
}

Vector2& Vector2::operator*=(const float& value)
{
	x *= value;
	y *= value;
	return *this;
}

Vector2& Vector2::operator/=(const Vector2& vec)
{
	//ガード処理
	if (vec.x > 0) {
		x /= vec.x;
	}
	else {
		x = 0;
	}

	if (vec.y > 0) {
		y /= vec.y;
	}
	else {
		y = 0;
	}


	return *this;
}

Vector2& Vector2::operator/=(const float& value)
{
	if (value == 0)
	{
		x = 0;
		y = 0;
		return *this;
	}

	x /= value;
	y /= value;
	return *this;
}


Vector2 Vector2::operator+(const Vector2& vec)
{
	return { x + vec.x,y + vec.y };
}

Vector2 Vector2::operator-(const Vector2& vec)
{
	return { x - vec.x,y - vec.y };
}

Vector2 Vector2::operator*(const Vector2& vec)
{
	return { x * vec.y,y * vec.y };
}

bool Vector2::operator==(const Vector2& vec) const
{
	if (x == vec.x && y == vec.y)return true;

	return false;
}

bool Vector2::operator!=(const Vector2& vec) const
{
	if (x == vec.x && y == vec.y)return false;

	return true;
}
