#pragma once
#include "StageBase.h"
class ModelMaterial;
class ModelRenderer;
class WarpGate :
    public StageBase
{
public:
	WarpGate(void);
	virtual ~WarpGate(void);

	void LoadData(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	//���W�Z�b�g
	const void SetPos(const VECTOR& pos);
	//�p�x�Z�b�g
	const void SetRot(const VECTOR& rot);

private:

	std::unique_ptr<ModelMaterial>modelMaterial_;	//���f���}�e���A��
	std::unique_ptr<ModelRenderer>modelRenderer_;	//���f�������_���[

};

