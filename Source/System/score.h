#pragma once

#include "Graphics/Sprite.h"
#include <memory>
#include "digits.h"



// �X�R�A

class Score : public Digits<int>
{
public:
    Score(bool render = false);
    ~Score() {};

public:
    void SpriteRender(ID3D11DeviceContext* dc);

public: // Get�֐�
    int NowScore() { return GetDigit(); }

public: // Set�֐�
    void AddScore(const int add) { Add(add); }
    void SubtractScore(const int subtract) { Subtract(subtract); }

private: // �萔
    static constexpr int zero_score = 0;            // �[��
    static constexpr int max_score = 99999;         // �ő�l

private: // �ϐ�
    std::unique_ptr<Sprite> spr_number = nullptr;   // �X�v���C�g
    DirectX::XMFLOAT2 number_size = {};             // �ꕶ���̑傫��

};