#pragma once
#include <string>
#include "../../Manager/DataManager/CmdManager.h"
#include "ButtonUI.h"
class CmdButton :
    public ButtonUI
{
public:

    CmdButton();
    ~CmdButton();

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    //�{�^������
    void Create(const Vector2& pos, const Vector2& size, const int& cmdNum);

    //�{�^���̏�Ƀ}�E�X������Ă��邩�ǂ���
    bool MouseOnButton(void);

    //�R�}���h���̍ăZ�b�g
    void SetCmd(const int& cmdNum);

    //�R�}���h�ڍו`��
    void DrawCmdDetail(void);

private:
    //�{�^���T�C�Y
    Vector2 size_;

    //�R�}���h�ԍ�
    int cmdNum_;

    //�R�}���h��
    std::string cmdName_;

    //�R�}���h�ڍהw��
    int cmtDetailBack_;

    //�t�H���g
    int fontHandle_;
    //�R�}���h�{�^���摜
    int btnImg_;


    //�l�p�`��Ƀ}�E�X�����邩�̊m�F
    bool IsMouseInRect(
        const Vector2& inside, const Vector2& outside, const Vector2& size);

    //�o�t�̕\�����ύX
    std::string ChageDenoteBuffName(const std::string& buff);


};

