#pragma once

#include <math.h>
#include "Math.h"
namespace RasterizerBasedSoftwareRendering {

	class Matrix;
	class Vector3;
	/// <summary>
	/// 2次元ベクトルクラス。
	/// </summary>
	class Vector2 {
	public:
		static const Vector2 Zero;
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		explicit Vector2()
		{
			x = 0.0f;
			y = 0.0f;
		}
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		Vector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		Vector2(const Vector3& v);
		/// <summary>
		/// 代入演算子
		/// </summary>
		/// <param name="_v"></param>
		/// <returns></returns>
		Vector2& operator=(const Vector2& _v)
		{
            x = _v.x;
            y = _v.y;
			return *this;
		}
		void Set(float _x, float _y)
		{
			x = _x;
			y = _y;
		}
		union {
#ifdef USE_DIRECTX_MATH
			DirectX::XMFLOAT2 vec;
#endif
			struct { float x, y; };
			float v[2];
		};

		/// <summary>
		/// 線形補完。
		/// </summary>
		/// <param name="t">補完率</param>
		/// <param name="v0">補完開始のベクトル。</param>
		/// <param name="v1">補完終了のベクトル。</param>
		void Lerp(float t, const Vector2& v0, const Vector2& v1)
		{
			x = v0.x + (v1.x - v0.x) * t;
			y = v0.y + (v1.y - v0.y) * t;
		}
		float Length()
		{
			return static_cast<float>(sqrt(x * x + y * y));
		}
		void Normalize()
		{
			float len = Length();
			if (len < 0.0001f) {
				return;
			}
			x /= len;
			y /= len;
		}
		static inline float Dot(const Vector2& v0, const Vector2& v1)
		{
			return v0.x * v1.x + v0.y * v1.y;
		}

		static inline float Cross(const Vector2& v0, const Vector2& v1)
		{
			return v0.x * v1.y - v0.y * v1.x;
		}
	};
	/// <summary>
	/// 3次元ベクトル。
	/// </summary>
	class Vector3{
	public:
		union{
#ifdef USE_DIRECTX_MATH
			DirectX::XMFLOAT3 vec;
#endif
			float v[3];
			struct { float x, y, z; };
		};
		static const Vector3 Zero;
		static const Vector3 Right;
		static const Vector3 Left;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Front;
		static const Vector3 Back;
		static const Vector3 AxisX;
		static const Vector3 AxisY;
		static const Vector3 AxisZ;
		static const Vector3 One;
	public:
#ifdef USE_DIRECTX_MATH
		/// <summary>
		/// XMVECTORへの暗黙の変換。
		/// </summary>
		/// <returns></returns>
		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat3(&vec);
		}
#endif
		/// <summary>
		/// 代入演算子。
		/// </summary>
		/// <param name="_v"></param>
		/// <returns></returns>
		Vector3& operator=(const Vector3& _v)
		{
			memcpy(v, _v.v, sizeof(v));
			return *this;
		}
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <remarks>
		/// x,y,z全ての要素0で初期化されます。
		/// </remarks>
		explicit Vector3()
		{
			x = y = z = 0.0f;
		}
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		Vector3(float x, float y, float z)
		{
			Set(x, y, z);
		}
		/// <summary>
		/// 線形補完
		/// </summary>
		/// <remarks>
		/// this = v0 + (v1-v0) * t;
		/// </remarks>
		/// <param name="t">補完率。</param>
		/// <param name="v0">補完開始のベクトル。</param>
		/// <param name="v1">補完終了のベクトル。</param>
		void Lerp(float t, const Vector3& v0, const Vector3& v1)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR _v = DirectX::XMVectorLerp(
				DirectX::XMLoadFloat3(&v0.vec),
				DirectX::XMLoadFloat3(&v1.vec),
				t);
			DirectX::XMStoreFloat3(&vec, _v);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// ベクトルのコピー。
		/// </summary>
		template<class TVector>
		void CopyTo(TVector& dst) const
		{
			dst.x = x;
			dst.y = y;
			dst.z = z;
		}
		/// <summary>
		/// ベクトルの設定。
		/// </summary>
		/// <param name="_x"></param>
		/// <param name="_y"></param>
		/// <param name="_z"></param>
		void Set(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}
		template<class TVector>
		void Set(TVector& _v)
		{
			Set(_v.x, _v.y, _v.z);
		}
		
		/// <summary>
		/// ベクトルの加算。
		/// </summary>
		/// <remarks>
		/// this += _v;
		/// </remarks>
		/// <param name="_v">加算されるベクトル。</param>
		void Add( const Vector3& _v) 
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// ベクトルの加算。
		/// </summary>
		/// <remarks>
		/// this = v0 + v1;
		/// </remarks>
		void Add( const Vector3& v0, const Vector3& v1 )
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&v0.vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// ベクトルの減算。
		/// </summary>
		/// <remarks>
		/// this -= _v;
		/// </remarks>
		void Subtract( const Vector3& _v )
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// ベクトルの減算。
		/// </summary>
		/// <remarks>
		/// this = v0 - v1;
		/// </remarks>
		void Subtract( const Vector3& v0, const Vector3& v1 )
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&v0.vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
#else
			x = v0.x - v1.x;
			y = v0.y - v1.y;
			z = v0.z - v1.z;
#endif
		}

		/// <summary>
		/// 内積を計算。
		/// </summary>
		/// <remarks>
		/// float d = this->x * _v.x + this->y * _v.y + this->z * _v.z;
		/// return d;
		/// </remarks>
		float Dot( const Vector3& _v ) const
		{
			float d = this->x * _v.x + this->y * _v.y + this->z * _v.z;
			return d;
		}

		/// <summary>
		/// 外積。
		/// </summary>
		/// <remarks>
		/// Vector3 v ;
		/// v.x = this->y * _v.z - this->z * _v.y;
		/// v.y = thiz->z * _v.x - this->x * _v.z;
		/// v.z = this->x * _v.y - this->y * _v.x;
		/// this = v;
		/// </remarks>
		void Cross(const Vector3& _v)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&_v.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVector3Cross(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// 外積。
		/// </summary>
		/// <remarks>
		/// this->x = v0.y * v1.z - v0.z * v1.y;
		/// this->y = v0.z * v1.x - v0.x * v1.z;
		/// this->z = v0.x * v1.y - v0.y * v1.x; 
		/// </remarks>
		void Cross(const Vector3& v0, const Vector3& v1)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&v0.vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVector3Cross(xmv0, xmv1);
			DirectX::XMStoreFloat3(&vec, xmvr);
#else
			x = v0.y * v1.z - v0.z * v1.y;
			y = v0.z * v1.x - v0.x * v1.z;
			z = v0.x * v1.y - v0.y * v1.x;
#endif
		}
		/// <summary>
		/// ベクトルの長さを取得
		/// </summary>
		float Length() const
		{
#if defined(USE_DIRECTX_MATH) && defined(USE_DIRECTX_MATH)
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
			return DirectX::XMVector3Length(xmv).m128_f32[0];
#else
            return sqrt(x*x + y*y + z*z);
#endif
		}
		/// <summary>
		/// ベクトルの長さの二乗を取得。
		/// </summary>
		float LengthSq() const
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
			return DirectX::XMVector3LengthSq(xmv).m128_f32[0];
#else
			assert(false);
            return 0.0f;
#endif
		}

		/// <summary>
		/// ベクトルをスカラーで拡大。
		/// </summary>
		/// <remarks>
		/// 下記のような処理が行われています。
		/// this->x *= s;
		/// this->y *= s;
		/// this->z *= s;
		/// </remarks>
		void Scale(float s)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
			xmv = DirectX::XMVectorScale(xmv, s);
			DirectX::XMStoreFloat3(&vec, xmv);
#else
			x *= s;
			y *= s;
			z *= s;
#endif
		}

		/// <summary>
		/// ベクトルを正規化。
		/// </summary>
		/// <remarks>
		///		下記のような処理が行われています。
		///		//ベクトルの大きさを求める。
		///		float len = this->x * this->x + this->y * this->y + this->z * this->z;
		///		len = sqrt(len);
		///		//ベクトルの大きさで、各要素を除算する。
		///		this->x /= len;
		///		this->y /= len;
		///		this->z /= len;
		/// </remarks>
		void Normalize()
		{
#if defined(USE_DIRECTX_MATH) && defined(USE_DIRECTX_MATH)
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat3(&vec);
			xmv = DirectX::XMVector3Normalize(xmv);
			DirectX::XMStoreFloat3(&vec, xmv);
#else
			Div(Length());
#endif
		}
		/// <summary>
		/// ベクトルをスカラーで除算。
		/// </summary>
		/// <remarks>
		///		下記のような処理が行われています。
		///		this->x /= d;
		///		this->y /= d;
		///		this->z /= d;
		/// </remarks>
		void Div(float d)
		{
			float scale = 1.0f / d;
			Scale(scale);
		}

		/// <summary>
		/// 最大値を設定。
		/// </summary>
		/// <remarks>
		/// 下記のような処理が行われています。
		/// this->x = max( this->x, v.x );
		/// this->y = max( this->y, v.y );
		/// this->z = max( this->z, v.z );
		/// </remarks>
		void Max(const Vector3& v)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v.vec);
			DirectX::XMStoreFloat3(&vec,  DirectX::XMVectorMax(xmv0, xmv1));
#else
			assert(false);
#endif
		}

		/// <summary>
		/// 最小値を設定。
		/// </summary>
		/// <remarks>
		/// 下記のような処理が行われています。
		/// this->x = min( this->x, v.x );
		/// this->y = min( this->y, v.y );
		/// this->z = min( this->z, v.z );
		/// </remarks>
		void Min(const Vector3& v)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v.vec);
			DirectX::XMStoreFloat3(&vec, DirectX::XMVectorMin(xmv0, xmv1));
#else
			assert(false);
#endif
		}

		/// <summary>
		/// 加算代入演算子。
		/// </summary>
		/// <remarks>
		///	ベクトルクラスに下記のような加算代入の機能を提供します。
		///	Vector3 v0 = {20.0f, 30.0f, 40.0f};
		///	Vector3 v1 = {10.0f, 20.0f, 30.0f};
		///	v0 += v1;
		/// </remarks>
		const Vector3& operator+=(const Vector3& _v)
		{
			Add(_v);
			return *this;
		}
		/// <summary>
		/// 乗算代入演算子。
		/// </summary>
		/// <remarks>
		///	ベクトルクラスに下記のような乗算代入演算の機能を提供します。
		/// Vector3 v = {20.0f, 30.0f, 40.0f};
		///	v *= 10;
		/// </remarks>
		const Vector3& operator*=(float s) 
		{
			Scale(s);
			return *this;
		}
		/// <summary>
		/// 減算代入演算子。
		/// </summary>
		/// <remarks>
		/// ベクトルクラスに下記のような減算代入演算の機能を提供します。
		/// Vector3 v0 = {20.0f, 30.0f, 40.0f};
		///	Vector3 v1 = {10.0f, 20.0f, 30.0f};
		///	v0 -= v1;
		/// </remarks>
		const Vector3& operator-=(const Vector3& _v)
		{
			Subtract(_v);
			return *this;
		}
		/// <summary>
		/// 除算代入演算子。
		/// </summary>
		/// <remarks>
		/// ベクトルクラスに下記のような除算代入演算の機能を提供します。
		/// Vector3 v = {20.0f, 30.0f, 40.0f};
		///	v /= 10;
		/// </remarks>
		const Vector3& operator/=(const float s)
		{
			Div(s);
			return *this;
		}
	};
	/// <summary>
	/// 4次元ベクトルクラス。
	/// </summary>
	class Vector4{
	public:
		union{
#ifdef USE_DIRECTX_MATH
			DirectX::XMFLOAT4 vec;
#endif
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
			float v[4];
		};
		static const Vector4 White;
		static const Vector4 Black;
		static const Vector4 Yellow;
		static const Vector4 Gray;
	public:
#ifdef USE_DIRECTX_MATH
		/// <summary>
		/// XMVECTORへの暗黙の型変換。
		/// </summary>
		/// <returns></returns>
		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat4(&vec);
		}
#endif
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <remarks>
		/// x,y,zが0.0、wは1.0で初期化されます。
		/// </remarks>
		explicit Vector4()
		{
			x = y = z = 0.0f;
			w = 1.0f;
		}
		
		/// <summary>
		/// ベクトルのコピー。
		/// </summary>
		template<class TVector4>
		void CopyTo(TVector4& dst) const
		{
			dst.x = x;
			dst.y = y;
			dst.z = z;
			dst.w = w;
		}
	
		/// <summary>
		/// 代入演算子。
		/// </summary>
		/// <remarks>
		/// ベクトルクラスに下記のような代入演算の機能を提供します。
		/// Vector4 v0 = {10.0f, 20.0f, 30.0f, 10.0f};
		/// Vector4 v1;
		/// v1 = v0;
		/// </remarks>
		Vector4& operator=(const Vector4& _v)
		{
			memcpy(v, _v.v, sizeof(v));
			return *this;
		}

		/// <summary>
		/// コンストラクタ。
		/// </summary>
		Vector4(float x, float y, float z, float w)
		{
			Set(x, y, z, w);
		}
		/// <summary>
		/// 3次元のベクトルデータを引数で受け取るコンストラクタ
		/// </summary>
		/// <remarks>
		/// wには1.0が格納されます。
		/// </remarks>
		Vector4(const Vector3& v)
		{
			Set(v);
		}
		Vector4(const ColorR8G8B8A8& color) 
		{
			r = static_cast<float>(color.r);
			g = static_cast<float>(color.g);
			b = static_cast<float>(color.b);
		}
		/// <summary>
		/// ベクトルの各要素を設定。
		/// </summary>
		void Set(float _x, float _y, float _z, float _w)
		{
			this->x = _x;
			this->y = _y;
			this->z = _z;
			this->w = _w;
		}
		/// <summary>
		/// ベクトルを正規化。
		/// </summary>
		void Normalize()
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
			xmv = DirectX::XMVector4Normalize(xmv);
			DirectX::XMStoreFloat4(&vec, xmv);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// ベクトルを設定。
		/// </summary>
		/// <param name="_v"></param>
		void Set(const Vector4& _v)
		{
			*this = _v;
		}
		/// <summary>
		/// 3次元ベクトルデータをもとに、ベクトルを設定。
		/// </summary>
		/// <remarks>
		/// wには1.0が設定されます。
		/// </remarks>
		void Set(const Vector3& _v)
		{
			this->x = _v.x;
			this->y = _v.y;
			this->z = _v.z;
			this->w = 1.0f;
		}
		/// <summary>
		/// ベクトルを加算。
		/// </summary>
		/// <remarks>
		/// this += _v;
		/// </remarks>
		void Add( const Vector4& _v )
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
			DirectX::XMStoreFloat4(&vec, xmvr);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// ベクトルの加算。
		/// </summary>
		/// <remarks>
		/// this = v0 + v1;
		/// </remarks>
		void Add( const Vector4& v0, const Vector4& v1 )
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&v0.vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v1.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
			DirectX::XMStoreFloat4(&vec, xmvr);
#else
            x = v0.x + v1.x;
            y = v0.y + v1.y;
            z = v0.z + v1.z;
            w = v0.w + v1.w;
#endif
		}
		/// <summary>
		/// ベクトルの減算。
		/// </summary>
		/// <remarks>
		/// this -= _v;
		/// </remarks>
		void Subtract( const Vector4& _v )
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
			DirectX::XMStoreFloat4(&vec, xmvr);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// ベクトルの減算。
		/// </summary>
		/// <remarks>
		/// this = v0 - v1;
		/// </remarks>
		void Subtract( const Vector4& v0, const Vector4& v1 )
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&v0.vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v1.vec);
			DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
			DirectX::XMStoreFloat4(&vec, xmvr);
#else
			assert(false);
#endif
		}

		/// <summary>
		/// 内積を計算。
		/// </summary>
		/// <remarks>
		/// float d = this->x * _v.x + this->y * _v.y + this->z * _v.z + this->w * _v.w;
		/// return d;
		/// </remarks>
		float Dot( const Vector4& _v ) const
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
			return DirectX::XMVector4Dot(xmv0, xmv1).m128_f32[0];
#else
			assert(false);
            return 0.0f;
#endif
		}
		/// <summary>
		/// ベクトルの長さを取得
		/// </summary>
		float Length()
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
			return DirectX::XMVector4Length(xmv).m128_f32[0];
#else
			assert(false);
            return 0.0f;
#endif
		}
		/// <summary>
		/// ベクトルの長さの二乗を取得。
		/// </summary>
		float LengthSq()
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
			return DirectX::XMVector4LengthSq(xmv).m128_f32[0];
#else
			assert(false);
            return 0.0f;
#endif
		}
		/// <summary>
		/// ベクトルをスカラーで拡大。
		/// </summary>
		/// <remarks>
		/// 下記のような処理が行われています。
		/// this->x *= s;
		/// this->y *= s;
		/// this->z *= s;
		/// this->w *= s;
		/// </remarks>
		void Scale(float s)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
			xmv = DirectX::XMVectorScale(xmv, s);
			DirectX::XMStoreFloat4(&vec, xmv);
#else
            x *= s;
            y *= s;
            z *= s;
            w *= s;
#endif
		}
		/// <summary>
		/// 最大値を設定。
		/// </summary>
		/// <remarks>
		/// 下記のような処理が行われています。
		/// this->x = max( this->x, v.x );
		/// this->y = max( this->y, v.y );
		/// this->z = max( this->z, v.z );
		/// this->w = max( this->w, v.w );
		/// </remarks>
		void Max(const Vector4& v)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v.vec);
			DirectX::XMStoreFloat4(&vec, DirectX::XMVectorMax(xmv0, xmv1));
#else
			assert(false);
#endif
		}

		/// <summary>
		/// 最小値を設定。
		/// </summary>
		/// <remarks>
		/// 下記のような処理が行われています。
		/// this->x = min( this->x, v.x );
		/// this->y = min( this->y, v.y );
		/// this->z = min( this->z, v.z );
		/// this->w = min( this->w, v.w );
		/// </remarks>
		void Min(const Vector4& v)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
			DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v.vec);
			DirectX::XMStoreFloat4(&vec, DirectX::XMVectorMin(xmv0, xmv1));
#else
			assert(false);
#endif
		}
	};

	const Vector2 g_vec2Zero = { 0.0f, 0.0f };
	const Vector3 g_vec3Zero = { 0.0f,  0.0f,  0.0f };
	const Vector3 g_vec3Right = { 1.0f,  0.0f,  0.0f };
	const Vector3 g_vec3Left = { -1.0f,  0.0f,  0.0f };
	const Vector3 g_vec3Up = { 0.0f,  1.0f,  0.0f };
	const Vector3 g_vec3Down = { 0.0f, -1.0f,  0.0f };
	const Vector3 g_vec3Front = { 0.0f,   0.0f,  1.0f };
	const Vector3 g_vec3Back = { 0.0f,   0.0f, -1.0f };
	const Vector3 g_vec3AxisX = { 1.0f,  0.0f,  0.0f };
	const Vector3 g_vec3AxisY = { 0.0f,  1.0f,  0.0f };
	const Vector3 g_vec3AxisZ = { 0.0f,  0.0f,  1.0f };
	const Vector3 g_vec3One = { 1.0f, 1.0f, 1.0f };

	const Vector4 g_vec4White = { 1.0f, 1.0f, 1.0f, 1.0f };
	const Vector4 g_vec4Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	const Vector4 g_vec4Gray = { 0.5f, 0.5f, 0.5f, 1.0f };
	const Vector4 g_vec4Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };

	/// <summary>
	/// クォータニオンクラス。
	/// </summary>
	class Quaternion : public Vector4{
	public:
		static const Quaternion Identity;		//!<単位クォータニオン。
		Quaternion() 
		{
			x = y = z = 0.0f;
			w = 1.0f;
		}
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		Quaternion(float x, float y, float z, float w) :
			Vector4(x, y, z, w)
		{
		}
		/// <summary>
		/// X軸周りの回転クォータニオンを作成。
		/// </summary>
		/// <param name="axis"></param>
		/// <param name="angle"></param>
		void SetRotationX(float angle)
		{
			SetRotation(g_vec3AxisX, angle);
		}
		/// <summary>
		/// X軸周りの回転クォータニオンを作成。
		/// </summary>
		/// <param name="angle">回転角度。単位Degree</param>
		void SetRotationDegX(float angle)
		{
			SetRotationDeg(g_vec3AxisX, angle);
		}
		/// <summary>
		/// Y軸周りの回転クォータニオンを作成。
		/// </summary>
		/// <param name="axis"></param>
		/// <param name="angle"></param>
		void SetRotationY(float angle)
		{
			SetRotation(g_vec3AxisY, angle);
		}
		/// <summary>
		/// Y軸周りの回転クォータニオンを作成。
		/// </summary>
		/// <param name="angle">回転角度。単位Degree</param>
		void SetRotationDegY(float angle)
		{
			SetRotationDeg(g_vec3AxisY, angle);
		}
		/// <summary>
		/// Y軸周りの回転クォータニオンを作成。
		/// </summary>
		/// <param name="direction">方向ベクトル。</param>
		void SetRotationYFromDirectionXZ(const Vector3& direction)
		{
			SetRotationY(atan2f(direction.x, direction.z));
		}
		/// <summary>
		/// Z軸周りの回転クォータニオンを作成。
		/// </summary>
		/// <param name="axis"></param>
		/// <param name="angle"></param>
		void SetRotationZ(float angle)
		{
			SetRotation(g_vec3AxisZ, angle);
		}
		/// <summary>
		/// Z軸周りの回転クォータニオンを作成。
		/// </summary>
		/// <param name="angle">回転角度。単位Degree</param>
		void SetRotationDegZ(float angle)
		{
			SetRotationDeg(g_vec3AxisZ, angle);
		}

	

		/// <summary>
		/// 任意の軸周りの回転クォータニオンを作成。
		/// </summary>
		/// <param name="axis">回転軸</param>
		/// <param name="angle">回転角度。単位ラジアン。</param>
		void SetRotation( const Vector3& axis, float angle )
		{
			float s;
			float halfAngle = angle * 0.5f;
			s = sinf(halfAngle);
			w = cosf(halfAngle);
			x = axis.x * s;
			y = axis.y * s;
			z = axis.z * s;
		}
		/// <summary>
		/// 任意の軸周りの回転クォータニオンを作成。
		/// </summary>
		/// <param name="axis">回転軸</param>
		/// <param name="angle">回転角度。単位Degree</param>
		void SetRotationDeg(const Vector3& axis, float angle)
		{
			float s;
			float halfAngle = Math::DegToRad(angle) * 0.5f;
			s = sinf(halfAngle);
			w = cosf(halfAngle);
			x = axis.x * s;
			y = axis.y * s;
			z = axis.z * s;
		}
		/// <summary>
		/// 行列からクォータニオンを作成。
		/// </summary>
		/// <param name="m">行列</param>
		void SetRotation(const Matrix& m);
		/// <summary>
		/// fromベクトルからtoベクトルに回転させるクォータニオンを作成。
		/// </summary>
		/// <param name="from">回転前のベクトル</param>
		/// <param name="to">回転後のベクトル</param>
		void SetRotation( Vector3 from,  Vector3 to);
		/// <summary>
		/// 球面線形補完
		/// </summary>
		/// <param name="t">補完率</param>
		/// <param name="q1">開始クォータニオン。</param>
		/// <param name="q2">終了クォータニオン。</param>
		void Slerp(float t, Quaternion q1, Quaternion q2)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv = DirectX::XMQuaternionSlerp(
				DirectX::XMLoadFloat4(&q1.vec),
				DirectX::XMLoadFloat4(&q2.vec),
				t
			);
			DirectX::XMStoreFloat4(&vec, xmv);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// Y軸周りの回転を加算。
		/// </summary>
		/// <returns>加算する回転角度。ラジアン単位。</returns>
		void AddRotationY(float angle)
		{
			Quaternion addRot;
			addRot.SetRotation(Vector3::AxisY, angle);
			*this *= addRot;
		}
		/// <summary>
		/// Y軸周りの回転を加算。
		/// </summary>
		/// <returns>加算する回転角度。単位Degree</returns>
		void AddRotationDegY(float degree)
		{
			float halfAngle = Math::DegToRad(degree) * 0.5f;
			Quaternion addRot;
			addRot.SetRotation(Vector3::AxisY, halfAngle);
			*this *= addRot;
		}
		/// <summary>
		/// X軸周りの回転を加算。
		/// </summary>
		/// <returns>加算する回転角度。ラジアン単位。</returns>
		void AddRotationX(float angle)
		{
			Quaternion addRot;
			addRot.SetRotation(Vector3::AxisX, angle);
			*this *= addRot;
		}
		/// <summary>
		/// X軸周りの回転を加算。
		/// </summary>
		/// <returns>加算する回転角度。単位Degree</returns>
		void AddRotationDegX(float degree)
		{
			float halfAngle = Math::DegToRad(degree) * 0.5f;
			Quaternion addRot;
			addRot.SetRotation(Vector3::AxisX, halfAngle);
			*this *= addRot;
		}
		/// <summary>
		/// Z軸周りの回転を加算。
		/// </summary>
		/// <returns>加算する回転角度。ラジアン単位。</returns>
		void AddRotationZ(float angle)
		{
			Quaternion addRot;
			addRot.SetRotation(Vector3::AxisZ, angle);
			*this *= addRot;
		}
		/// <summary>
		/// Z軸周りの回転を加算。
		/// </summary>
		/// <returns>加算する回転角度。単位Degree</returns>
		void AddRotationDegZ(float degree)
		{
			float halfAngle = Math::DegToRad(degree) * 0.5f;
			Quaternion addRot;
			addRot.SetRotation(Vector3::AxisZ, halfAngle);
			*this *= addRot;
		}
		/// <summary>
		/// クォータニオン同士の乗算
		/// </summary>
		/// <param name="rot"></param>
		void Multiply(const Quaternion& rot)
		{
			float pw, px, py, pz;
			float qw, qx, qy, qz;

			qw = w; qx = x; qy = y; qz = z;
			pw = rot.w; px = rot.x; py = rot.y; pz = rot.z;

			w = pw * qw - px * qx - py * qy - pz * qz;
			x = pw * qx + px * qw + py * qz - pz * qy;
			y = pw * qy - px * qz + py * qw + pz * qx;
			z = pw * qz + px * qy - py * qx + pz * qw;

		}
		/// <summary>
		/// クォータニオン同士の乗算。
		/// </summary>
		/// <param name="rot0"></param>
		/// <param name="rot1"></param>
		void Multiply(const Quaternion& rot0, const Quaternion& rot1)
		{
			float pw, px, py, pz;
			float qw, qx, qy, qz;

			qw = rot0.w; qx = rot0.x; qy = rot0.y; qz = rot0.z;
			pw = rot1.w; px = rot1.x; py = rot1.y; pz = rot1.z;

			w = pw * qw - px * qx - py * qy - pz * qz;
			x = pw * qx + px * qw + py * qz - pz * qy;
			y = pw * qy - px * qz + py * qw + pz * qx;
			z = pw * qz + px * qy - py * qx + pz * qw;
		}
		/// <summary>
		/// クォータニオンの代入乗算演算子
		/// </summary>
		const Quaternion& operator*=(const Quaternion& rot0)
		{
			Multiply(rot0, *this);
			return *this;
		}
		/// <summary>
		/// ベクトルにクォータニオンを適用する。
		/// </summary>
		void Apply(Vector4& _v) const
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
			DirectX::XMStoreFloat4(&_v.vec, xmv);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// ベクトルにクォータニオンを適用する。
		/// </summary>
		void Apply(Vector3& _v) const
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
			DirectX::XMStoreFloat3(&_v.vec, xmv);
#else
			assert(false);
#endif
		}
	};
	
	//整数型のベクトルクラス。
#ifdef USE_DIRECTX_MATH
	__declspec(align(16))
#endif
    class Vector4i {
	public:
		union {
			struct { int x, y, z, w; };
			int v[4];
		};
	};
	/// <summary>
	/// ベクトル同士の加算。
	/// </summary>
	static inline Vector3 operator+(const Vector3& v0, const Vector3& v1)
	{
		Vector3 result;
		result.Add(v0, v1);
		return result;
	}
	/// <summary>
	/// ベクトルとスカラーの乗算。
	/// </summary>

	static inline Vector3 operator*(const Vector3& v, float s)
	{
		Vector3 result;
		result = v;
		result.Scale(s);
		return result;
	}
	/// <summary>
	/// ベクトルとスカラーの除算。
	/// </summary>
	static inline Vector3 operator/(const Vector3& v, float s)
	{
		Vector3 result;
		result = v;
		result.Div(s);
		return result;
	}
	/// <summary>
	/// ベクトル同士の減算。
	/// </summary>
	static inline Vector3 operator-(const Vector3& v0, const Vector3& v1)
	{
		Vector3 result;
		result.Subtract(v0, v1);
		return result;
	}

	/// <summary>
	/// 外積を計算。
	/// </summary>
	static inline Vector3 Cross(const Vector3& v0, const Vector3& v1)
	{
		Vector3 result;
		result.Cross(v0, v1);
		return result;
	}
	/// <summary>
	/// 内積を計算。
	/// </summary>
	static inline float Dot(const Vector3& v0, const Vector3& v1)
	{
		return v0.Dot(v1);
	}

	/// <summary>
	/// ベクトル同士の加算。
	/// </summary>

	static inline Vector4 operator+(const Vector4& v0, const Vector4& v1)
	{
		Vector4 result;
		result.Add(v0, v1);
		return result;
	}
	/// <summary>
	/// ベクトルとスカラーの乗算。
	/// </summary>

	static inline Vector4 operator*(const Vector4& v, float s)
	{
		Vector4 result;
		result = v;
		result.Scale(s);
		return result;
	}

	/// <summary>
	/// ベクトル同士の減算。
	/// </summary>
	static inline Vector4 operator-(const Vector4& v0, const Vector4& v1)
	{
		Vector4 result;
		result.Subtract(v0, v1);
		return result;
	}


	/// <summary>
	/// 内積を計算。
	/// </summary>
	static inline float Dot(const Vector4& v0, const Vector4& v1)
	{
		return v0.Dot(v1);
	}

	static inline Vector4 operator*(const Vector4& v0, const Vector4& v1)
	{
		Vector4 result;
		result.x = v0.x * v1.x;
		result.y = v0.y * v1.y;
		result.z = v0.z * v1.z;
		result.w = v0.w * v1.w;
		return result;
	}
	/// <summary>
	/// クォータニオン同士の乗算。
	/// </summary>
	static inline Quaternion operator*(const Quaternion& q1, const Quaternion q2)
	{
		Quaternion qRet;
		qRet.Multiply(q2, q1);
		return qRet;
	}
	static inline Vector4 operator/(const Vector4& v, float s)
	{
		Vector4 result;
		result = v;
		result.Scale(1.0f/s);
		return result;
	}
	static inline Vector2 operator-(const Vector2& v0, const Vector2& v1)
	{
		Vector2 result;
		result.x = v0.x - v1.x;
		result.y = v0.y - v1.y;
		return result;
	}
	inline Vector2::Vector2(const Vector3& v)
	{
		x = v.x;
		y = v.y;
	}
	inline ColorR8G8B8A8::ColorR8G8B8A8(const Vector4& color)
	{
		r = static_cast<float>(color.r);
		g = static_cast<float>(color.g);
		b = static_cast<float>(color.b);

	}
	const Quaternion g_quatIdentity = { 0.0f,  0.0f, 0.0f, 1.0f };
}
