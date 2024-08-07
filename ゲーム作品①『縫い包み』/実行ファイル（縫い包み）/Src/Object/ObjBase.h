#pragma once
#include <memory>
#include <cassert>
#include "Common/Transform.h"
class ResourceManager;
class SceneManager;

class ObjBase
{

public:
	ObjBase(void);
	virtual ~ObjBase(void);

	virtual void LoadData(void) = 0;
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	const Transform& GetTransform(void) const;

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform transform_;

private:

};
