#include "NormalEnemy.h"

NormalEnemy::NormalEnemy()
{
	model = new Model("Data/Model/target/target_demo.mdl");

	scale.x = scale.y = scale.z = 1.0f;

	// 幅、高さ設定
	radius = 1.0f;
	height = 2.0f;
}

NormalEnemy::~NormalEnemy()
{
	delete model;
	model = NULL;
}

void NormalEnemy::Update(float elapsedTime)
{
	// 速力処理更新
	UpdateVelocity(elapsedTime, KIND::NORMAL_ENEMY);

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform(transform);

	// オブジェクト行列を更新
	UpdateTransform();

}

void NormalEnemy::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}