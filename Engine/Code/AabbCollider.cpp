#include "EngineStdafx.h"
#include "AabbCollider.h"

USING(Engine)
CAabbCollider::CAabbCollider()
{
}


CAabbCollider::~CAabbCollider()
{
}

CAabbCollider * CAabbCollider::Create(_float3 size, _float3 offset)
{
	CAabbCollider*	pAabb = new CAabbCollider;
	pAabb->SetOffsetOrigin(offset);
	pAabb->SetOffset(offset);
	pAabb->SetSize(size);
	pAabb->SetHalfSize(size / 2.f);
	pAabb->Awake();

	return pAabb;
}

CCollider * CAabbCollider::MakeClone(CCollisionC * pCC)
{
	CAabbCollider* pAabbClone = new CAabbCollider;
	pAabbClone->SetOffsetOrigin(m_offsetOrigin);
	pAabbClone->SetOffset(m_offsetOrigin);
	pAabbClone->SetSize(m_size);
	pAabbClone->SetHalfSize(m_halfSize);

	pAabbClone->SetRadiusBS(m_radiusBS);

	pAabbClone->SetColliderType(m_colliderType);

	pAabbClone->SetOwner(pCC);

	return pAabbClone;
}

void CAabbCollider::Awake(void)
{
	__super::Awake();
	m_colliderType = (_uint)EColliderType::AABB;
}

void CAabbCollider::OnDestroy(void)
{
}

void CAabbCollider::OnEnable(void)
{
}

void CAabbCollider::OnDisable(void)
{
}

void CAabbCollider::UpdatePosition(void)
{
	__super::UpdatePosition();
}

_float CAabbCollider::SqDistFromPoint(_float3 const& point)
{
	_float sqDist = 0.f;
	
	_float3 minPos = m_pOwner->GetTransform()->GetPosition() + m_offset - m_halfSize;
	_float3 maxPos = m_pOwner->GetTransform()->GetPosition() + m_offset + m_halfSize;

	for (int i = 0; i < 3; ++i)
	{
		if (point[i] < minPos[i]) sqDist += (minPos[i] - point[i]) * (minPos[i] - point[i]);
		if (point[i] > maxPos[i]) sqDist += (point[i] - maxPos[i]) * (point[i] - maxPos[i]);
	}

	return sqDist;
}

_float3 CAabbCollider::ClosestFromPoint(_float3 const& point)
{
	_float3 closestPoint = ZERO_VECTOR;

	_float3 minPos = m_pOwner->GetTransform()->GetPosition() + m_offset - m_halfSize;
	_float3 maxPos = m_pOwner->GetTransform()->GetPosition() + m_offset + m_halfSize;

	for (int i = 0; i < 3; ++i)
	{
		if (point[i] < minPos[i])		closestPoint[i] = minPos[i];
		else if (point[i] > maxPos[i])	closestPoint[i] = maxPos[i];
		else							closestPoint[i] = point[i];
		
	}

	return closestPoint;
}

_float3 CAabbCollider::SurfacePoint(_float3 const& dir)
{
	SP(CTransformC) spTransform = m_pOwner->GetTransform();
	_float3 hitPoint;
	_float3 proportion(abs(dir.x / m_halfSize.x), abs(dir.y / m_halfSize.y), abs(dir.z / m_halfSize.z));
	if (proportion.x > proportion.y && proportion.x > proportion.z)
		hitPoint = spTransform->GetPosition() + m_offset + dir / proportion.x;
	else if (proportion.y > proportion.z)
		hitPoint = spTransform->GetPosition() + m_offset + dir / proportion.y;
	else
		hitPoint = spTransform->GetPosition() + m_offset + dir / proportion.z;

	return hitPoint;
}

_float3 CAabbCollider::SurfacePointFromInside(_float3 const & dir, _float3 const & pos)
{
	SP(CTransformC) spTransform = m_pOwner->GetTransform();
	_float3 hitPoint;

	_float xDet = (m_halfSize.x - (abs(pos.x - (spTransform->GetPosition().x + m_offset.x))));
	_float yDet = (m_halfSize.y - (abs(pos.y - (spTransform->GetPosition().y + m_offset.y))));
	_float zDet = (m_halfSize.z - (abs(pos.z - (spTransform->GetPosition().z + m_offset.z))));

	if (xDet == 0 || yDet == 0 || zDet == 0)
		return pos;

	_float3 proportion(abs(dir.x / xDet), 
					   abs(dir.y / yDet), 
					   abs(dir.z / zDet));

	if (proportion.x > proportion.y && proportion.x > proportion.z)
		hitPoint = pos + dir / proportion.x;
	else if (proportion.y > proportion.z)
		hitPoint = pos + dir / proportion.y;
	else
		hitPoint = pos + dir / proportion.z;

	return hitPoint;
}

void CAabbCollider::MakeBS(void)
{
	_float3 minPos = m_offsetOrigin - m_halfSize;
	_float3 maxPos = m_offsetOrigin + m_halfSize;

	m_radiusBS = D3DXVec3Length(&(maxPos - minPos)) / 2.f;
}
