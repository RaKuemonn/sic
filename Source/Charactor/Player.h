#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "inhale.h"
#include "Actor/scaleManager.h"

//#include "Audio/Audio.h"

//#define HIT_VOLUME (1.0f)
//#define SHOT_VOLUME (1.0f)

class Player : public Character
{
public:
	Player();
	~Player() override;

	void Update(float elapsedTime, bool explaining = false);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// デバッグ用GUI描画
	void DrawDebugGUI();

public:	// Get関数
	DirectX::XMFLOAT3 GetFront() { return ConvertToIdentityVec(transform._31, transform._32, transform._33); }
	DirectX::XMFLOAT3 GetRight() { return ConvertToIdentityVec(transform._11, transform._12, transform._13); }
	DirectX::XMFLOAT3 GetVelocity() { return velocity; }
	ScaleManager* GetScaleManager() { return scale_manager; }
	
public: // Set関数


private:

	void Input(float elapsedTime);

	// 移動入力値処理
	void InputMove(float elapsedTime);

	void Turn(float elapsedTime, float vx, float vz, float speed)override;

	void InputInhale();


private:
	DirectX::XMFLOAT3 GetMoveVec() const;
	DirectX::XMFLOAT3 ConvertToIdentityVec(float x, float y, float z)
	{
		DirectX::XMFLOAT3 identity_vec = {};
		DirectX::XMStoreFloat3(&identity_vec, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z))));
		return identity_vec;
	}

private:
	Model* model = nullptr;

	float	moveSpeed = 25.0f;
	float	turnSpeed = DirectX::XMConvertToRadians(720);

	Inhale* inhale = nullptr;				// 吸い込み関係の処理クラス
	ScaleManager* scale_manager = nullptr;	// スケール値管理クラス

	//float	velocity_calc = 0;

public:
	/*std::unique_ptr<AudioSource> Shot;
	std::unique_ptr<AudioSource> Hit;*/

};