#pragma once

#include "Graphics/Sprite.h"
#include <memory>
#include "digits.h"



// スコア

class Score : public Digits<int>
{
public:
    Score(bool render = false);
    ~Score() {};

public:
    void SpriteRender(ID3D11DeviceContext* dc);

public: // Get関数
    int NowScore() { return GetDigit(); }

public: // Set関数
    void Reset() { Digits::Reset(); }
    void Add(const int add) { Digits::Add(add); }
    void Subtract(const int subtract) { Digits::Subtract(subtract); }

private: // 定数
    static constexpr int zero_score = 0;            // ゼロ
    static constexpr int max_score = 99999;         // 最大値

private: // 変数
    std::unique_ptr<Sprite> spr_number = nullptr;   // スプライト
    DirectX::XMFLOAT2 number_size = {};             // 一文字の大きさ

};