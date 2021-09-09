#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

class NormalEnemy : public Enemy
{
public:
	NormalEnemy();
	~NormalEnemy() override;

	void Update(float elapsedTime) override;
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	float inhale() override;

private:
	Model*		model = nullptr;
};