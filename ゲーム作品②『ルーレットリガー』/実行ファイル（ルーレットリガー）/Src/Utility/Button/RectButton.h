#pragma once
#include "ButtonUI.h"
class RectButton :
    public ButtonUI
{
public:

    RectButton();
    ~RectButton();

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    //�{�^������
    void Create(Vector2 pos, int back, int front = 0);
    void Create(Vector2 pos, Vector2 size, int back, int front = 0);

    //�{�^���̏�Ƀ}�E�X������Ă��邩�ǂ���
    bool MouseOnButton(void);

    //�ړ�����W�܂ł̈ړ��l���Z�b�g
    void SetMovedPos(Vector2 moveDis);



private:
    //�{�^���T�C�Y
    Vector2 size_;


    //�ړ���A�ړ��O���W
    Vector2 beforMovePos_;
    Vector2 afterMovePos_;
    //�ړ��o�ߎ���
    float movingTime_;

    //�l�p�`��Ƀ}�E�X�����邩�̊m�F
    bool IsMouseInRect(
        const Vector2& inside, const Vector2& outside, const Vector2& size);

    //�ړ����W�ɂ��Ă��邩�ǂ������f
    bool IsGoalPos(void);
    //���W�ړ�
    void MovePosProcess(void);

};

