

#include "gameSystem.h"
#include <algorithm>



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
    constexpr int start_time_second = 120;

    timer = std::make_unique<Timer>(COUNT::DOWN, start_time_second, true);
    score = std::make_unique<Score>(true);

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
}


void GameSystem::ResultDataSave()
{
    CompareScoreAndRanking();

    FileIO::Write("Data/Binary/test.box", data_ranking);
}


void GameSystem::CompareScoreAndRanking()
{
    constexpr size_t max_ranking_size = 3;

    data_ranking.data_array.emplace_back(score->NowScore());

    std::sort(data_ranking.data_array.begin(), data_ranking.data_array.end(), std::greater<int>());

    data_ranking.data_array.erase(data_ranking.data_array.begin() + 3);
}