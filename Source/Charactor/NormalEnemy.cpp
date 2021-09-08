#include "NormalEnemy.h"

NormalEnemy::NormalEnemy()
{
	model = new Model("Data/Model/target/target_demo.mdl");

	scale.x = scale.y = scale.z = 1.0f;

	// ���A�����ݒ�
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
	// ���͏����X�V
	UpdateVelocity(elapsedTime, KIND::NORMAL_ENEMY);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

}

void NormalEnemy::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}