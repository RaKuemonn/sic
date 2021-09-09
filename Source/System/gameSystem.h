#pragma once

#include "Graphics/Graphics.h"
#include <memory>

// 追加
#include "timer.h"
#include "score.h"
#include "fileIO.h"



// ゲームシステム ※シングルトンパターン

class GameSystem
{
private:
    GameSystem() {};
    ~GameSystem() {};

public:
    static GameSystem& Instance()
    {
        static GameSystem instance;
        return instance;
    }

public:
    void Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc);

    void Init();
    void DeInit();

public: // Get関数
    float NowTime() { return timer->NowTime(); }
    int NowScore() { return score->NowScore(); }

public: // Set関数
    void AddScore(const int add) { score->AddScore(add); }
    void StopTime() { timer->Stop(); }
    void UnlockStopTime() { timer->UnlockStop(); }


private:
    std::unique_ptr<Timer> timer = nullptr;     // 残り時間
    std::unique_ptr<Score> score = nullptr;     // 総スコア
    FileData data_ranking = {};                 // ランキングデータ
};