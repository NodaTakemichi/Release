#include "../../../Application.h"
#include "PixelMaterial.h"
#include "PixelRenderer.h"

PixelRenderer::PixelRenderer(PixelMaterial& pixelMaterial):
	pixelMaterial_(pixelMaterial)
{
	pos_ = Vector2();
	size_ = Vector2();

}

PixelRenderer::~PixelRenderer(void)
{
}

void PixelRenderer::Draw(void)
{
	//�V�F�[�_�[�f�[�^�FPS
	auto psData = pixelMaterial_.GetShaderDataPS();

	// �V�F�[�_�[�ݒ�
	SetUsePixelShader(psData.handle);

	//�萔�o�b�t�@
	auto cb = psData.constBufHandle;

	// �e�N�X�`���̐ݒ�
	int index = 0;
	for (const auto& tex : pixelMaterial_.GetTextures())
	{
		SetUseTextureToShader(index, tex);
		index++;
	}

	// �萔�o�b�t�@
	FLOAT4* constBufsPtr =
		(FLOAT4*)GetBufferShaderConstantBuffer(cb);
	// ��Z�F
	for (const auto& constBuf : psData.constBufs)
	{
		constBufsPtr->x = constBuf.x;
		constBufsPtr->y = constBuf.y;
		constBufsPtr->z = constBuf.z;
		constBufsPtr->w = constBuf.w;
		constBufsPtr++;
	}

	// �萔�o�b�t�@���X�V���ď������񂾓��e�𔽉f����
	UpdateShaderConstantBuffer(cb);
	// �萔�o�b�t�@���s�N�Z���V�F�[�_�[�p�萔�o�b�t�@���W�X�^�ɃZ�b�g
	SetShaderConstantBuffer(
		cb, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);
	
	SetDrawMode(DX_DRAWMODE_NEAREST);
	// �`��
	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);
	SetDrawMode(DX_DRAWMODE_BILINEAR);

}

void PixelRenderer::Draw(int x, int y)
{
}

void PixelRenderer::MakeSquereVertex(Vector2 pos, Vector2 size)
{
	int cnt = 0;
	float sX = static_cast<float>(pos.x);
	float sY = static_cast<float>(pos.y);
	float eX = static_cast<float>(pos.x+ size.x);
	float eY = static_cast<float>(pos.y+ size.y);

	// �S���_�̏�����
	for (int i = 0; i < 4; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	// ����
	vertexs_[cnt].pos = VGet(sX, sY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// �E��
	vertexs_[cnt].pos = VGet(eX, sY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// �E��
	vertexs_[cnt].pos = VGet(eX, eY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	// ����
	vertexs_[cnt].pos = VGet(sX, eY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;

	// ���_�C���f�b�N�X
	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;
	indexes_[cnt++] = 3;
}

