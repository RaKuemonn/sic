#include "Character.h"
#include "collision.h"
#include "stageManager.h"


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
void Character::Move(float vx, float vz, float speed)
{
    // 移動方向のベクトル設定
    moveVecX = vx;
    moveVecZ = vz;

    // 最大速度の設定
    maxMoveSpeed = speed;
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
    //　経過フレーム
    float elapsedFrame = 60.0f * elapsedTime;

    // 垂直速力
    UpdateVerticalVelocity(elapsedFrame, kind);

    // 水平速力
    UpdateHorizontalVelocity(elapsedFrame);


    // 垂直移動
    UpdateVerticalMove(elapsedTime);

    // 水平移動
    UpdateHorizontalMove(elapsedTime);
}

void Character::UpdateVerticalVelocity(float elapsedFrame, int kind)
{
    if (kind == KIND::PLAYER)
    {
        // 重力処理
        velocity.y += gravity * elapsedFrame;
    }

}


void Character::UpdateVerticalMove(float elapsedTime)
{

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


void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    // XZの力の強さ(速力)を算出
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

    // 速力がある時 (摩擦処理をする)
    if (length > 0.0f)
    {

        // 摩擦力
        float friction = this->friction * elapsedFrame;

        // 空中なら摩擦力を減らす
        if (!IsGround()) friction *= 0.7f; // ３割減

        // 摩擦による減速処理 (速力が摩擦以上)
        if (length > friction)
        {
            DirectX::XMVECTOR n_Velo = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity));


            DirectX::XMFLOAT3 normalized_velo;
            DirectX::XMStoreFloat3(&normalized_velo, n_Velo);

            float vx = normalized_velo.x;
            float vz = normalized_velo.z;

            velocity.x = velocity.x - vx * friction;
            velocity.z = velocity.z - vz * friction;

        }

        // 速力をなしにする (速力が摩擦以下)
        else
        {

            velocity.x = 0.0f;
            velocity.z = 0.0f;

        }

    }


    // 速力が最大速度以内のとき　(加速処理をする)
    if (length <= maxMoveSpeed)
    {
        // 移動ベクトルの算出
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

        //移動ベクトルがある時 (加速処理をする)
        if (moveVecLength > 0.0f)
        {
            // 加速力
            float acceleration = this->acceleration * elapsedFrame;

            // 空中なら加速力を減らす
            if (!IsGround()) acceleration *= 0.5f; // 半減

            // 加速処理  
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;


            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

            // 最大速度の制限
            if (length > maxMoveSpeed)
            {
                    // Xの最大速度よりXの速力(絶対値)が大きければ
                if (fabsf(velocity.x) > fabsf(moveVecX * maxMoveSpeed))
                    velocity.x = moveVecX * maxMoveSpeed;

                // Zの最大速度よりZの速力(絶対値)が大きければ
                if (fabsf(velocity.z) > fabsf(moveVecZ * maxMoveSpeed))
                    velocity.z = moveVecZ * maxMoveSpeed;

            }

        }

    }

    // 移動ベクトルのリセット
    moveVecX = 0.0f;
    moveVecZ = 0.0f;

}


void Character::UpdateHorizontalMove(float elapsedTime)
{

    // 水平速力
    float veclocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (veclocityLengthXZ > 0.0f)
    {
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

        HitResult hit;
        //壁があれば
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            // 壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);


            // 壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
            Normal = DirectX::XMVector3Normalize(Normal);
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

            DirectX::XMFLOAT3 wall_vec;
            DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorSubtract(Vec, DirectX::XMVectorMultiply(Dot, Normal)));



            start = { position.x,position.y + stepOffset,position.z };
            end = { position.x + wall_vec.x , position.y + stepOffset, position.z + wall_vec.z };

            // 壁ずり後の位置が壁に
            // めり込んでいれば
            if (StageManager::Instance().RayCast(start, end, hit))
            {
                // 更新しない

            }

            // めり込んでいなければ
            else
            {
                // 位置の更新
                position.x += wall_vec.x;
                position.z += wall_vec.z;
            }


        }

        //壁がなければ
        else
        {
            // 通常の位置の更新
            position.x += mx;
            position.z += mz;
        }
    }

}