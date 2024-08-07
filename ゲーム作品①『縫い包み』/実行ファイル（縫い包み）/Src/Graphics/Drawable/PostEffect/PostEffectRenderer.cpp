#include "../../../Application.h"
#include "../../../Manager/SceneManager.h"
#include "PostEffectMaterial.h"
#include "PostEffectRenderer.h"

PostEffectRenderer::PostEffectRenderer(PostEffectMaterial& postEffectMaterial):
	postEffectMaterial_(postEffectMaterial)
{
	//�|���S���̍쐬
	MakeSquereVertex();

	//���C���X�N���[��
	mainScreen_ = SceneManager::GetInstance().GetMainScreen();
}


PostEffectRenderer::~PostEffectRenderer(void)
{
}


void PostEffectRenderer::Draw(void)
{
	//�X�N���[�����Z�b�g
	int screen = postEffectMaterial_.GetPEScreen();
	SetDrawScreen(screen);
	ClearDrawScreen();

	//�V�F�[�_�[�f�[�^�FPS
	auto psData = postEffectMaterial_.GetShaderDataPS();

	// �V�F�[�_�[�ݒ�
	SetUsePixelShader(psData.handle);

	//�萔�o�b�t�@
	auto cb = psData.constBufHandle;

	// �e�N�X�`���̐ݒ�
	//�ꖇ�ڂ͕K���A���C���X�N���[����n��
	SetUseTextureToShader(0, mainScreen_);
	//2���ڈȍ~
	int index = 1;
	for (const auto& tex : postEffectMaterial_.GetTextures())
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

	// �`��
	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);

	// ���C���ɖ߂�
	SetDrawScreen(mainScreen_);
	DrawGraph(0, 0, screen, false);
}

void PostEffectRenderer::MakeSquereVertex(void)
{
	int cnt = 0;
	float sX = static_cast<float>(0.0f);
	float sY = static_cast<float>(0.0f);
	float eX = static_cast<float>(Application::SCREEN_SIZE_X);
	float eY = static_cast<float>(Application::SCREEN_SIZE_Y);

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
