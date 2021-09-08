#include "Collision.h"

// 球と球の交差判定
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

	// 距離判定
	float range = radiusA + radiusB;
	if (lengthSq > range * range)
	{
		return false; // 当たっていない
	}

	// AがBを押し出す
	//Vec /= LengthSq;
	Vec = DirectX::XMVector3Normalize(Vec);
	Vec *= range;// 長さrangeのA→Bベクトル
	PositionB = PositionA + Vec;

	DirectX::XMStoreFloat3(&outPositionB, PositionB);

	return true; // 当たっている
}

// 円柱と円柱の交差判定
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB)
{
	// Aの足元がBの頭より上なら当たっていない
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}
	// Aの頭がBの足元より下なら当たっていない
	if (positionA.y + heightA < positionB.y )
	{
		return false;
	}

	//XZ平面での範囲チェック
	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float dist = sqrtf(vx * vx + vz * vz);
	float range = radiusA + radiusB;

	if (dist > range)
	{
		return false; // 当たっていない
	}

	// AがBを押し出す
	vx /= dist;
	vz /= dist;

	outPositionB.x = positionA.x + vx * range;
	outPositionB.y = positionB.y;
	outPositionB.z = positionA.z + vz * range;

	return true;
}

// 球と円柱の交差判定
bool Collision::IntersectSqhereVsCylinder(
	const DirectX::XMFLOAT3& sqherePosition,
	float sqhereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition
)
{
	// Aの足元がBの頭より上なら当たっていない
	if (sqherePosition.y > cylinderPosition.y + cylinderHeight)
	{
		return false;
	}
	// Aの頭がBの足元より下なら当たっていない
	if (sqherePosition.y + sqhereRadius < cylinderPosition.y)
	{
		return false;
	}

	//XZ平面での範囲チェック
	float vx = cylinderPosition.x - sqherePosition.x;
	float vz = cylinderPosition.z - sqherePosition.z;
	float dist = sqrtf(vx * vx + vz * vz);
	float range = sqhereRadius + cylinderRadius;

	if (dist > range)
	{
		return false; // 当たっていない
	}

	// AがBを押し出す
	vx /= dist;
	vz /= dist;

	outCylinderPosition.x = sqherePosition.x + vx * range;
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = sqherePosition.z + vz * range;
	
	return true;
	
}

// 的の当たり判定
bool Collision::IntersectSqhereVsXYCircle(
	const DirectX::XMFLOAT3& sqherePosition, // projectile
	float sqhereRadius,
	const DirectX::XMFLOAT3& XYSquarePosition, // obstacle
	float xysquareRadius,
	float xysquareHeight,
	DirectX::XMFLOAT3& outXYSquarePosition)
{

	// Aの足元がBの頭より上なら当たっていない
	if (sqherePosition.y > XYSquarePosition.y + xysquareHeight)
	{
		return false;
	}
	// Aの頭がBの足元より下なら当たっていない
	if (sqherePosition.y < XYSquarePosition.y)
	{
		return false;
	}

	// Aの座標がBの右端より右なら当たっていない
	if (sqherePosition.x > XYSquarePosition.x + xysquareRadius)
	{
		return false;
	}
	// Aの座標がBの左端より左なら当たっていない
	if (sqherePosition.x < XYSquarePosition.x - xysquareRadius)
	{
		return false;
	}

	//XZ平面での範囲チェック
	float vx = XYSquarePosition.x - sqherePosition.x;
	float vz = XYSquarePosition.z - sqherePosition.z;
	float dist = sqrtf(vx * vx + vz * vz);
	float range = sqhereRadius + xysquareRadius;

	if (dist > range)
	{
		return false; // 当たっていない
	}

	// AがBを押し出す
	vx /= dist;
	vz /= dist;

	outXYSquarePosition.x = sqherePosition.x + vx * range;
	outXYSquarePosition.y = XYSquarePosition.y;
	outXYSquarePosition.z = sqherePosition.z + vz * range;

	return true;

}
