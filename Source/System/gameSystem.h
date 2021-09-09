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
    FileData& DataRanking() { return data_ranking; }

public: // Set関数
    void GameStart();
    void ResultDataSave();
    void AddScore(const int add) { score->Add(add); }
    void StopTime() { timer->Stop(); }
    void UnlockStopTime() { timer->UnlockStop(); }

private:
    void CompareScoreAndRanking();

private: // 定数
    static constexpr size_t max_data_ranking_size = 3;

private: // 変数
    std::unique_ptr<Timer> timer = nullptr;     // 残り時間
    std::unique_ptr<Score> score = nullptr;     // 総スコア
    FileData data_ranking = {};                 // ランキングデータ
};