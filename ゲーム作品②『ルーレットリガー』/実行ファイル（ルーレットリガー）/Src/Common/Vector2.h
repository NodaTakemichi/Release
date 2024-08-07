#pragma once
class Vector2
{
public:

	int x;
	int y;

	// �R���X�g���N�^
	Vector2(void);

	// �R���X�g���N�^
	Vector2(int vX, int vY);

	// �f�X�g���N�^
	~Vector2(void);

	//���
	Vector2& operator=(const Vector2& vec);
	//�P�����Z�q
	Vector2& operator+=(const Vector2& vec);
	Vector2& operator+=(const int& value);
	Vector2& operator-=(const Vector2& vec);
	Vector2& operator-=(const int& value);
	Vector2& operator*=(const Vector2& vec);
	Vector2& operator*=(const int& value);
	Vector2& operator/=(const Vector2& vec);
	Vector2& operator/=(const int& value);
	//�񍀉��Z�q
	Vector2 operator+(const Vector2& vec);
	Vector2 operator-(const Vector2& vec);
	Vector2 operator*(const Vector2& vec);
	//��r
	bool operator==(const Vector2& vec)const;
	bool operator!=(const Vector2& vec)const;


};

