#include "Enemy.h"
#include "Graphics/Graphics.h"

// デバッグプリミティブ描画
void Enemy::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}