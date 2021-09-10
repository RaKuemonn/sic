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

	// inhaled(形容詞で吸い込まれた)
	virtual float inhaled() = 0;

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

public:
	int enemy_tag;
	bool Is_inhaling = false; // Is_inhaling(吸入中) = 吸い込まれてるかどうか

	enum ENEMYTAG
	{
		NORMAL,
		//RARE,
		BOMB,
	};
};