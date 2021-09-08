#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

void EnemyManager::Update(float elapsedTime)
{
	for (auto& enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

}

void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (auto& enemy : enemies)
	{
		enemy->Render(context, shader);
	}
}

// デバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
	for (auto& enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

// エネミー登録
void EnemyManager::Register(Enemy* enemy, int tag)
{
	enemies.emplace_back(enemy);
	enemy->objtag = tag;
}
void EnemyManager::Clear()
{
	// エネミー終了化
	for (Enemy* enemy : enemies)
	{
		//enemy->~Enemy();
		if (enemy != NULL)
		{
			enemies.clear();
			delete enemy;
		}
	}
}

// エネミー同士の衝突判定
//void EnemyManager::CollisionEnemyVsEnemies()
//{
//	size_t enemyCount = enemies.size();
//	for (int i = 0; i < enemyCount; ++i)
//	{
//		Enemy* enemyA = enemies.at(i);
//		for (int j = i + 1; j < enemyCount; ++j)
//		{
//			Enemy* enemyB = enemies.at(j);
//
//			DirectX::XMFLOAT3 outPosition;
//			if (Collision::IntersectCylinderVsCylinder(
//				enemyA->GetPosition(),
//				enemyA->GetRadius(),
//				enemyA->GetHeight(),
//				enemyB->GetPosition(),
//				enemyB->GetRadius(),
//				enemyB->GetHeight(),
//				outPosition
//			))
//			{
//				//押し出し後の位置設定
//				enemyB->SetPosition(outPosition);
//			}
//			
//		}
//	}
//}
