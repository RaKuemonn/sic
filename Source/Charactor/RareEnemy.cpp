#include "RareEnemy.h"
#include "EnemyManager.h"

RareEnemy::RareEnemy()
{
	model = new Model("Data/Model/Test/test_enemy3.mdl");

	enemy_tag = RARE;

	scale.x = scale.y = scale.z = 3.0f;

	// 幅、高さ設定
	radius = 1.0f;
	height = 2.0f;

	// 吸い込み用のパラメータ設定
	{

		float total_scale_value = (scale.x + scale.y + scale.z);


		// 吸い込めるようになる最低限のスケール
		par.enough_total_scale_value = total_scale_value * 0.87f;

		// 消化できるようになる最低限のスケール
		// ※ありえない数値を入れておく
		par.can_be_digestion_total_scale_value = FLT_MAX;
	}
}

RareEnemy::~RareEnemy()
{
	delete model;
	model = NULL;
}

void RareEnemy::Update(float elapsedTime)
{
	// 速力処理更新
	UpdateVelocity(elapsedTime, KIND::RARE_ENEMY);

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform(transform);

}

void RareEnemy::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

DirectX::XMFLOAT3 RareEnemy::inhaled()
{
	constexpr float STAGE_1_SCALING_VALUE = 0.5f;

	switch (1 /* stage */)
	{
	case 1:
		Is_inhaling = true;
		EnemyManager::Instance().Remove(this);
		par.scaling_value = { STAGE_1_SCALING_VALUE * scale.x, STAGE_1_SCALING_VALUE * scale.y, STAGE_1_SCALING_VALUE * scale.z };
		return par.scaling_value;
		break;
	default:
		break;
	}
}