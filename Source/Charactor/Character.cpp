#include "Character.h"


// 行列更新処理
void Character::UpdateTransform()
{
	// スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// 位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// ３つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

// 移動処理
void Character::Move(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	position.x += vx * speed;
	position.z += vz * speed;
}

// 旋回処理
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// 進行ベクトルがゼロベクトルの場合は処理する必要なし
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.0001f) return;

	// 進行ベクトルを単位ベクトル化
	vx /= length;
	vz /= length;

	// 自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// 回転角を求めるため、２つの単位ベクトルの内積を計算する
	// A.x * B.x + A.z * B.z
	float dot = frontX * vx + frontZ * vz; // 内積
	float rot = 1.0 - dot; // 補正値

	// 内積値は-1.0~1.0で表現されており、２つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	if (rot < speed) speed = rot;


	// 左右判定を行うために２つの単位ベクトルの外積を計算する
	// A.x * B.y - A.z * B.x
	float cross = frontX * vz - frontZ * vx;

	// 2Dの外積値が正の場合か負の場合によって左右判定が行える


	// 左右判定を行うことによって左右回転を選択する
	if (cross <= 0.0f) // 右に旋回
	{
		angle.y += speed;
	}
	else if (cross > 0.0f) // 左に旋回
	{
		angle.y -= speed;
	}

}

// ジャンプ処理
//void Character::Jump(float speed)
//{
//	// 上方向の力を設定
//	velocity.y = speed;
//}

// 速力処理更新
void Character::UpdateVelocity(float elapsedTime, int kind)
{
	// 経過フレーム
	float elapsedFrame = 60.0f * elapsedTime;

	if (kind == KIND::PLAYER)
	{
		// 重力処理
		velocity.y += gravity * elapsedFrame;
	}

	// 移動処理
	position.y += velocity.y * elapsedTime;

	// 地面判定
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