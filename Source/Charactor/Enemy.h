#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
class Player;


class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy() override {}

	virtual void Update(float elapsedTime) = 0;
	virtual void Render(ID3D11DeviceContext* context, Shader* shader) = 0;

	void UpdateVelocity(float elapsedTime, int kind)override;

	// inhaled(形容詞で吸い込まれた)
	virtual DirectX::XMFLOAT3 inhaled() = 0;

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

public: // Get関数
	float EnoughTotalScaleValue() { return par.enough_total_scale_value; }
	float CanBeDigestionTotalScaleValue() { return par.can_be_digestion_total_scale_value; }

public: // Set関数
	void SetPlayerAddress(Player* player_) { player = player_; }
	virtual void SetInhaleParameter() = 0;

public:
	int enemy_tag;
	bool Is_inhaling = false; // Is_inhaling(吸入中) = 吸い込まれてるかどうか
	Player* player = nullptr;

	enum ENEMYTAG
	{
		NORMAL,
		BOMB,
		RARE,
	};
};