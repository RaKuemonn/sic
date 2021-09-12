

#include "scaleManager.h"
#include "CameraController.h"
#include "gameSystem.h"



void ScaleManager::Update()
{
    constexpr int bonus_score = 50;


    static bool did3 = false;
    if (total_scale_value > 25.0f && did3 == false)
    {
        CameraController::Instance()->SetRange(80.0f);
        GameSystem::Instance().AddScore(bonus_score * 4);
        did3 = true;
    }


    static bool did2 = false;
    if (total_scale_value > 20.0f && did2 == false)
    {
        CameraController::Instance()->SetRange(65.0f);
        GameSystem::Instance().AddScore(bonus_score* 3);
        did2 = true;
    }


    static bool did1 = false;
    if (total_scale_value > 15.0f && did1 == false)
    {
        CameraController::Instance()->SetRange(50.0f);
        GameSystem::Instance().AddScore(bonus_score * 2);
        did1 = true;
    }


    static bool did0 = false;
    if (total_scale_value > 10.0f && did0 == false)
    {
        CameraController::Instance()->SetRange(35.0f);
        GameSystem::Instance().AddScore(bonus_score * 1);
        did0 = true;
    }
}