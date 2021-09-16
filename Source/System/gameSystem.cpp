

#include "gameSystem.h"
#include <algorithm>



void GameSystem::Update(float elapsedTime)
{
    timer->Update(elapsedTime);
}


void GameSystem::SpriteRender(ID3D11DeviceContext* dc)
{
    timer->SpriteRender(dc);
    //score->SpriteRender(dc);
}


void GameSystem::ScoreSpriteRender(ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale)
{
    score->SpriteRender(dc, pos, scale);
}


void GameSystem::Init()
{
    constexpr float time_limit_second = 60.8f;

    timer   = std::make_unique<Timer>(COUNT::DOWN, time_limit_second, true);
    score   = std::make_unique<Score>(true);
    hitstop = std::make_unique<HitStop>();

    data_ranking.data_array.clear();
    FileIO::Open("Data/Binary/test.box", data_ranking);
}


void GameSystem::DeInit()
{
    // TODO: リザルトが表示されたあとに数値をリセットさせる必要がある
    data_ranking.data_array.clear();
}


void GameSystem::GameStart()
{
    timer->Reset();
    //timer->Stop();

    score->Reset();

    total_scale_value = 0.0f;
}


void GameSystem::ResultDataSave()
{
    if (data_ranking.Size() <= 0)
    {
        for (size_t i = 0; i < 3; ++i)
        {
            data_ranking.SetData(0);
        }
    }

    CompareScoreAndRanking();

    FileIO::Write("Data/Binary/test.box", data_ranking);
}


void GameSystem::CompareScoreAndRanking()
{
    // 現在のスコアをランキングデータに追加して
    data_ranking.data_array.emplace_back(score->NowScore());

    //　降順にソート
    std::sort(data_ranking.data_array.begin(), data_ranking.data_array.end(), std::greater<int>());

    // 始め(0)から３つ目(3)の要素を削除 { 0 ~ 2 までの要素数３に抑える }
    data_ranking.data_array.erase(data_ranking.data_array.begin() + max_data_ranking_size);
}