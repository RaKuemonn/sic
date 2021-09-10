#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include <memory>

// 追加
#include "Charactor/Player.h"
#include "Charactor/EnemyManager.h"
#include "Charactor/NormalEnemy.h"
#include "Charactor/BombEnemy.h"

// ゲームシーン

class Game : public Scene
{
public:
    Game() {}
    ~Game()override{}

    void Update(float elapsedTime)override;
    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)override;
    void SpriteRender(ID3D11DeviceContext* dc)override;

    void DeInit()override;

    void Set()override;
    void Load()override;

    void ImGui()override;

    void DebugRender()override;

private:
    void CameraSet();

private:
    //std::unique_ptr<Model> mdl_cube = nullptr;
    //std::unique_ptr<Model> mdl_enemy1 = nullptr;
    //std::unique_ptr<Model> mdl_enemy2 = nullptr;
    std::unique_ptr<Model> mdl_sky = nullptr;

    Player* player = nullptr;
};