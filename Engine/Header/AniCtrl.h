#ifndef ANICTRL_H
#define ANICTRL_H


BEGIN(Engine)
class ENGINE_DLL CAniCtrl final
{
private:
	explicit					CAniCtrl			(LPD3DXANIMATIONCONTROLLER pAniCtrl);
							   ~CAniCtrl			(void);

public:
	static			CAniCtrl*	Create				(LPD3DXANIMATIONCONTROLLER pAniCtrl);
					void		Free				(void);

					CAniCtrl*	MakeClone			(void);

					void		Awake				(void);
					void		OnDestroy			(void);

					void		OnEnable			(void);
					void		OnDisable			(void);

public:
					void		ChangeAniSet		(_uint index, 
													 _bool fixTillEnd = false, 
													 _double smoothTime = 0.25,
													 _float changeWeight = 0.9f);
					void		ChangeAniSet		(std::string name, 
													 _bool fixTillEnd = false, 
													 _double smoothTime = 0.25,
													 _float changeWeight = 0.9f);

					void		Play				(void);
					_bool		IsItEnd				(void);

private:
					_uint		FindIndexByName		(std::string const& name, LPD3DXANIMATIONSET pAS);
private:
	GETTOR			(LPD3DXANIMATIONCONTROLLER,		m_pAniCtrl,		nullptr,	AniCtrl)
	GETTOR			(_uint,							m_curTrack,		0,			CurTrack)
	GETTOR			(_uint,							m_curIndex,		0,			CurIndex)
	GETTOR			(_float,						m_timer,		0.f,		Timer)
	GETTOR_SETTOR	(_float,						m_speed,		1.f,		Speed)
	GETTOR			(_double,						m_period,		0,			Period)
	GETTOR_SETTOR	(_bool,							m_replay,		true,		Replay)
	
	GETTOR_SETTOR	(_bool,							m_fixTillEnd,	false,		FixTillEnd)
};
END

#endif