#pragma once

#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include <memory>

enum class COUNT
{
    UP,
    DOWN,
};



// タイマー

class Timer
{
public:
    Timer(COUNT count, bool render = false, float default_start_time_ = 0.0f);
    ~Timer() {};

public:
    void Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc);

public: // Get関数
    float NowTime() const { return now_time; }

public: // Set関数
    void AddTime(const float add);
    void SubtractTime(const float subtract);

private:
    void SliceDigits(float digits_/*数値*/, float& ones_place/*1の位*/, float& tens_place/*10の位*/, float& hundreds_place/*100の位*/);

    //　関数ポインタ //
    using pFunc = void(Timer::*)(float);
    pFunc pCount = nullptr;

    // 関数ポインタ用関数
    void CountUp(float elapsedTime);
    void CountDown(float elapsedTime);

private: // 定数
    static constexpr float zero_time = 0.0f;
    static constexpr float max_time = 999.0f;

private: // 変数
    const float default_start_time;                 // 初期値
    float now_time = default_start_time;            // 現在時間
    std::unique_ptr<Sprite> spr_number = nullptr;   // スプライト
    DirectX::XMFLOAT2 number_size = {};             // 一文字の大きさ

};