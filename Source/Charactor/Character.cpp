#include "Character.h"


// �s��X�V����
void Character::UpdateTransform()
{
	// �X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// ��]�s����쐬
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// �ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// �R�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;
	// �v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

// �ړ�����
void Character::Move(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	position.x += vx * speed;
	position.z += vz * speed;
}

// ���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.0001f) return;

	// �i�s�x�N�g����P�ʃx�N�g����
	vx /= length;
	vz /= length;

	// ���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	// A.x * B.x + A.z * B.z
	float dot = frontX * vx + frontZ * vz; // ����
	float rot = 1.0 - dot; // �␳�l

	// ���ϒl��-1.0~1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	if (rot < speed) speed = rot;


	// ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	// A.x * B.y - A.z * B.x
	float cross = frontX * vz - frontZ * vx;

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����


	// ���E������s�����Ƃɂ���č��E��]��I������
	if (cross <= 0.0f) // �E�ɐ���
	{
		angle.y += speed;
	}
	else if (cross > 0.0f) // ���ɐ���
	{
		angle.y -= speed;
	}

}

// �W�����v����
//void Character::Jump(float speed)
//{
//	// ������̗͂�ݒ�
//	velocity.y = speed;
//}

// ���͏����X�V
void Character::UpdateVelocity(float elapsedTime, int kind)
{
	// �o�߃t���[��
	float elapsedFrame = 60.0f * elapsedTime;

	if (kind == KIND::PLAYER)
	{
		// �d�͏���
		velocity.y += gravity * elapsedFrame;
	}

	// �ړ�����
	position.y += velocity.y * elapsedTime;

	// �n�ʔ���
	if (position.y < 0.0f)
	{
		position.y = 0.0f;
		velocity.y = 0.0f;

		isGround = true;
	}
	else
	{
		isGround = false;
	}
}