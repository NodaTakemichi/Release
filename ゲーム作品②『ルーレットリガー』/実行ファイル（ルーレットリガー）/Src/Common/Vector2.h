#pragma once
class Vector2
{
public:

	int x;
	int y;

	// コンストラクタ
	Vector2(void);

	// コンストラクタ
	Vector2(int vX, int vY);

	// デストラクタ
	~Vector2(void);

	//代入
	Vector2& operator=(const Vector2& vec);
	//単項演算子
	Vector2& operator+=(const Vector2& vec);
	Vector2& operator+=(const int& value);
	Vector2& operator-=(const Vector2& vec);
	Vector2& operator-=(const int& value);
	Vector2& operator*=(const Vector2& vec);
	Vector2& operator*=(const int& value);
	Vector2& operator/=(const Vector2& vec);
	Vector2& operator/=(const int& value);
	//二項演算子
	Vector2 operator+(const Vector2& vec);
	Vector2 operator-(const Vector2& vec);
	Vector2 operator*(const Vector2& vec);
	//比較
	bool operator==(const Vector2& vec)const;
	bool operator!=(const Vector2& vec)const;


};

