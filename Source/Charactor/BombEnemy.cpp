#include "BombEnemy.h"

#define STAGE_1_SCALING_VALUE (0.1f)

BombEnemy::BombEnemy()
{
	model = new Model("Data/Model/Test/test_enemy2.mdl");

	scale.x = scale.y = scale.z = 1.0f;

	// 幅、高さ設定
	radius = 1.0f;
	height = 2.0f;
}

BombEnemy::~BombEnemy()
{
	delete model;
	model = NULL;
}

void BombEnemy::Update(float elapsedTime)
{
	// 速力処理更新
	UpdateVelocity(elapsedTime, KIND::BOMB_ENEMY);

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform(transform);
	
	// オブジェクト行列を更新
	UpdateTransform();
}

void BombEnemy::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

float BombEnemy::inhale()
{
	switch ( 1 /* stage */)
	{
	case 1:
		return par.scaling_value = STAGE_1_SCALING_VALUE;
		break;
	default:
		break;
	}
}