#include "RareEnemy.h"
#include "EnemyManager.h"

RareEnemy::RareEnemy()
{
	model = new Model("Data/Model/Test/test_enemy3.mdl");

	enemy_tag = RARE;

	scale.x = scale.y = scale.z = 3.0f;

	// ���A�����ݒ�
	height = 2.0f;

	// �z�����ݗp�̃p�����[�^�ݒ�
	{

		float total_scale_value = (scale.x + scale.y + scale.z);

		// �����蔻��̔��a
		radius = total_scale_value / 3;

		// �z�����߂�悤�ɂȂ�Œ���̃X�P�[��
		par.enough_total_scale_value = total_scale_value * 0.87f;

		// �����ł���悤�ɂȂ�Œ���̃X�P�[��
		// �����肦�Ȃ����l�����Ă���
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
	// ���͏����X�V
	UpdateVelocity(elapsedTime, KIND::RARE_ENEMY);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
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