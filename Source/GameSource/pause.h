#pragma once

#include "Graphics/Sprite.h"
#include <memory>



// ポーズクラス

class Pause
{
public:
    Pause();
    ~Pause();

public:
    bool Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc);
    
public: // Get関数



private:
    bool now_pause = false;
    std::unique_ptr<Sprite> spr_back = nullptr;

};