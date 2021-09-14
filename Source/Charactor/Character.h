#pragma once

#include <DirectXMath.h>



struct Parameter
{
	DirectX::XMFLOAT3 scaling_value = {0,0,0};		// 吸い込んだ物によって＋or－
	float enough_total_scale_value = 0.0f;			// 吸い込める最低ライン
	float can_be_digestion_total_scale_value = 0.0f;// 消化できる最低ライン
};


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
	void Move(float vx, float vz, float speed);

	// 旋回処理
	virtual void Turn(float elapsedTime, float vx, float vz, float speed);

	// ジャンプ処理
	//void Jump(float speed);

	// 速力処理更新
	virtual void UpdateVelocity(float elapsedTime, int kind);


protected:
	// 垂直速力の更新
	void UpdateVerticalVelocity(float elapsedFrame, int kind);
	// 垂直移動の更新
	void UpdateVerticalMove(float elapsedTime);
	// 水平速力の更新
	void UpdateHorizontalVelocity(float elapsedFrame);
	// 水平移動の更新
	void UpdateHorizontalMove(float elapsedTime);

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
	DirectX::XMFLOAT3 foot_pos = { 0,0,0 };
	
	float				radius = 1.5f;

	float				gravity = -1.0f;
	DirectX::XMFLOAT3	velocity = { 0,0,0 };

	bool				isGround = false;

	float				height = 2.0f;


	float friction = 0.5f;
	float acceleration = 1.0f;
	float maxMoveSpeed = 5.0f;
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;

	float stepOffset = 1.0f;

	enum KIND{
		PLAYER,		// プレイヤー
		NORMAL_ENEMY, // ノーマルモブ
		BOMB_ENEMY,	// デメリットモブ
		RARE_ENEMY,	// レアモブ
	};

public:
	Parameter par;
};