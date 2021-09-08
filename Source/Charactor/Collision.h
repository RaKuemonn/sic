#pragma once

#include <DirectXMath.h>

// ƒRƒŠƒWƒ‡ƒ“
class Collision
{
public:
	// ‹…‚Æ‹…‚ÌŒğ·”»’è
	static bool IntersectSqhereVsSqhere(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionB
	);

	// ‰~’Œ‚Æ‰~’Œ‚ÌŒğ·”»’è
	static bool IntersectCylinderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3& outPositionB
	);

	// ‹…‚Æ‰~’Œ‚ÌŒğ·”»’è
	static bool IntersectSqhereVsCylinder(
		const DirectX::XMFLOAT3& sqherePosition,
		float sqhereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition
	);

	// “I‚Ì“–‚½‚è”»’è
	static bool Collision::IntersectSqhereVsXYCircle(
		const DirectX::XMFLOAT3& sqherePosition,
		float sqhereRadius,
		const DirectX::XMFLOAT3& XYSquarePosition,
		float xysquareRadius,
		float xysquareHeight,
		DirectX::XMFLOAT3& outXYSquarePosition
	);


private:

};