#include "stdafx.h"
#include "DongScene.h"


CDongScene::CDongScene()
{
}


CDongScene::~CDongScene()
{
}

Engine::CScene * CDongScene::Create(void)
{
	CDongScene* pInstance = new CDongScene;
	pInstance->Awake((_int)ELayerID::NumOfLayerID);

	return pInstance;
}

void CDongScene::Free(void)
{
	OnDestroy();
	delete this;
}

void CDongScene::Awake(_int numOfLayers)
{
	__super::Awake(numOfLayers);
	InitPrototypes();
}

void CDongScene::Start(void)
{
}

void CDongScene::FixedUpdate(void)
{
	__super::FixedUpdate();
}

void CDongScene::Update(void)
{
	__super::Update();

}

void CDongScene::LateUpdate(void)
{
	__super::LateUpdate();

}

void CDongScene::OnDestroy(void)
{
	__super::OnDestroy();

}

void CDongScene::OnEnable(void)
{
	__super::OnEnable();

}

void CDongScene::OnDisable(void)
{
	__super::OnDisable();

}

void CDongScene::InitPrototypes(void)
{
}
