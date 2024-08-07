#pragma once
#include <memory>
#include "ActorBase.h"
class Capsule;
class ItemBase;
class GoalGate;
class SoundController;

class Player :
    public ActorBase
{
public:

    // �ċp��������
    static constexpr float BURN_END_TIME = 6.0f;
    // �아�̎g�p�N�[���^�C��
    static constexpr float AMULET_USE_COOL_TIME = 6.0f;

    Player(void);
    virtual ~Player(void);
    void LoadData(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;

    //�L�����N�^�[�����̎擾
    const VECTOR GetDirection(void)const;

    //�A�C�e�����̒ǉ�
    void AddItemInfo(const std::weak_ptr<ItemBase> coll);
    //�A�C�e�����̏���
    void ClearItemInfos(void);

    //�아�̎g�p����
    const bool IsUseAmulet(void)const { return isUseAmulet_; }
    const void SetUseAmulet(const bool& use) { isUseAmulet_ = use; }

    //�E�o����
    const bool IsEscape(void)const { return isEscape_; }

    //�Q�[���I�[�o�[�̐ݒ�
    void SetGameOverInfo(const VECTOR& opponentPos)override;

    //�S�[���Q�[�g�̐ݒ�
    void SetGoalGate(std::weak_ptr<GoalGate> goalGate) { goalGate_ = goalGate; }


#pragma region Get<-PlayerUI
    //�A�N�V�����e�L�X�g�̎擾
    const std::wstring& GetActionText(void)const { return actionText_; }
    //�R�����g�e�L�X�g�̎擾
    const std::wstring& GetCommentText(void)const { return commentText_; }
    //�ċp����
    const float& GetBurnTime(void)const { return burnTime_; }
    //�아�̏��������̎擾
    const int& GetAmuletNum(void)const {return haveAmuletNum_;}
    //�ʂ�����݂̏�������̎擾
    const bool& HaveTheDoll(void)const { return haveTheDoll_; }
#pragma endregion


private:

    //�I�u�W�F�N�g�Փ˗pray
    VECTOR objHitRayPosFront_;
    VECTOR objHitRayPosBack_;

    //�A�C�e�����
    std::vector<std::weak_ptr<ItemBase>>itemInfos_;
    //�S�[���Q�[�g���
    std::weak_ptr<GoalGate>goalGate_;

    //�아�F�g�p����
    bool isUseAmulet_ = false;
    //�������Ă���아�̐�
    int haveAmuletNum_ = 0;
    //�아�̃N�[���^�C���v��
    float amuletCoolTime_;

    //�����ʂ������
    std::weak_ptr<ItemBase>haveDoll_;
    //�ʂ�����݂��������Ă��邩�ǂ���
    bool haveTheDoll_ = false;
    //�ċp����
    float burnTime_ = 0.0f;

    //�A�N�V�����R�}���h�̕`�敶��
    std::wstring actionText_;
    //�v���C���[�R�����g�̕`�敶��
    std::wstring commentText_;

    //���C�g�n���h��
    int lightHandle_;

    //�E�o����
    bool isEscape_;

    //�����T�E���h
    std::shared_ptr<SoundController>footstepsSound_;
    //�擾�T�E���h
    std::shared_ptr<SoundController>getItemSound_;
    //�아�g�p�T�E���h
    std::shared_ptr<SoundController>useAmuletSound_;
    //�아�Ď��T�E���h
    std::shared_ptr<SoundController>burnAmuletSound_;


    //�A�C�e���Ƃ̎����Փ˔���
    void CollisionItem(void);
    //�c�ݕǂƂ̎����Փ˔���
    void CollisionGoalWall(void);
    //�S�[���Ƃ̎����Փ˔���
    void CollisionGoalDoor(void);

    //����
    void ProcessMove(void);         //�v���C���[�̓���
    void ProcessObtainItem(void);   //�A�C�e���̓���
    void ProcessUseItem(void);      //�A�C�e���̎g�p

    //�ʂ�����݂̏ċp���f
    bool CheckBurnTheDoll(void);
    //�ʂ�����݂̑S�Ŕ���    true�F�S�� / false�F�S�ł��Ă��Ȃ�
    bool CheckDeadAllDoll(void);

    //�A�N�V�����R�}���h�̕`�敶���Z�b�g
    void SetActionText(std::wstring text);
    //�R�����g�e�L�X�g�̕`�敶���Z�b�g
    void SetCommentText(std::wstring text);

    //�X�V����
    void UpdateSearch(void);        //�T�����̍X�V
    void UpdateBurning(void);       //�ʂ�����ݏċp���̍X�V
    void UpdateGameOver(void)override;      //�Q�[���I�[�o�[��Ԃ̍X�V


};

