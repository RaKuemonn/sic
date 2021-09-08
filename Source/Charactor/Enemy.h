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
	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	void SetLengthSq(float sq) { lengthSq = sq; }
	float GetLengthSq() const{ return lengthSq; }

protected:
	float lengthSq;

public:
	int objtag;

	enum ENEMYTAG
	{
		NORMAL,
		RARE,
		BOMB,
	};
};