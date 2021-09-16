

#include "score.h"



Score::Score(bool render) : Digits(0, 5)
{
    if (render == false) return;

    spr_number = std::make_unique<Sprite>("Data/Sprite/数字.png");

    number_size.x = static_cast<float>(spr_number->GetTextureWidth() / 10);
    number_size.y = static_cast<float>(spr_number->GetTextureHeight());
}


void Score::SpriteRender(ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale)
{
    if (spr_number == nullptr) return;


    /* 2Dスプライトの描画 */

    std::vector<int> digit_place;
    digit_place.resize(max_digits_place);
    SliceDigits(digit, digit_place);

    for (size_t i = digit_place.size(); i > 0; --i)
    {
        spr_number->Render2(dc,
            { pos.x + number_size.x * (i - 1), pos.y + number_size.y },
            { scale.x,scale.y },
            { 0 + number_size.x * digit_place.at(digit_place.size() - i) ,0 }, { number_size.x, number_size.y },
            { 0,0 },
            0,
            { 1,1,1,1 });
    }
}