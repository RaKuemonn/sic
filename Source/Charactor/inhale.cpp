

#include "inhale.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "common.h"
#include "easy_math.h"
#include "Player.h"
#include "EnemyManager.h"
#include "collision.h"



Inhale::Inhale(Player* player_) : player(player_)
{
	float test_scale = 2.0f;
	// スケールに合わせたポジション
	nozzle.position = float3SUM(player->GetPosition(), float3Scaling(player->GetFront(), test_scale /*TODO: プレイヤーのスケール値にする*/));

}

void Inhale::Update(float elapsedTime)
{
	UpdateNozzlePosition();

	Collision();
}


void Inhale::DebugRender()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画

	if (is_during_inhale == false)	// 青色で表示
		debugRenderer->DrawSphere(nozzle.position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));

	else							// 赤色で表示
		debugRenderer->DrawSphere(nozzle.position, radius, DirectX::XMFLOAT4(1, 0, 0, 1));
}

void Inhale::InputInhale()
{
    is_during_inhale = false;

	if (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_A /*Z key*/)
	{
		is_during_inhale = true;
	}
}


void Inhale::UpdateNozzlePosition()
{
	float test_scale = 2.0f;
	nozzle.position = float3SUM(player->GetPosition(), float3Scaling(player->GetFront(), test_scale /*TODO: プレイヤーのスケール値にする*/));
}


void Inhale::Collision()
{
	if (is_during_inhale == false) return;
	// 吸い込み動作中なら実行


	EnemyManager& enemyManager = EnemyManager::Instance();

	int enemyCount = enemyManager.GetEnemyCount();

	for (int j = 0; j < enemyCount; ++j)
	{
		Enemy* enemy = enemyManager.GetEnemy(j);

		// 衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision3D::BallVsBallAndExtrusion/*collision::IntersectSqhereVsXYCircle*/(
			nozzle.position,
			radius,
			enemy->GetPosition(),
			enemy->GetRadius(),
			/*enemy->GetHeight(),*/
			outPosition
		))
		{
			float add_scale = enemy->inhaled();
			player->AddScaleY(add_scale);

			break;

			//switch (enemy->enemy_tag)
			//{
			//case Enemy::ENEMYTAG::NORMAL:
			//	//Hit->Play(false, HIT_VOLUME);
			//	break;
			//case Enemy::ENEMYTAG::RARE:
			//	break;
			//case Enemy::ENEMYTAG::BOMB:
			//	break;
			//default:
			//	break;
			//}
		}
	}
}