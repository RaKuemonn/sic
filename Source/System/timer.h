#pragma once

#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include <memory>
#include "digits.h"

enum class COUNT
{
    UP,
    DOWN,
};



// タイマー

class Timer : public Digits<float>
{
public:
    Timer(COUNT count, bool render = false, float default_start_time_ = 0.0f);
    ~Timer() {};

public:
    void Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc);

public: // Get関数
    float NowTime() const { return digit; }

public: // Set関数
    void AddTime(const float add);
    void SubtractTime(const float subtract);

private:
    //　関数ポインタ //
    using pFunc = void(Timer::*)(float);
    pFunc pCount = nullptr;

    // 関数ポインタ用関数
    void CountUp(float elapsedTime);
    void CountDown(float elapsedTime);

private: // 定数
    static constexpr float zero_time = 0.0f;        // ゼロ
    static constexpr float max_time = 9999.0f;      // 最大値

private: // 変数
    std::unique_ptr<Sprite> spr_number = nullptr;   // スプライト
    DirectX::XMFLOAT2 number_size = {};             // 一文字の大きさ

};