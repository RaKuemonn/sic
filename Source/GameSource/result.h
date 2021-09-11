#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"



// 結果シーン

class Result : public Scene
{
public:
    Result() {}
    ~Result()override {}

    void Update(float elapsedTime)override;
    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)override;
    void SpriteRender(ID3D11DeviceContext* dc)override;

    void DeInit()override;

    void Set()override;
    void Load()override;

    void ImGui()override;

private:

    std::unique_ptr<Sprite> spr_class = nullptr;
    std::unique_ptr<Sprite> spr_retry = nullptr;
    std::unique_ptr<Sprite> spr_end = nullptr;

    int selecting = 0;
    int select_timer = 0;

    float angle = 0.0f;

    enum Select
    {
        RETRY,
        END,
    };

};