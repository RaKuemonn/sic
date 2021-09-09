#pragma once

#include <DirectXMath.h>

// キャラクター	
class Character
{
public:
	Character() {}
	virtual ~Character() {}

	// 行列更新処理
	void UpdateTransform();

	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// 半径取得
	float GetRadius() const { return radius; }

	// 高さ取得
	float GetHeight() const { return height; }

	// 地面に接地しているか
	bool IsGround() const { return isGround; }

protected:

	// 移動処理
	void Move(float elapsedTime, float vx, float vz, float speed);

	// 旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);

	// ジャンプ処理
	//void Jump(float speed);

	// 速力処理更新
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
		PLAYER,		// プレイヤー
		NORMAL_ENEMY, // ノーマルモブ
		RARE_ENEMY,	// レアモブ
		BOMB_ENEMY,	// デメリットモブ
	};

public:
	struct Parameter
	{
		float scaling_value = 0; // 吸い込んだ物によって＋or－
	}par;
};