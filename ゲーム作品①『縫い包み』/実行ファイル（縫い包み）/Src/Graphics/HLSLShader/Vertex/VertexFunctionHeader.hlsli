//インクルードガード
#if !defined(VERTEX_FUNCTION_HEADER)
	#define VERTEX_FUNCTION_HEADER

	#include "../Common/CommonFunctionHeader.hlsli"
	#include "VertexStructuresHeader.hlsli"

	/// <summary>
	/// フォグの強さを求める
	/// </summary>
	/// <param name="worldPos">ワールド座標</param>
	/// <param name="cameraPos">カメラ座標</param>
	/// <param name="fogStart">フォグのスタート地点</param>
	/// <param name="fogEnd">フォグのエンド地点</param>
	/// <returns>フォグの強さ</returns>
	float CalculateFogFactor(in float3 worldPos, in float3 cameraPos, in float fogStart, in float fogEnd)
	{
		float len = length(worldPos.xyz - cameraPos);
    float ret = (fogEnd - len) / (fogEnd - fogStart);
		return saturate(ret);
	}

	/// <summary>
	/// ローカル→ワールドのブレンド行列の作成
	/// </summary>
	/// <param name="indices">ボーン処理用インデックス</param>
	/// <param name="veweightc">ボーン処理用ウェイト</param>
	/// <param name="localWorldMatrix">ローカル→ワールドの行列</param>
	/// <param name="mat">ブレンド行列の入出力</param>
	void CalculateLocalWorldMatrixImplement(in float4 indices, in float4 weight, in VsLocalWorldMatrix localWorldMatrix, inout float3x4 mat)
	{
	    mat[0] += localWorldMatrix.lwMatrix[indices.x + 0] * weight.x;
		mat[0] += localWorldMatrix.lwMatrix[indices.y + 0] * weight.y;
		mat[0] += localWorldMatrix.lwMatrix[indices.z + 0] * weight.z;
		mat[0] += localWorldMatrix.lwMatrix[indices.w + 0] * weight.w;

	    mat[1] += localWorldMatrix.lwMatrix[indices.x + 1] * weight.x;
	    mat[1] += localWorldMatrix.lwMatrix[indices.y + 1] * weight.y;
	    mat[1] += localWorldMatrix.lwMatrix[indices.z + 1] * weight.z;
	    mat[1] += localWorldMatrix.lwMatrix[indices.w + 1] * weight.w;

	    mat[2] += localWorldMatrix.lwMatrix[indices.x + 2] * weight.x;
	    mat[2] += localWorldMatrix.lwMatrix[indices.y + 2] * weight.y;
	    mat[2] += localWorldMatrix.lwMatrix[indices.z + 2] * weight.z;
	    mat[2] += localWorldMatrix.lwMatrix[indices.w + 2] * weight.w;
	}

	/// <summary>
	/// ローカル→ワールドのブレンド行列の作成(1～4フレームの影響を受ける頂点を持つ場合)
	/// </summary>
	/// <param name="indices">ボーン処理用インデックス</param>
	/// <param name="weight">ボーン処理用ウェイト</param>
	/// <param name="localWorldMatrix">ローカル→ワールドの行列</param>
	/// <returns>ブレンド行列</returns>
	float4x3 CalculateLocalWorldMatrix(in float4 indices, in float4 weight, in VsLocalWorldMatrix localWorldMatrix)
	{
	    float3x4 mat = float3x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		CalculateLocalWorldMatrixImplement(indices, weight, localWorldMatrix, mat);
		return transpose(mat);
	}

	/// <summary>
	/// ローカル→ワールドのブレンド行列の作成(5～8フレームの影響を受ける頂点を持つ場合)
	/// </summary>
	/// <param name="indices0">ボーン処理用インデックス0</param>
	/// <param name="weight0">ボーン処理用ウェイト0</param>
	/// <param name="indices1">ボーン処理用インデックス1</param>
	/// <param name="weight1">ボーン処理用ウェイト1</param>
	/// <param name="localWorldMatrix">ローカル→ワールドの行列</param>
	/// <returns>ブレンド行列</returns>
	float4x3 CalculateLocalWorldMatrix(in float4 indices0, in float4 weight0, in float4 indices1, in float4 weight1, in VsLocalWorldMatrix localWorldMatrix)
	{
		float3x4 mat = float3x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		CalculateLocalWorldMatrixImplement(indices0, weight0, localWorldMatrix, mat);
	    CalculateLocalWorldMatrixImplement(indices1, weight1, localWorldMatrix, mat);
	    return transpose(mat);
	}

#endif

