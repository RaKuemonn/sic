

#include "coutdown.h"



CountDown::CountDown()
{
    constexpr int start_time_second = 3;

    timer = std::make_unique<Timer>(COUNT::DOWN, start_time_second, true);
}

CountDown::~CountDown()
{

}

bool CountDown::Update(float elapsedTime)
{
    return false;
}

void CountDown::SpriteRender(ID3D11DeviceContext* dc)
{
    timer->SpriteRender(dc);
}