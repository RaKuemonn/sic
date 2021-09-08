

#include "gameSystem.h"



void GameSystem::Update(float elapsedTime)
{
    timer->Update(elapsedTime);
}


void GameSystem::SpriteRender(ID3D11DeviceContext* dc)
{
    timer->SpriteRender(dc);
    score->SpriteRender(dc);
}


void GameSystem::Init()
{
    timer = std::make_unique<Timer>(COUNT::DOWN, 120, true);
    score = std::make_unique<Score>(true);

}


void GameSystem::DeInit()
{
    // TODO: リザルトが表示されたあとに数値をリセットさせる必要がある

}