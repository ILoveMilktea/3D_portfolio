#include "EngineStdafx.h"
#include "ResourceStore.h"


USING(Engine)
CResourceStore::CResourceStore()
{
}


CResourceStore::~CResourceStore()
{
}

void CResourceStore::Awake(void)
{
	__super::Awake();
}

void CResourceStore::Start(void)
{
	InitResource(m_resourcePath + L"\\StaticScene");
	m_isStatic = false;
}


void CResourceStore::InitResource(std::wstring sourcePath)
{
	//디렉토리와 파일이름을 저장할 벡터
	std::vector<std::wstring> dirNameVector;
	std::vector<std::wstring> fileNameVector;

	//파일 데이터를 받을 변수
	WIN32_FIND_DATA fileData;



	//현재 보고있는 파일, 디렉토리, 그리고 전체 패스
	std::wstring curFile;
	std::wstring curDir;
	std::wstring fullFilePath;

	//출발점을 넣어준다.
	dirNameVector.push_back(sourcePath);

	do
	{
		//현재 디렉토리를 벡터에서 꺼내어 입력 후 벡터는 pop.back()으로 입력해준 것을 제거.
		curDir = dirNameVector.back();
		dirNameVector.pop_back();

		//받은 디렉토리의 첫 파일을 찾아 fileData에 정보를 넣고 HANDLE을 반환
		HANDLE findHandle = FindFirstFile((curDir + L"\\*").c_str(), &fileData);

		//디렉토리가 잘못되었다면 여기서 터진다.
		if (findHandle == INVALID_HANDLE_VALUE)
		{
			MSG_BOX(__FILE__, L"Given path is wrong during getting handle in ResourceStore");
			ABORT;
		}

		do
		{
			//찾은 fileData로부터 현재 보고있는 파일, 그리고 풀 패스를 설정
			curFile = fileData.cFileName;
			fullFilePath = curDir + L"\\" + curFile;

			//현재 보고있는 파일이 directory인지 아닌지 체크
			bool isItDir = ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) ? false : true;


			//파일 이름이 .으로 시작하면 시스템 파일이다, 넘김.
			if (curFile[0] == '.')
				continue;

			if (curFile == L"더미용파일.dummy")
				continue;


			//디렉토리라면 디렉토리 벡터에 넣고 넘긴다.
			if (isItDir)
			{
				dirNameVector.push_back(fullFilePath);
				continue;
			}

			//디렉토리가 아닌 파일마다 함수 포인터에 담긴 함수를 호출한다.
			m_fpResourceHandler(curDir + L'\\', curFile);
		} while (FindNextFile(findHandle, &fileData));
	} while (!dirNameVector.empty());
}

std::wstring CResourceStore::GetLastDirName(std::wstring const& fullPath)
{
	_size startPoint, endPoint;
	endPoint = fullPath.find_last_of('\\') - 1;
	startPoint = fullPath.find_last_of('\\', endPoint) + 1;

	return fullPath.substr(startPoint, ++endPoint - startPoint);
}

std::wstring CResourceStore::GetFileName(std::wstring const& fullPath)
{
	_size startPoint, endPoint;
	startPoint = fullPath.find_last_of('\\') + 1;
	endPoint = fullPath.find_last_of('.');

	return fullPath.substr(startPoint, endPoint - startPoint);
}
