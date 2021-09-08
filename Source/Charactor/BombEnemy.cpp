#include "BombEnemy.h"

BombEnemy::BombEnemy()
{
	model = new Model("Data/Model/obstacle/Cube.mdl");

	scale.x = scale.y = scale.z = 1.0f;

	// ���A�����ݒ�
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
	// ���͏����X�V
	UpdateVelocity(elapsedTime, KIND::BOMB_ENEMY);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);
	
	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();
}

void BombEnemy::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}
