#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "inhale.h"

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

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// デバッグ用GUI描画
	void DrawDebugGUI();

public:	// Get関数
	DirectX::XMFLOAT3 GetFront() { return DirectX::XMFLOAT3(transform._31, transform._32, transform._33); }
	
public: // Set関数
	void AddScaleX(const float x) { scale.x += x; }
	void AddScaleY(const float y) { scale.y += y; }
	void AddScaleZ(const float z) { scale.z += z; }

private:

	void Input(float elapsedTime);

	// 移動入力値処理
	void InputMove(float elapsedTime);

	void Turn(float elapsedTime, float vx, float vz, float speed)override;

	void InputInhale();


private:
	DirectX::XMFLOAT3 GetMoveVec() const;

private:
	Model* model = nullptr;

	float	moveSpeed = 50.0f;
	float	turnSpeed = DirectX::XMConvertToRadians(720);

	Inhale* inhale = nullptr;		// 吸い込み関係の処理クラス

	//float	velocity_calc = 0;

public:
	/*std::unique_ptr<AudioSource> Shot;
	std::unique_ptr<AudioSource> Hit;*/

};