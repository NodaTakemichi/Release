#pragma once
class Vector2
{
public:

	float x;
	float y;

	Vector2(void);
	Vector2(float vX, float vY);
	~Vector2(void);

	//‘ã“ü
	Vector2& operator=(const Vector2& vec);
	//’P€‰‰Zq
	Vector2& operator+=(const Vector2& vec);
	Vector2& operator+=(const float& value);
	Vector2& operator-=(const Vector2& vec);
	Vector2& operator-=(const float& value);
	Vector2& operator*=(const Vector2& vec);
	Vector2& operator*=(const float& value);
	Vector2& operator/=(const Vector2& vec);
	Vector2& operator/=(const float& value);
	//“ñ€‰‰Zq
	Vector2 operator+(const Vector2& vec);
	Vector2 operator-(const Vector2& vec);
	Vector2 operator*(const Vector2& vec);
	//”äŠr
	bool operator==(const Vector2& vec)const;
	bool operator!=(const Vector2& vec)const;

};

