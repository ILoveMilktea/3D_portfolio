#include "EngineStdafx.h"
#include "Camera.h"

#include "Frustum.h"

#include "InputManager.h"
#include "WndApp.h"

#include "FRC.h"
#include "DebugCollider.h"
#include "TextManager.h"


USING(Engine)
_uint CCamera::m_s_uniqueID = 0;

CCamera::CCamera()
{
}


CCamera::~CCamera()
{
	OnDestroy();
}

SP(CCamera) CCamera::Create(void)
{
	SP(CCamera) spInstance(new CCamera, SmartDeleter<CCamera>);
	spInstance->SetIsStatic(true);
	spInstance->Awake();

	return spInstance;
}

SP(CObject) CCamera::MakeClone(void)
{
	SP(CCamera) spClone(new CCamera, SmartDeleter<CCamera>);
	__super::InitClone(spClone);

	spClone->m_spTransform	= spClone->GetComponent<CTransformC>();
	spClone->m_spCollision	= spClone->GetComponent<CCollisionC>();

	return spClone;
}

void CCamera::Awake(void)
{
	__super::Awake();
	m_layerID	= (_int)ELayerID::Camera;
	m_dataID	= (_int)EDataID::Object;

	m_spCollision	= AddComponent<Engine::CCollisionC>();
	m_spCollision->SetResolveIn(false);
}

void CCamera::Start(void)
{
	__super::Start();
	m_pCamRayCollider = static_cast<Engine::CRayCollider*>(m_spCollision->GetColliders()[0]);
	m_pFrustum = CFrustum::Create(this);
}

void CCamera::FixedUpdate(void)
{
	__super::FixedUpdate();
	
}

void CCamera::Update(void)
{
	__super::Update();
	
	switch (m_mode)
	{
	case ECameraMode::Fixed:
		UpdateFixed();
		break;
		
	case ECameraMode::Edit:
		UpdateEdit();
		break;

	case ECameraMode::Free:
		UpdateFree();
		break;

	case ECameraMode::FPS:
		UpdateFPS();
		break;

	case ECameraMode::TPS:
		UpdateTPS();
		break;

	default:
		MSG_BOX(__FILE__, L"Camera mode enum is broken in Update");
		ABORT;
		break;
	}
}

void CCamera::LateUpdate(void)
{
	__super::LateUpdate();


	switch (m_mode)
	{
	case ECameraMode::Fixed:
		LateUpdateFixed();
		break;

	case ECameraMode::Edit:
		LateUpdateEdit();
		break;

	case ECameraMode::Free:
		LateUpdateFree();
		break;

	case ECameraMode::FPS:
		LateUpdateFPS();
		break;

	case ECameraMode::TPS:
		LateUpdateTPS();
		break;

	default:
		MSG_BOX(__FILE__, L"Camera mode enum is broken in LateUpdate");
		ABORT;
		break;
	}

	UpdateProjMat();

	
}

void CCamera::OnDestroy(void)
{
	__super::OnDestroy();
	
}

void CCamera::OnEnable(void)
{
	__super::OnEnable();
	
}

void CCamera::OnDisable(void)
{
	__super::OnDisable();
	
}

void CCamera::SetBasicName(void)
{
	m_name = m_objectKey + std::to_wstring(m_s_uniqueID++);
}

void CCamera::OnCollisionEnter(Engine::_CollisionInfo ci)
{
	m_spTransform->AddPosition(ci.normal * ci.penetLength);
	m_wallCollided = true;
	if (m_spTarget != nullptr)
	{
		_float3 targetPos = m_spTarget->GetTransform()->GetPosition() + m_targetOffset;
		_float3 targetDir = targetPos - m_spTransform->GetPosition();
		D3DXVec3Normalize(&targetDir, &targetDir);

		m_targetDist = D3DXVec3Length(&(targetPos - m_spTransform->GetPosition()));

		m_pCamRayCollider->SetLength(m_targetDist);
		m_pCamRayCollider->SetDirection(targetDir);
	}
}

void CCamera::OnCollisionStay(Engine::_CollisionInfo ci)
{
	m_spTransform->AddPosition(ci.normal * ci.penetLength);
	if (m_spTarget != nullptr)
	{
		_float3 targetPos = m_spTarget->GetTransform()->GetPosition() + m_targetOffset;
		_float3 targetDir = targetPos - m_spTransform->GetPosition();
		D3DXVec3Normalize(&targetDir, &targetDir);

		m_targetDist = D3DXVec3Length(&(targetPos - m_spTransform->GetPosition()));

		m_pCamRayCollider->SetLength(m_targetDist);
		m_pCamRayCollider->SetDirection(targetDir);
	}
}

void CCamera::OnCollisionExit(Engine::_CollisionInfo ci)
{
	m_wallCollided = false;
}

void CCamera::SetNear(_float nearPlane)
{
	m_near = nearPlane;
	m_projHasChanged = true;
}

void CCamera::SetFar(_float farPlane)
{
	m_far = farPlane;
	m_projHasChanged = true;
}

void CCamera::SetFOV(_float FOV)
{
	m_FOV = FOV;
	m_projHasChanged = true;
}

void CCamera::UpdateFixed(void)
{
	m_spTransform->SetPosition(m_fixedPos);
	m_spTransform->SetForward(m_fixedDir);
}

void CCamera::UpdateFree(void)
{
	CameraMove();
	CameraRotate();
}

void CCamera::UpdateEdit(void)
{
	CameraMove();
}

void CCamera::UpdateFPS(void)
{
	CameraRotate();
}

void CCamera::UpdateTPS(void)
{
	CameraRotate();

	if (m_wallCollided == false && m_targetDist < 3)
	{
		_float newTargetDist = m_targetDist + m_awaySpeed * GET_DT;

		newTargetDist = GET_MATH->Min(newTargetDist, 3);
		m_targetDist = newTargetDist;
	}

	//RayCollider 길이 업데이트
	m_pCamRayCollider->SetLength(m_targetDist);
}

void CCamera::LateUpdateFixed(void)
{
	D3DXMatrixLookAtLH(&m_viewMat, 
					   &m_spTransform->GetPosition(), 
					   &(m_spTransform->GetPosition() + m_spTransform->GetForward()),
					   &m_spTransform->GetUp());
}

void CCamera::LateUpdateFree(void)
{
	D3DXMatrixLookAtLH(&m_viewMat, 
					   &m_spTransform->GetPosition(), 
					   &(m_spTransform->GetPosition() + m_spTransform->GetForward()),
					   &m_spTransform->GetUp());
}

void CCamera::LateUpdateEdit(void)
{
	D3DXMatrixLookAtLH(&m_viewMat, 
					   &m_spTransform->GetPosition(), 
					   &(m_spTransform->GetPosition() + m_spTransform->GetForward()),
					   &m_spTransform->GetUp());
}

void CCamera::LateUpdateFPS(void)
{
	_mat rotationMatrix;
	SP(CTransformC) spTargetTransform = m_spTarget->GetTransform();
	m_spTransform->SetPosition(spTargetTransform->GetPosition());
	_float3 look = spTargetTransform->GetForward();

	/* 임의의 축 회전 */
	D3DXMatrixRotationAxis(&rotationMatrix, &RIGHT_VECTOR, m_lookAngleRight);
	D3DXVec3TransformNormal(&look, &look, &rotationMatrix);

	m_spTransform->SetForward(look);

	D3DXMatrixLookAtLH(&m_viewMat, 
					   &m_spTransform->GetPosition(), 
					   &(m_spTransform->GetPosition() + m_spTransform->GetForward()),
					   &m_spTransform->GetUp());
}

void CCamera::LateUpdateTPS(void)
{
	//TPS ViewMatrixUpdate
	_mat rotationMatrix;
	SP(CTransformC) spTargetTransform = m_spTarget->GetTransform();
	_float3 invLook = _float3(0, 0, -m_targetDist);

	/* 임의의 축 회전 */
	D3DXMatrixRotationAxis(&rotationMatrix, &RIGHT_VECTOR, m_lookAngleRight);
	D3DXVec3TransformNormal(&invLook, &invLook, &rotationMatrix);

	/* 임의의 축 회전2 */
	D3DXMatrixRotationAxis(&rotationMatrix, &UP_VECTOR, m_lookAngleUp);
	D3DXVec3TransformNormal(&invLook, &invLook, &rotationMatrix);

	m_spTransform->SetPosition(invLook + spTargetTransform->GetPosition() + m_targetOffset);
	m_spTransform->SetForward(-invLook);

	D3DXMatrixLookAtLH(&m_viewMat, 
					   &m_spTransform->GetPosition(), 
					   &(m_spTransform->GetPosition() + m_spTransform->GetForward()), 
					   &UP_VECTOR);	
}

void CCamera::UpdateProjMat(void)
{
	if(m_projHasChanged)
	{
		D3DXMatrixPerspectiveFovLH(&m_projMat, m_FOV, 
			   				      (_float)GET_WND_WIDTH / GET_WND_HEIGHT, 
							      m_near, m_far);

		m_projHasChanged = false;
	}
}

void CCamera::CameraRotate(void)
{
	CInputManager* pIM = CInputManager::GetInstance();

	POINT curPt;
	GetCursorPos(&curPt);
	POINT m_centerPt = { GET_WND_WIDTH >> 1, GET_WND_HEIGHT >> 1 };
	ClientToScreen(GET_HANDLE, &m_centerPt);
	SetCursorPos(m_centerPt.x, m_centerPt.y);

	if (curPt.x == m_centerPt.x && curPt.y == m_centerPt.y)
		return;


	switch (m_mode)
	{
	case ECameraMode::Free:
		m_spTransform->AddRotationX(D3DXToRadian((_float)(curPt.y - m_centerPt.y)
									* pIM->GetMouseSensitivity().y
									* GET_DT));
		m_spTransform->AddRotationY(D3DXToRadian((_float)(curPt.x - m_centerPt.x)
									* pIM->GetMouseSensitivity().x
									* GET_DT));
		break;
	case ECameraMode::TPS:
		m_lookAngleRight += D3DXToRadian((_float)(curPt.y - m_centerPt.y)
										* pIM->GetMouseSensitivity().y
										* GET_DT);
		m_lookAngleUp += D3DXToRadian((_float)(curPt.x - m_centerPt.x)
									 * pIM->GetMouseSensitivity().x
									 * GET_DT);

		m_lookAngleRight = GET_MATH->MinMax(m_lookAngleRight, -PI / 2.f, PI / 2.f);
		m_lookAngleUp = GET_MATH->RoundOffRange(m_lookAngleUp, 2 * PI);
		break;
	}
}

void CCamera::CameraMove(void)
{
	if (IMKEY_PRESS(KEY_D))
		m_spTransform->MoveRight(m_moveSpeed * GET_DT);
	if (IMKEY_PRESS(KEY_A))
		m_spTransform->MoveLeft(m_moveSpeed * GET_DT);
	if (IMKEY_PRESS(KEY_W))
		m_spTransform->MoveForward(m_moveSpeed * GET_DT);
	if (IMKEY_PRESS(KEY_S))
		m_spTransform->MoveBackward(m_moveSpeed * GET_DT);
	if (IMKEY_PRESS(KEY_SPACE))
		m_spTransform->MoveUp(m_moveSpeed * GET_DT);
	if (IMKEY_PRESS(KEY_CONTROL))
		m_spTransform->MoveDown(m_moveSpeed * GET_DT);
}
