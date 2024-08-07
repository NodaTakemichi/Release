#pragma once
#include<memory>
#include<string>
class Player;
class PixelMaterial;
class PixelRenderer;

class PlayerUI
{
public:

	PlayerUI(const Player& player);
	~PlayerUI(void);

	void LoadData(void);
	void Init(void);
	void Update(void);
	void Draw(void);


private:
	const Player& player_;

	//��ʒ����摜
	int centerImg_;

	//�아UI�摜
	int amiletUiImage_;
	//�ʂ������UI�摜
	int doolUiImage_;

	//�t�H���g�n���h��
	int fontHandle_;
	//�A�N�V�����e�L�X�g�w�i�摜
	int actionTextBackImg_;
	//�A�N�V�����e�L�X�g�i��Ɏ��A�R�₷.....�j
	std::wstring actionText_;

	//�R�����g�e�L�X�g�w�i�摜
	int commnetTextBackImg_;
	//�R�����g�e�L�X�g
	std::wstring commnetText_;

	//�R�����g�̕\�����ԃJ�E���g
	float displayCoundTime_;
	//�R�����g�̕\�����f
	bool isDisplayCommentText_;


	// �아�̃f�B�]���u�V�F�[�_�[
	std::unique_ptr<PixelMaterial>amuletMaterial_;
	std::unique_ptr<PixelRenderer> amuletRenderer_;
	float amuletBurnTime_ ;
	//�아�̏���o�������f
	bool isBurningAmulet_ ;

	// �ċp�Q�[�W
	std::unique_ptr<PixelMaterial>gaugeMaterial_;
	std::unique_ptr<PixelRenderer> gaugeRenderer_;

	//�A�N�V�����e�L�X�g�̕`��
	void DrawTextUI(void);

	//�ċp���Ԃ̕`��
	void DrawBurningTime(void);

	//�아�̏���o
	void DrawBurningAmulet(void);

	//�����A�C�e���̕\��
	void DrawHasItem(void);
};

