#pragma once

#include "timer.h"
#include "Graphics/Sprite.h"



// カウントダウンクラス

class CountDown
{
public:
    CountDown();
    ~CountDown();

public:
    bool Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc);

private:
    std::unique_ptr<Timer> timer = nullptr;
};