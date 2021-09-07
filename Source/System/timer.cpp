
#include "timer.h"
#include "common.h"



Timer::Timer(COUNT count, bool render, float default_start_time_) : default_start_time(default_start_time_)
{

    // 三項演算子
    pCount = static_cast<int>(count) ? /* DOWN = true */&Timer::CountDown : /* Up = false */&Timer::CountUp;


    // 描画する場合のみスプライトの実体を生成
    if (render == false) return;

    spr_number = std::make_unique<Sprite>("Data/Sprite/yone.png");

    number_size.x = static_cast<float>(spr_number->GetTextureWidth() / 10);
    number_size.y = static_cast<float>(spr_number->GetTextureHeight());

}


void Timer::Update(float elapsedTime)
{
    // 時間の更新
    (this->*pCount)(elapsedTime);
}


void Timer::SpriteRender(ID3D11DeviceContext* dc)
{
    if (spr_number == nullptr) return;


    /* 2Dスプライトの描画 */

    float one, ten, hundred;
    SliceDigits(now_time, one, ten, hundred); // 桁ごとに数値を分解する

    // 100の位
    spr_number->Render2(dc,
        { 0 + number_size.x * 0,0 },
        { 1,1 },
        { 0 + number_size.x * hundred ,0 }, { number_size.x, number_size.y },
        { 0,0 },
        0,
        { 1,1,1,1 });

    // 10の位
    spr_number->Render2(dc,
        { 0 + number_size.x * 1,0 },
        { 1,1 },
        { 0 + number_size.x * ten ,0 }, { number_size.x, number_size.y },
        { 0,0 },
        0,
        { 1,1,1,1 });

    // 1の位
    spr_number->Render2(dc,
        { 0 + number_size.x * 2,0 },
        { 1,1 },
        { 0 + number_size.x * one ,0 }, { number_size.x, number_size.y },
        { 0,0 },
        0,
        { 1,1,1,1 });
}


void Timer::AddTime(const float add)
{
    if (add < 0) return;

    now_time += add;
}


void Timer::SubtractTime(const float subtract)
{
    if (subtract > 0) return;

    now_time += subtract;
}


void Timer::SliceDigits(float digits_, float& ones_place, float& tens_place, float& hundreds_place)
{
    // 100の位
    int hundred = static_cast<int>(digits_ * 0.01f);
    hundreds_place = static_cast<float>(hundred);
    digits_ += -hundreds_place * 100.0f;

    // 10の位
    int ten = static_cast<int>(digits_ * 0.1f);
    tens_place = static_cast<float>(ten);
    digits_ += -tens_place * 10.0f;

    // 1の位
    int one = static_cast<int>(digits_);
    ones_place = static_cast<float>(one);
    digits_ += -ones_place;


    if (static_cast<int>(digits_) <= 0) return;
    // 0　以外はエラー
    assert(false);
}


void Timer::CountUp(float elapsedTime)
{
    // タイマーが限界値になったら更新しない
    if (now_time == max_time) return;


    now_time += elapsedTime;


    //TODO: 限界値 999.0f　まで
    // 限界地を超えたら
    if (now_time < max_time) return;
    now_time = max_time;

}


void Timer::CountDown(float elapsedTime)
{
    // タイマーが０になったら更新しない
    if (now_time == zero_time) return;


    now_time += -elapsedTime;


    // ゼロより下回ったら
    if (now_time > zero_time) return;
    now_time = zero_time;
}