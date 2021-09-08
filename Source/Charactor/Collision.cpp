#include "Collision.h"

// ���Ƌ��̌�������
bool Collision::IntersectSqhereVsSqhere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB)
{
	// A->B
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	
	//DirectX::XMVECTOR Vec		= DirectX::XMVectorSubtract(PositionB, PositionA);
	using namespace DirectX;
	DirectX::XMVECTOR Vec = PositionB - PositionA;
	
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// ��������
	float range = radiusA + radiusB;
	if (lengthSq > range * range)
	{
		return false; // �������Ă��Ȃ�
	}

	// A��B�������o��
	//Vec /= LengthSq;
	Vec = DirectX::XMVector3Normalize(Vec);
	Vec *= range;// ����range��A��B�x�N�g��
	PositionB = PositionA + Vec;

	DirectX::XMStoreFloat3(&outPositionB, PositionB);

	return true; // �������Ă���
}

// �~���Ɖ~���̌�������
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB)
{
	// A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}
	// A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (positionA.y + heightA < positionB.y )
	{
		return false;
	}

	//XZ���ʂł͈̔̓`�F�b�N
	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float dist = sqrtf(vx * vx + vz * vz);
	float range = radiusA + radiusB;

	if (dist > range)
	{
		return false; // �������Ă��Ȃ�
	}

	// A��B�������o��
	vx /= dist;
	vz /= dist;

	outPositionB.x = positionA.x + vx * range;
	outPositionB.y = positionB.y;
	outPositionB.z = positionA.z + vz * range;

	return true;
}

// ���Ɖ~���̌�������
bool Collision::IntersectSqhereVsCylinder(
	const DirectX::XMFLOAT3& sqherePosition,
	float sqhereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition
)
{
	// A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
	if (sqherePosition.y > cylinderPosition.y + cylinderHeight)
	{
		return false;
	}
	// A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (sqherePosition.y + sqhereRadius < cylinderPosition.y)
	{
		return false;
	}

	//XZ���ʂł͈̔̓`�F�b�N
	float vx = cylinderPosition.x - sqherePosition.x;
	float vz = cylinderPosition.z - sqherePosition.z;
	float dist = sqrtf(vx * vx + vz * vz);
	float range = sqhereRadius + cylinderRadius;

	if (dist > range)
	{
		return false; // �������Ă��Ȃ�
	}

	// A��B�������o��
	vx /= dist;
	vz /= dist;

	outCylinderPosition.x = sqherePosition.x + vx * range;
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = sqherePosition.z + vz * range;
	
	return true;
	
}

// �I�̓����蔻��
bool Collision::IntersectSqhereVsXYCircle(
	const DirectX::XMFLOAT3& sqherePosition, // projectile
	float sqhereRadius,
	const DirectX::XMFLOAT3& XYSquarePosition, // obstacle
	float xysquareRadius,
	float xysquareHeight,
	DirectX::XMFLOAT3& outXYSquarePosition)
{

	// A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
	if (sqherePosition.y > XYSquarePosition.y + xysquareHeight)
	{
		return false;
	}
	// A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (sqherePosition.y < XYSquarePosition.y)
	{
		return false;
	}

	// A�̍��W��B�̉E�[���E�Ȃ瓖�����Ă��Ȃ�
	if (sqherePosition.x > XYSquarePosition.x + xysquareRadius)
	{
		return false;
	}
	// A�̍��W��B�̍��[��荶�Ȃ瓖�����Ă��Ȃ�
	if (sqherePosition.x < XYSquarePosition.x - xysquareRadius)
	{
		return false;
	}

	//XZ���ʂł͈̔̓`�F�b�N
	float vx = XYSquarePosition.x - sqherePosition.x;
	float vz = XYSquarePosition.z - sqherePosition.z;
	float dist = sqrtf(vx * vx + vz * vz);
	float range = sqhereRadius + xysquareRadius;

	if (dist > range)
	{
		return false; // �������Ă��Ȃ�
	}

	// A��B�������o��
	vx /= dist;
	vz /= dist;

	outXYSquarePosition.x = sqherePosition.x + vx * range;
	outXYSquarePosition.y = XYSquarePosition.y;
	outXYSquarePosition.z = sqherePosition.z + vz * range;

	return true;

}