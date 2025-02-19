#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "Engine.h"

BEGIN(Engine)
class CObject;
class CScene;
class ENGINE_DLL CObjectFactory final : public CEngine
{
	DECLARE_SINGLETON(CObjectFactory)
public:
			void			Awake				(void) override;
			void			Start				(void);

			void			OnDestroy			(void);

			void			OnEnable			(void);
			void			OnDisable			(void);

			HRESULT			AddPrototype		(SP(CObject) pPrototype);
			SP(CObject)		AddClone			(const std::wstring& protoObjectKey,
												 CScene* pScene = nullptr,
												 _bool isStatic = false,
												 const std::wstring& name = L"",
												 _int layerTag = UNDEFINED);

			void			ClearCurPrototype	(void);
			void			InitPrototypes		(void);
private:
	//여러개의 맵을 가지고 있을 것. 각 맵은 하나의 Scene을 의미. 프로토타입
	typedef std::unordered_map<std::wstring, SP(CObject)> _PROTOTYPES;
	GETTOR	(_PROTOTYPES,	m_mCurPrototypes,		{},		CurPrototypes)
	GETTOR	(_PROTOTYPES,	m_mStaticPrototypes,	{},		StaticPrototypes)
};
END
#endif