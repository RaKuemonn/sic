#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"

//#include "Audio/Audio.h"

//#define HIT_VOLUME (1.0f)
//#define SHOT_VOLUME (1.0f)

class Player : public Character
{
public:
	Player();
	~Player() override;

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// �ړ����͒l����
	void InputMove(float elapsedTime);

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI(DirectX::XMFLOAT3 wind_velocity);

protected:

	// �ړ�����
	void Move(float elapsedTime, float vx, float vz, float speed);

private:
	DirectX::XMFLOAT3 GetMoveVec() const;

private:
	Model* model = nullptr;

	float	moveSpeed = 50.0f;
	float	turnSpeed = DirectX::XMConvertToRadians(720);

	float	velocity_calc = 0;

public:

	/*std::unique_ptr<AudioSource> Shot;
	std::unique_ptr<AudioSource> Hit;*/
};