#pragma once

#include <DirectXMath.h>

// �L�����N�^�[	
class Character
{
public:
	Character() {}
	virtual ~Character() {}

	// �s��X�V����
	void UpdateTransform();

	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// ���a�擾
	float GetRadius() const { return radius; }

	// �����擾
	float GetHeight() const { return height; }

	// �n�ʂɐڒn���Ă��邩
	bool IsGround() const { return isGround; }

protected:

	// �ړ�����
	void Move(float elapsedTime, float vx, float vz, float speed);

	// ���񏈗�
	void Turn(float elapsedTime, float vx, float vz, float speed);

	// �W�����v����
	//void Jump(float speed);

	// ���͏����X�V
	void UpdateVelocity(float elapsedTime, int kind);

protected:
	DirectX::XMFLOAT3	position = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	
	float				radius = 0.5f;

	float				gravity = -1.0f;
	DirectX::XMFLOAT3	velocity = { 0,0,0 };

	bool				isGround = false;

	float				height = 2.0f;

	enum KIND{
		PLAYER,		// �v���C���[
		NORMAL_ENEMY, // �m�[�}�����u
		RARE_ENEMY,	// ���A���u
		BOMB_ENEMY,	// �f�����b�g���u
	};
};