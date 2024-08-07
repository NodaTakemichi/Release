#pragma once
#include <vector>
#include "SceneBase.h"
#include "../Utility/DrawShader.h"
#include "../Common/Vector2.h"

class UnitManager;
class Roulette;
class GameUI;

class BattleSystem;
class DeathStaging;
class EffectManager;

class GameScene : public SceneBase
{

	const float START_IMAGE_FADE_TIME = 2.0f;

public:
	enum class GAME_PHASE
	{
		NONE,
		BATTLE_START,
		RULLET_TIME,
		AIM,
		EFFECT,
		BATTLE,
		BUFF_EFFECT,
		TURN_END,
		GAME_END
	};


	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	void UpdateRouTime(void);
	void UpdateAIM(void);
	void UpdateEffect(void);
	void UpdateBattle(void);
	void UpdateBuffEffect(void);
	void UpdateTurnEnd(void);
	void UpdateGameEnd(void);


private:
	//���j�b�g�}�l�[�W���[
	UnitManager* unitMng_;
	//���[���b�g
	Roulette* roulette_;
	//UI
	GameUI* GameUi_;
	//���j�b�g�̃o�g���V�X�e��
	BattleSystem* battleSys_;

	//�Q�[���t�F�[�Y
	GAME_PHASE phase_;
	//�l�N�X�g�t�F�[�Y
	GAME_PHASE nextPhase_;

	//�t���[��
	int frameImg_;
	int topFrameImg_;

	//�s�����j�b�g�̑��삪�������蓮�����f
	bool actUnitAoutm_;

	//��ʃX�N���[��
	int mainScreen_;
	DrawShader::PS_TYPE psType_;
	COLOR_F screenPsBuf_;

	//�X�N���[�����W
	Vector2 screenPos_;
	//�X�N���[���U��
	float amplitude_;
	//�t���[��
	int frame_;

	//�����֘A
	int winBgm_;		//����BGM
	int loseBgm_;		//�s�kBGM
	int resultBgm_;		//����BGM

	int turnChangeSE_;	//�^�[���ύXSE

	//UI�t�H���g�n���h��
	int uiFontHandle_;
	
	//��������
	bool CheckVictory(void);
	bool isPlayerWin_;

	//�������ʉ摜
	int resultImg_;		//���ʉ摜
	int winImg_;		//�����摜
	int loseImg_;		//�s�k�摜
	float resutlTime_;	//���ʉ�ʌo�ߎ���

	//�o�g���X�^�[�g�摜
	int battleStartImg_;
	//�o�g���X�^�[�g�p�t�F�[�h�摜
	int bStartFadeImg_;
	//�w�i�摜
	std::vector<int>backImgs_;


	//�X�^�[�g��ʕ`��
	void DrawBattleStart(void);

	//���ʉ�ʕ`��
	void DrawResult(void);
	//���ʉ�ʂ̕`��ݒ�
	void SettingScreenShader(void);

	//�t�F�[�Y�ύX
	void ChangeGamePhase(GAME_PHASE phase);

	//�X�N���[�����W�h��v�Z
	void CalcScreenShake(void);

};
