//�C���N���[�h�K�[�h
#if !defined(VERTEX_FUNCTION_HEADER)
	#define VERTEX_FUNCTION_HEADER

	#include "../Common/CommonFunctionHeader.hlsli"
	#include "VertexStructuresHeader.hlsli"

	/// <summary>
	/// �t�H�O�̋��������߂�
	/// </summary>
	/// <param name="worldPos">���[���h���W</param>
	/// <param name="cameraPos">�J�������W</param>
	/// <param name="fogStart">�t�H�O�̃X�^�[�g�n�_</param>
	/// <param name="fogEnd">�t�H�O�̃G���h�n�_</param>
	/// <returns>�t�H�O�̋���</returns>
	float CalculateFogFactor(in float3 worldPos, in float3 cameraPos, in float fogStart, in float fogEnd)
	{
		float len = length(worldPos.xyz - cameraPos);
    float ret = (fogEnd - len) / (fogEnd - fogStart);
		return saturate(ret);
	}

	/// <summary>
	/// ���[�J�������[���h�̃u�����h�s��̍쐬
	/// </summary>
	/// <param name="indices">�{�[�������p�C���f�b�N�X</param>
	/// <param name="veweightc">�{�[�������p�E�F�C�g</param>
	/// <param name="localWorldMatrix">���[�J�������[���h�̍s��</param>
	/// <param name="mat">�u�����h�s��̓��o��</param>
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
	/// ���[�J�������[���h�̃u�����h�s��̍쐬(1�`4�t���[���̉e�����󂯂钸�_�����ꍇ)
	/// </summary>
	/// <param name="indices">�{�[�������p�C���f�b�N�X</param>
	/// <param name="weight">�{�[�������p�E�F�C�g</param>
	/// <param name="localWorldMatrix">���[�J�������[���h�̍s��</param>
	/// <returns>�u�����h�s��</returns>
	float4x3 CalculateLocalWorldMatrix(in float4 indices, in float4 weight, in VsLocalWorldMatrix localWorldMatrix)
	{
	    float3x4 mat = float3x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		CalculateLocalWorldMatrixImplement(indices, weight, localWorldMatrix, mat);
		return transpose(mat);
	}

	/// <summary>
	/// ���[�J�������[���h�̃u�����h�s��̍쐬(5�`8�t���[���̉e�����󂯂钸�_�����ꍇ)
	/// </summary>
	/// <param name="indices0">�{�[�������p�C���f�b�N�X0</param>
	/// <param name="weight0">�{�[�������p�E�F�C�g0</param>
	/// <param name="indices1">�{�[�������p�C���f�b�N�X1</param>
	/// <param name="weight1">�{�[�������p�E�F�C�g1</param>
	/// <param name="localWorldMatrix">���[�J�������[���h�̍s��</param>
	/// <returns>�u�����h�s��</returns>
	float4x3 CalculateLocalWorldMatrix(in float4 indices0, in float4 weight0, in float4 indices1, in float4 weight1, in VsLocalWorldMatrix localWorldMatrix)
	{
		float3x4 mat = float3x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		CalculateLocalWorldMatrixImplement(indices0, weight0, localWorldMatrix, mat);
	    CalculateLocalWorldMatrixImplement(indices1, weight1, localWorldMatrix, mat);
	    return transpose(mat);
	}

#endif

