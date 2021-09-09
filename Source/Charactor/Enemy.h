#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy() override {}

	virtual void Update(float elapsedTime) = 0;
	virtual void Render(ID3D11DeviceContext* context, Shader* shader) = 0;

	// inhale(吸う) = 吸った時の動作
	virtual float inhale() = 0;

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

public:
	int enemy_tag;

	enum ENEMYTAG
	{
		NORMAL,
		RARE,
		BOMB,
	};
};