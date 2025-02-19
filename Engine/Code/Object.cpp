#include "EngineStdafx.h"
#include "Object.h"
#include "Layer.h"
#include "ObjectFactory.h"
USING(Engine)

CObject::CObject(void)
{
}

CObject::CObject(const CObject & other)
{
}

CObject::~CObject(void)
{
	OnDestroy();
}

void CObject::Awake(void)
{
	m_isAwaked		= true;

	if(m_objectKey == L"")
		m_objectKey		= GetCurClassName(this);
	m_spTransform	= AddComponent<Engine::CTransformC>();
}

void CObject::Start(void)
{
	m_isStarted		= true;
}

void CObject::FixedUpdate(void)
{
	for (auto& component : m_mComponents)
	{
		if (component.second->GetIsStarted() == false)
			continue;

		if (component.second->GetIsEnabled())
			component.second->FixedUpdate(component.second);
	}
}

void CObject::Update(void)
{
	for (auto& component : m_mComponents)
	{
		if (component.second->GetIsStarted() == false)
			component.second->Start(component.second);

		if(component.second->GetIsEnabled())
			component.second->Update(component.second);
	}
}

void CObject::LateUpdate(void)
{
	for (auto& component : m_mComponents)
	{
		if (component.second->GetIsStarted() == false)
			continue;

		if (component.second->GetIsEnabled())
			component.second->LateUpdate(component.second);
	}
}


void CObject::OnDestroy(void)
{
	for (auto& component : m_mComponents)
	{
		component.second->SetOwner(nullptr);
		component.second.reset();
	}

	m_mComponents.clear();
}

void CObject::OnEnable(void)
{
	m_isEnabled = true;
	for (auto& component : m_mComponents)
	{
		if(component.second->GetIsEnabled() == false)
			component.second->OnEnable();
	}
}


void CObject::OnDisable(void)
{
	m_isEnabled = false;
	for (auto& component : m_mComponents)
	{
		if(component.second->GetIsEnabled() == true)
			component.second->OnDisable();
	}
}

void CObject::SetIsEnabled(_bool var)
{
	if (m_isEnabled == var)
		return;

	if (var == true)
		OnEnable();
	else
		OnDisable();
}

void CObject::InitClone(SP(CObject) spClone)
{
	spClone->SetIsClone(true);
	spClone->SetIsStatic(m_isStatic);

	spClone->SetIsAwaked(m_isAwaked);
	
	spClone->SetObjectKey(m_objectKey);
	spClone->SetDataID(m_dataID);
	spClone->SetLayerID(m_layerID);
	spClone->SetIsEnabled(m_isEnabled);
	
	for (auto& component : m_mComponents)
	{
		spClone->CopyComponentToClone(component.second);
	}

	spClone->SetAddExtra(true);
}