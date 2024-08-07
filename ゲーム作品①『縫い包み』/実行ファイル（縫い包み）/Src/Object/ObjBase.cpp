#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "Common/Transform.h"

#include "ObjBase.h"

ObjBase::ObjBase(void)
	: resMng_(ResourceManager::GetInstance()),scnMng_(SceneManager::GetInstance())
{
}

ObjBase::~ObjBase(void)
{
	transform_.Release();
}

const Transform& ObjBase::GetTransform(void) const
{
	return transform_;
}

