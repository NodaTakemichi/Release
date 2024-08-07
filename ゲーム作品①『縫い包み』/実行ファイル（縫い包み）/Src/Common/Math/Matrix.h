
#pragma once

#include "Vector.h"

namespace RasterizerBasedSoftwareRendering {
	/// <summary>
	/// 行列クラス。
	/// </summary>
	class Matrix {
	public:

		union {
#ifdef USE_DIRECTX_MATH
			DirectX::XMFLOAT4X4 mat;
#endif
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			Vector4 v[4];
			float m[4][4];
		};
		//単位行列
		static const Matrix Identity;
	public:
#ifdef USE_DIRECTX_MATH
		/// <summary>
		/// DirectX::XMMATRIX型への暗黙の型変換。
		/// </summary>
		/// <returns></returns>
		operator DirectX::XMMATRIX() const
		{
			return DirectX::XMLoadFloat4x4(&mat);
		}
#endif
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <remarks>
		/// 単位行列として初期化されます。
		/// </remarks>
		Matrix() {
			m[0][0] = 1.0f;
			m[0][1] = 0.0f;
			m[0][2] = 0.0f;
			m[0][3] = 0.0f;

			m[1][0] = 0.0f;
			m[1][1] = 1.0f;
			m[1][2] = 0.0f;
			m[1][3] = 0.0f;

			m[2][0] = 0.0f;
			m[2][1] = 0.0f;
			m[2][2] = 1.0f;
			m[2][3] = 0.0f;

			m[3][0] = 0.0f;
			m[3][1] = 0.0f;
			m[3][2] = 0.0f;
			m[3][3] = 1.0f;
		}
		Matrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[0][3] = m03;

			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[1][3] = m13;

			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
			m[2][3] = m23;

			m[3][0] = m30;
			m[3][1] = m31;
			m[3][2] = m32;
			m[3][3] = m33;
		}
#ifdef USE_DIRECTX_MATH
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="m"></param>
		Matrix(const DirectX::XMFLOAT4X4& m)
		{
			mat = m;
		}
#endif
		/// <summary>
		/// 代入演算子。
		/// </summary>
		/// <param name="_m"></param>
		/// <returns></returns>
		Matrix& operator=(const Matrix& _m)
		{
			memcpy(m, _m.m, sizeof(m));
			return *this;
		}
		static inline Vector4 Mul(const Vector4& v, const Matrix& m) 
		{
			Vector4 result = v;
			m.Apply(result);
			return result;
		}
		/// <summary>
		/// ベクトルと3x3行列を乗算。
		/// </summary>
		/// <param name="vOut">乗算されるベクトル。</param>
		void Apply3x3(Vector3& vOut) const
		{
			Vector3 vTmp = vOut;
			vOut.x = vTmp.x * m[0][0] + vTmp.y * m[1][0] + vTmp.z * m[2][0];
			vOut.y = vTmp.x * m[0][1] + vTmp.y * m[1][1] + vTmp.z * m[2][1];
			vOut.z = vTmp.x * m[0][2] + vTmp.y * m[1][2] + vTmp.z * m[2][2];
		}
		
		/// <summary>
		/// ベクトルに行列を乗算。
		/// </summary>
		/// <param name="vOut">乗算されるベクトル。</param>
		void Apply(Vector3& vOut) const
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat3(
				&vOut.vec,
				DirectX::XMVector3Transform(vOut, *this)
			);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// ベクトルに行列を乗算。
		/// </summary>
		/// <param name="vOut">乗算されるベクトル。</param>
		void Apply(Vector4& vOut) const
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4(
				&vOut.vec,
				DirectX::XMVector4Transform(vOut, *this)
			);
#else
			float fX = (m[0][0] * vOut.v[0]) + (m[1][0] * vOut.v[1]) + (m[2][0] * vOut.v[2]) + (m[3][0] * vOut.v[3]);
			float fY = (m[0][1] * vOut.v[0]) + (m[1][1] * vOut.v[1]) + (m[2][1] * vOut.v[2]) + (m[3][1] * vOut.v[3]);
			float fZ = (m[0][2] * vOut.v[0]) + (m[1][2] * vOut.v[1]) + (m[2][2] * vOut.v[2]) + (m[3][2] * vOut.v[3]);
			float fW = (m[0][3] * vOut.v[0]) + (m[1][3] * vOut.v[1]) + (m[2][3] * vOut.v[2]) + (m[3][3] * vOut.v[3]);
			vOut.Set( fX, fY, fZ, fW );
#endif
		}
		/// <summary>
		/// 平行移動行列を作成。
		/// </summary>
		/// <param name="trans">平行移動。</param>
		void MakeTranslation(const Vector3& trans)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixTranslationFromVector(trans)
			);
#else
			v[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
			v[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
			v[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
			v[3] = { trans.x, trans.y, trans.z, 1.0f };
#endif
		}
		void MakeTranslation(float x, float y, float z)
		{
			MakeTranslation(Vector3(x, y, z));
		}
		/// <summary>
		/// Y軸周りの回転行列を作成。
		/// </summary>
		/// <param name="angle">回転角度(単位：ラジアン)</param>
		void MakeRotationY(float angle)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixRotationY(angle)
			);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// Z軸周りの回転行列を作成。
		/// </summary>
		/// <param name="angle">回転角度(単位：ラジアン)</param>
		void MakeRotationZ(float angle)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixRotationZ(angle)
			);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// X軸周りの回転行列を作成。
		/// </summary>
		/// <param name="angle">回転角度(単位：ラジアン)</param>
		void MakeRotationX(float angle)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixRotationX(angle)
			);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// クォータニオンから回転行列を作成。
		/// </summary>
		/// <param name="q">クォータニオン。</param>
		void MakeRotationFromQuaternion(const Quaternion& q)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixRotationQuaternion(q)
			);
#else
			assert(false);
#endif
		}

		/// <summary>
		/// 任意の軸周りの回転行列を作成。
		/// </summary>
		/// <param name="axis">回転軸。</param>
		/// <param name="angle">回転角度。</param>
		void MakeRotationAxis(const Vector3& axis, float angle)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixRotationAxis(axis, angle)
			);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// 拡大行列を作成。
		/// </summary>
		/// <param name="scale">拡大率。</param>
		void MakeScaling(const Vector3& scale)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixScalingFromVector(scale)
			);
#else
			assert(false);
#endif
		}
		inline void XMScalarSinCos
		(
			float* pSin,
			float* pCos,
			float  Value
		) noexcept
		{
			constexpr float XM_1DIV2PI = 1.570796327f;
			constexpr float XM_2PI = 6.283185307f;
			constexpr float XM_PIDIV2 = 1.570796327f;
			constexpr float XM_PI = 3.141592654f;
			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = XM_1DIV2PI * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - XM_2PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float sign;
			if (y > XM_PIDIV2)
			{
				y = XM_PI - y;
				sign = -1.0f;
			}
			else if (y < -XM_PIDIV2)
			{
				y = -XM_PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 11-degree minimax approximation
			*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

			// 10-degree minimax approximation
			float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			*pCos = sign * p;
		}
		/// <summary>
		/// 透視変換行列を作成。
		/// </summary>
		/// <param name="viewAngle">画角(単位：ラジアン)</param>
		/// <param name="aspect">アスペクト比</param>
		/// <param name="fNear">近平面。</param>
		/// <param name="fFar">遠平面。</param>
		void MakeProjectionMatrix(
			float viewAngle,
			float aspect,
			float fNear,
			float fFar
		)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixPerspectiveFovLH(viewAngle, aspect, fNear, fFar)
			);
#else
			float    SinFov;
			float    CosFov;
			XMScalarSinCos(&SinFov, &CosFov, 0.5f * viewAngle);

			float Height = CosFov / SinFov;
			float Width = Height / aspect;
			float fRange = fFar / (fFar - fNear);

			
			m[0][0] = Width;
			m[0][1] = 0.0f;
			m[0][2] = 0.0f;
			m[0][3] = 0.0f;

			m[1][0] = 0.0f;
			m[1][1] = Height;
			m[1][2] = 0.0f;
			m[1][3] = 0.0f;

			m[2][0] = 0.0f;
			m[2][1] = 0.0f;
			m[2][2] = fRange;
			m[2][3] = 1.0f;

			m[3][0] = 0.0f;
			m[3][1] = 0.0f;
			m[3][2] = -fRange * fNear;
			m[3][3] = 0.0f;
#endif
			
		}
		/// <summary>
		/// 平行投影行列を作成。
		/// </summary>
		/// <param name="w">ビューボリュームの幅。</param>
		/// <param name="h">ビューボリュームの高さ。</param>
		/// <param name="fNear">近平面。</param>
		/// <param name="fFar">遠平面。</param>
		void MakeOrthoProjectionMatrix(float w, float h, float fNear, float fFar)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixOrthographicLH(w, h, fNear, fFar)
			);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// カメラ行列を作成。
		/// </summary>
		/// <param name="position">視点</param>
		/// <param name="target">注視点</param>
		/// <param name="up">カメラの上方向</param>
		void MakeLookAt(const Vector3& position, const Vector3& target, const Vector3& up)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixLookAtLH(position, target, up)
			);
#else
			// XMVECTOR R2 = XMVector3Normalize(EyeDirection);
			Vector3 zAxis = target - position;
			zAxis.Normalize();

			// XMVECTOR R0 = XMVector3Cross(UpDirection, R2);
			// R0 = XMVector3Normalize(R0);
			Vector3 xAxis = Cross(up, zAxis);
			xAxis.Normalize();

			// XMVECTOR R1 = XMVector3Cross(R2, R0);
			Vector3 yAxis = Cross(zAxis, xAxis);
			// yAxis.Normalize();

			// XMVECTOR NegEyePosition = XMVectorNegate(EyePosition);
			Vector3 positionNeg = position * -1.0f;

			float d0 = Dot(xAxis, positionNeg);
			float d1 = Dot(yAxis, positionNeg);
			float d2 = Dot(zAxis, positionNeg);

			v[0] = { xAxis.x, xAxis.y, xAxis.z, d0 };
			v[1] = { yAxis.x, yAxis.y, yAxis.z, d1 };
			v[2] = { zAxis.x, zAxis.y, zAxis.z, d2 };
			v[3] = { 0, 0, 0, 1 };

			Transpose();
#endif
		}
		/// <summary>
		/// 行列と行列の乗算
		/// </summary>
		/// <remarks>
		/// *this = m0 * m1;
		/// </remarks>
		void Multiply(const Matrix& m0, const Matrix& m1)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMFLOAT4X4 lm;
			DirectX::XMStoreFloat4x4(
				&lm,
				DirectX::XMMatrixMultiply(m0, m1)
			);
			mat = lm;
#else
			assert(false);
#endif
		}

		/// <summary>
		/// 行列の代入演算子
		/// </summary>
		/// <remarks>
		/// *this *= _m;
		/// return *this;
		/// </remarks>
		const Matrix& operator*=(const Matrix& _m)
		{
			Multiply(*this, _m);
			return *this;
		}
		/// <summary>
		/// 逆行列を計算。
		/// </summary>
		/// <param name="_m">元になる行列。</param>
		void Inverse(const Matrix& _m)
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixInverse(NULL, _m)
			);
#else
			assert(false);
#endif
		}
		/// <summary>
		/// 自身の逆行列を計算。
		/// </summary>
		void Inverse()
		{
			Inverse(*this);
		}
		/// <summary>
		/// 転置行列を計算。
		/// </summary>
		void Transpose()
		{
#ifdef USE_DIRECTX_MATH
			DirectX::XMStoreFloat4x4(
				&mat,
				DirectX::XMMatrixTranspose(*this)
			);
#else
			float source[4][4];
			memcpy(source, m, sizeof(m));
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m[i][j] = source[j][i];
				}
			}
#endif
		}
	};
	/// <summary>
	/// 行列同士の乗算
	/// </summary>
	/// <remarks>
	/// 乗算は左から右に向かってかかっていく。
	/// </remarks>
	static inline Matrix operator*(const Matrix& m1, const Matrix m2)
	{
		Matrix mRet;
		mRet.Multiply(m1, m2);
		return mRet;
	}
	/// <summary>
	/// 単位行列
	/// </summary>
	const Matrix g_matIdentity = {
		1.0f, 0.0f, 0.0f, 0.0f ,
		0.0f, 1.0f, 0.0f, 0.0f ,
		0.0f, 0.0f, 1.0f, 0.0f ,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	/// <summary>
	/// 零行列
	/// </summary>
	const Matrix g_matZero = {
		0.0f, 0.0f, 0.0f, 0.0f ,
		0.0f, 0.0f, 0.0f, 0.0f ,
		0.0f, 0.0f, 0.0f, 0.0f ,
		0.0f, 0.0f, 0.0f, 0.0f
	};
}
