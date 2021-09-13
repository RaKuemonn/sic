

#include "inhale.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "common.h"
#include "easy_math.h"
#include "Player.h"
#include "EnemyManager.h"
#include "collision.h"
#include "gameSystem.h"



Inhale::Inhale(Player* player_) : player(player_)
{
	// �X�P�[���ɍ��킹���|�W�V����
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

	// �Փ˔���p�̃f�o�b�O����`��

	if (is_during_inhale == false)	// �F�ŕ\��
		debugRenderer->DrawSphere(nozzle.position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));

	else							// �ԐF�ŕ\��
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
	float average_scale_value = player->GetScaleManager()->TotalScaleValue() / 3 /* 3���� x,y,z */;

	// �m�Y�����{�̂���ǂꂮ�炢��ɂ���̂����v�Z
	float scale_factor = player->GetScaleManager()->GetScaleZ();
	nozzle.position = float3SUM(player->GetPosition(), float3Scaling(player->GetFront(), scale_factor));

	// �����蔻��̑傫�����ύX����
	radius = average_scale_value;
}


void Inhale::Collision()
{
	if (is_during_inhale == false) return;
	// �z�����ݓ��쒆�Ȃ���s


	EnemyManager& enemyManager = EnemyManager::Instance();

	int enemyCount = enemyManager.GetEnemyCount();

	for (int j = 0; j < enemyCount; ++j)
	{
		Enemy* enemy = enemyManager.GetEnemy(j);

		// �Փˏ���
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
			// �z�����߂�ł����ɒB���Ă��Ȃ���΁@�z�����܂��e���Ԃ�
			if (player->GetScaleManager()->TotalScaleValue() < enemy->EnoughTotalScaleValue())
			{
				KnockBack(player->GetPosition(), enemy->GetPosition());
				continue;
			}


			// �X�P�[���̉��Z�Ƌz�����ݏ����̊m��
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

			// TODO: �X�R�A���Z�̉�����, �ύX����
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
}