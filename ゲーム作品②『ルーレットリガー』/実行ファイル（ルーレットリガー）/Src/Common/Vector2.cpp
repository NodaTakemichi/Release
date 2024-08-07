#include "Vector2.h"

// �R���X�g���N�^
Vector2::Vector2(void)
{
	x = 0;
	y = 0;
}

// �R���X�g���N�^
Vector2::Vector2(int vX, int vY)
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

Vector2& Vector2::operator+=(const int& value)
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

Vector2& Vector2::operator-=(const int& value)
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

Vector2& Vector2::operator*=(const int& value)
{
	x *= value;
	y *= value;
	return *this;
}

Vector2& Vector2::operator/=(const Vector2& vec)
{
	//�K�[�h����
	if (vec.x) {
		x /= vec.x;
	}
	else{
		x = 0;
	}
	
	if (vec.y) {
		y /= vec.y;
	}
	else{
		y = 0;
	}
	

	return *this;
}

Vector2& Vector2::operator/=(const int& value)
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


