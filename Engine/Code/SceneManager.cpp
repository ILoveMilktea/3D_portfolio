#include "EngineStdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectFactory.h"
#include "MeshStore.h"
#include "TextureStore.h"
#include "DataStore.h"
#include "Object.h"
#include "WndApp.h"
#include "FRC.h"
#include "CameraManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSceneManager)
void CSceneManager::Awake(void)
{
	__super::Awake();
}

void CSceneManager::Start(void)
{
}

void CSceneManager::FixedUpdate(void)
{
	if (m_pCurScene != nullptr && m_pCurScene->GetEnable())
	{
		m_pCurScene->FixedUpdate();
	}
}

void CSceneManager::Update(void)
{
	m_sceneChanged = false;
	if (m_pCurScene != nullptr && m_pCurScene->GetEnable())
	{
		m_pCurScene->Update();
	}
}

void CSceneManager::LateUpdate(void)
{
	if (m_pCurScene != nullptr && m_pCurScene->GetEnable())
	{
		m_pCurScene->LateUpdate();
	}
}



void CSceneManager::OnDestroy(void)
{
	while (m_sScene.size() != 0)
	{
		m_sScene.top()->Free();
		m_sScene.pop();
	}
}

void CSceneManager::OnEnable(void)
{
}

void CSceneManager::OnDisable(void)
{
}


void CSceneManager::SceneChange(CScene* pScene, _bool alreadyStarted, _bool deleteCurScene/* = true */)
{
	CFRC::GetInstance()->OnDisable();
	if (m_pCurScene && deleteCurScene)
	{
		m_sScene.top()->Free();
		//CObjectFactory::GetInstance()->ClearCurPrototype();
		//CMeshStore::GetInstance()->ClearCurResource();
		//CTextureStore::GetInstance()->ClearCurResource();
		//CDataStore::GetInstance()->ClearCurResource();
		m_sScene.pop();
	}
	else if (m_pCurScene)
	{
		m_pCurScene->SetEnable(false);
		m_pPrevScene = m_pCurScene;
	}

	m_sScene.push(pScene);
	m_pCurScene = pScene;
	
	if(alreadyStarted == false)
		m_pCurScene->Start();

	CFRC::GetInstance()->OnEnable();
	m_sceneChanged = true;
}

void CSceneManager::OrganizeScene(_bool deleteCurScene)
{
	
}