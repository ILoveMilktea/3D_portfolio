#include "stdafx.h"
#include "Loading.h"


CLoading::CLoading()
{
}


CLoading::~CLoading()
{
}

_uint CLoading::ThreadMain(void * pArg)
{
	CLoading* pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(&pLoading->m_crt);

	switch (pLoading->GetLoadingID())
	{
	case (_int)ELoadingID::StaticResource:
		break;

	case (_int)ELoadingID::BossSceneResource:
		break;
	}
	
	LeaveCriticalSection(&pLoading->m_crt);
	_endthreadex(0);

	return iFlag;
}

CLoading * CLoading::Create(_int loadingID)
{
	CLoading* pInstance = new CLoading;

	pInstance->StartLoading(loadingID);
	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_threadHandle, INFINITE);
	CloseHandle(m_threadHandle);
	DeleteCriticalSection(&m_crt);
}

void CLoading::StartLoading(_int loadingID)
{
	InitializeCriticalSection(&m_crt);
	m_threadHandle = (HANDLE)_beginthreadex(NULL, 0, ThreadMain, this, 0, NULL);
	m_loadingID = loadingID;
}

