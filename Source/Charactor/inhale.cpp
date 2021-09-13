

#include "inhale.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "common.h"
#include "easy_math.h"
#include "Player.h"
#include "EnemyManager.h"
#include "collision.h"
#include "gameSystem.h"
#include "CameraController.h"



Inhale::Inhale(Player* player_) : player(player_)
{
	// スケールに合わせたポジション
	nozzle.position = float3SUM(player->GetPosition(), float3Scaling(player->GetFront(), player->GetScaleManager()->TotalScaleValue()));

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
	float average_scale_value = player->GetScaleManager()->TotalScaleValue() / 3 /* 3次元 x,y,z */;

	// ノズルが本体からどれぐらい先にあるのかを計算
	float scale_factor = player->GetScaleManager()->GetScaleZ();
	nozzle.position = float3SUM(player->GetPosition(), float3Scaling(player->GetFront(), scale_factor));

	// 当たり判定の大きさも変更する
	radius = average_scale_value;
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
			// 吸い込めるでかさに達していなければ　吸い込まず弾き返す
			if (player->GetScaleManager()->TotalScaleValue() < enemy->EnoughTotalScaleValue())
			{
				KnockBack(player->GetPosition(), enemy->GetPosition());
				continue;
			}


			// スケールの加算と吸い込み処理の確定
			DirectX::XMFLOAT3 add_scale = enemy->inhaled();

			float factor = 1.0f;
			if (player->GetScaleManager()->TotalScaleValue() >= enemy->CanBeDigestionTotalScaleValue()) factor *= -1.0f;

			add_scale = float3Scaling(add_scale, factor);

			player->GetScaleManager()->AddScaleX(add_scale.x);
			player->GetScaleManager()->AddScaleY(add_scale.y);
			player->GetScaleManager()->AddScaleZ(add_scale.z);
			player->GetScaleManager()->SubtractScaleX(add_scale.x);
			player->GetScaleManager()->SubtractScaleY(add_scale.y);
			player->GetScaleManager()->SubtractScaleZ(add_scale.z);
			


			float sum_add_scale = add_scale.x + add_scale.y + add_scale.z;

			// TODO: スコア加算の仮実装, 変更する
			GameSystem::Instance().AddScore(static_cast<int>(sum_add_scale * 10.0f));
			

			break;
		}
	}
}



void Inhale::KnockBack(DirectX::XMFLOAT3 pos_a, DirectX::XMFLOAT3 pos_b)
{
	DirectX::XMFLOAT3 impact = {};
	DirectX::XMStoreFloat3(&impact, DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&pos_a), DirectX::XMLoadFloat3(&pos_b))), 25.0f));

	player->AddImpact(impact);

	CameraController::Instance()->SetCameraShake();
}