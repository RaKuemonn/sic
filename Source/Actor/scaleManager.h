#pragma once

#include "Graphics/Graphics.h"



// スケール関係の総管理クラス

class ScaleManager
{
public:
    ScaleManager(DirectX::XMFLOAT3* scale_) : p_scale(scale_)
    {
        CalculateTotalScaleValue();
    };
    ~ScaleManager(){}

public:
    void Update();


public: // Get関数
    float TotalScaleValue() { return total_scale_value; }
    float GetScaleX() { return (*p_scale).x; }
    float GetScaleY() { return (*p_scale).y; }
    float GetScaleZ() { return (*p_scale).z; }


public: // Set関数
    void AddScaleX(const float add)         { if (add > 0) (*p_scale).x += add;                 CalculateTotalScaleValue(); }
    void AddScaleY(const float add)         { if (add > 0) (*p_scale).y += add;                 CalculateTotalScaleValue(); }
    void AddScaleZ(const float add)         { if (add > 0) (*p_scale).z += add;                 CalculateTotalScaleValue(); }
    void SubtractScaleX(const float minus)  { if (minus < 0) (*p_scale).x += minus; Safety();   CalculateTotalScaleValue(); }
    void SubtractScaleY(const float minus)  { if (minus < 0) (*p_scale).y += minus; Safety();   CalculateTotalScaleValue(); }
    void SubtractScaleZ(const float minus)  { if (minus < 0) (*p_scale).z += minus; Safety();   CalculateTotalScaleValue(); }


private:
    void Safety() 
    {
        if((*p_scale).x < 0.1f) (*p_scale).x = 0.1f;
        if((*p_scale).y < 0.1f) (*p_scale).y = 0.1f;
        if((*p_scale).z < 0.1f) (*p_scale).z = 0.1f;
    }
    void CalculateTotalScaleValue()
    {
        total_scale_value = (*p_scale).x + (*p_scale).y + (*p_scale).z;
    }

private:
    DirectX::XMFLOAT3* p_scale = nullptr;
    float total_scale_value = 0.0f;
};