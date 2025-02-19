#ifndef SCENE_H
#define SCENE_H

BEGIN(Engine)
class CLayer;
class CObject;
class ENGINE_DLL CScene abstract  
{
protected:
	explicit							CScene				(void);
	virtual							   ~CScene				(void);

public:
	virtual		void					Free				(void) PURE;
	virtual		void					Awake				(_int numOfLayers) PURE;
	virtual		void					Start				(void) PURE;
																   
	virtual		void					FixedUpdate			(void) PURE;
	virtual		void					Update				(void) PURE;
	virtual		void					LateUpdate			(void) PURE;	
																   
	virtual		void					OnDestroy			(void) PURE;
																   
	virtual		void					OnEnable			(void) PURE;
	virtual		void					OnDisable			(void) PURE;
				
				SP(CObject)				FindObjectByName		(std::wstring name);
				SP(CObject)				FindObjectWithKey		(std::wstring objectKey);
				_uint					FindObjectsWithKey		(std::wstring objectKey, 
																 std::vector<SP(CObject)>& gameObjects);


protected:												   
				void					InitLayers			(_int numOfLayers);
	virtual		void					InitPrototypes		(void) PURE;

	
protected:
	typedef std::vector<CLayer*> _LAYERS;
	GETTOR			(_LAYERS,					m_vLayers,			{},				Layers)

	GETTOR			(_int,						m_sceneID,			-1,				SceneID)

	GETTOR			(_bool,						m_isAwaked,			false,			IsAwaked)
	GETTOR			(_bool,						m_isStarted,		false,			IsStarted)
	GETTOR_SETTOR	(_bool,						m_enable,			true,			Enable)
	GETTOR_SETTOR	(_bool,						m_SceneEvent,		false,			SceneEvent)
};
END

#endif