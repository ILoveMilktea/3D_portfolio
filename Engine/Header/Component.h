#ifndef COMPONENT_H
#define COMPONENT_H

BEGIN(Engine)
class CObject;
class ENGINE_DLL CComponent abstract  
{
public:
	explicit	CComponent		(void);
	virtual	   ~CComponent		(void);

public:
	virtual		SP(CComponent)	MakeClone		(CObject* pObject) PURE;
				void			InitClone		(SP(CComponent) spClone, CObject* pOwner);

	virtual		void			Awake			(void) PURE;
	virtual		void			Start			(SP(CComponent) spThis) PURE;
																	    
	virtual		void			FixedUpdate		(SP(CComponent) spThis) PURE;
	virtual		void			Update			(SP(CComponent) spThis) PURE;
	virtual		void			LateUpdate		(SP(CComponent) spThis) PURE;

	virtual		void			OnDestroy		(void) PURE;
													   
	virtual		void			OnEnable		(void) PURE;
	virtual		void			OnDisable		(void) PURE;

public:
				void			SetIsEnabled	(_bool var);

protected:
	GETTOR_SETTOR	(CObject*,		m_pOwner,		nullptr,					Owner)
	
	
	GETTOR_SETTOR	(_bool,			m_isAwaked,		false,						IsAwaked)
	GETTOR_SETTOR	(_bool,			m_isStarted,	false,						IsStarted)
	GETTOR			(_bool,			m_isEnabled,	true,						IsEnabled)

	GETTOR_SETTOR	(_int,			m_dataID,		(_int)EDataID::Component,	DataID)
	GETTOR_SETTOR	(std::wstring,  m_name,			L"",						Name)

	GETTOR_SETTOR	(_int,			m_componentID,	-1,							ComponentID)
};
END

#endif // !COMPONENT_H