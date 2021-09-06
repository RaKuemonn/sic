#pragma once

#include "Graphics/Graphics.h"



// レイピック用判定結果

struct HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 }; // 当てたレイキャストと当たったポリゴンの交点
    DirectX::XMFLOAT3 normal = { 0,0,0 };   // 当たったポリゴンの法線ベクトル
    float distance = 0.0f;                  // レイキャストの始点~終点までの距離
    int materialIndex = -1;                 // 当たったポリゴンのマテリアル番号
};




// コリジョンクラス (衝突判定)

class Collision
{
public:

    // A押す方、B押し出される方
    static bool BallvsBall(
        const DirectX::XMFLOAT3& posA,
        const float radiusA_,
        const DirectX::XMFLOAT3& posB,
        const float radiusB_,
        DirectX::XMFLOAT3& outPosB
    );

    static bool RayPickvsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );

};