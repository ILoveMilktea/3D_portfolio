#include "EngineStdafx.h"
#include "MeshStore.h"
#include "DeviceManager.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"


#include <fstream>
#include <algorithm>
#include <filesystem>

USING(Engine)
IMPLEMENT_SINGLETON(CMeshStore)

void CMeshStore::Awake(void)
{
	__super::Awake();
	m_fpResourceHandler = std::bind(&CMeshStore::ParsingMesh, this, std::placeholders::_1, std::placeholders::_2);
	m_resourcePath = L"..\\..\\Resource\\Mesh";
}

void CMeshStore::Start(void)
{
	__super::Start();
}

void CMeshStore::OnDestroy(void)
{
	for (auto& mesh : m_mStaticMeshData)
		mesh.second->Free();
	m_mStaticMeshData.clear();

	for (auto& mesh : m_mCurSceneMeshData)
		mesh.second->Free();
	m_mCurSceneMeshData.clear();
}

void CMeshStore::ClearCurResource(void)
{
	for (auto& mesh : m_mCurSceneMeshData)
		mesh.second->Free();
	m_mCurSceneMeshData.clear();
}

CMeshData* CMeshStore::GetMeshData(std::wstring meshKey)
{
	auto iter_find_static = m_mStaticMeshData.find(meshKey);
	if (iter_find_static != m_mStaticMeshData.end())
		return iter_find_static->second->MakeClone();

	auto iter_find_cur = m_mCurSceneMeshData.find(meshKey);
	if (iter_find_cur != m_mCurSceneMeshData.end())
		return iter_find_cur->second->MakeClone();

	MSG_BOX(__FILE__, L"meshKey is broken in GetMeshData");
	ABORT;

	return nullptr;
}

void CMeshStore::InitMeshForScene(std::wstring curScene)
{
	InitResource(m_resourcePath + L"\\" + curScene);
}

void CMeshStore::InitResource(std::wstring sourcePath)
{
	__super::InitResource(sourcePath);
}

void CMeshStore::ParsingMesh(std::wstring filePath, std::wstring fileName)
{
	std::wstring fullFilePath = filePath + fileName;
	
	if (filePath.find(L"\\Static\\") != std::wstring::npos)
	{	
		CStaticMesh* pNewStaticMesh = CStaticMesh::Create(filePath, fileName);
		if (m_isStatic)
			m_mStaticMeshData[pNewStaticMesh->GetMeshKey()] = pNewStaticMesh;
		else
			m_mCurSceneMeshData[pNewStaticMesh->GetMeshKey()] = pNewStaticMesh;
	}
	else// if (GetLastDirName(fullFilePath) == L"Dynamic")//Dynamic Mesh
	{
		CDynamicMesh* pNewDynamicMesh = CDynamicMesh::Create(filePath, fileName);
		if (m_isStatic)
			m_mStaticMeshData[pNewDynamicMesh->GetMeshKey()] = pNewDynamicMesh;
		else
			m_mCurSceneMeshData[pNewDynamicMesh->GetMeshKey()] = pNewDynamicMesh;
	}

}


