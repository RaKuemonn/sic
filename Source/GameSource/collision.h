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

class Collision3D
{
public:

    // A押す方、B押し出される方
    static bool BallVsBallAndExtrusion(
        const DirectX::XMFLOAT3& pos_A,
        const float radius_A,
        const DirectX::XMFLOAT3& pos_B,
        const float radius_B,
        DirectX::XMFLOAT3& out_pos_B
    );

    static bool RayPickVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
};


class Collision2D
{
public:

    // 円と円の判定
    static bool CircleVsCircle(
        const DirectX::XMFLOAT2& pos_A,
        const float radius_A,
        const DirectX::XMFLOAT2& pos_B,
        const float radius_B
    );

    // 円と円の判定と押し出し
    static bool CircleVsCircleAndExtrusion(
        const DirectX::XMFLOAT2& pos_A,
        const float radius_A,
        const DirectX::XMFLOAT2& pos_B,
        const float radius_B,
        DirectX::XMFLOAT2& out_pos_B
    );

    // 矩形と矩形の判定 ※回転非対応
    static bool RectVsRect(
        const DirectX::XMFLOAT2& rect_A_center_pos,
        const DirectX::XMFLOAT2& rect_A_size,
        const DirectX::XMFLOAT2& rect_B_center_pos,
        const DirectX::XMFLOAT2& rect_B_size
    );

    // 未完成
    // 矩形と矩形の判定と押し出し ※回転非対応
    static bool RectVsRectAndExtrusion(
        const DirectX::XMFLOAT2& rect_A_center_pos,
        const DirectX::XMFLOAT2& rect_A_size,
        const DirectX::XMFLOAT2& rect_B_center_pos,
        const DirectX::XMFLOAT2& rect_B_size,
        DirectX::XMFLOAT2& rect_B_out_center_pos
    );
};