#pragma once

#include <DirectXMath.h>
#include "common.h"



// カメラ操作クラス (シングルトン)

class CameraController
{
private:
    CameraController(){}
    ~CameraController(){}

public:
    static CameraController* Instance()
    {
        static CameraController instance;

        return &instance;
    }

    void Update(float elapsedTime);
    void init();
    
public:
    // カメラの挙動

    // 追うだけ
    void NormalTracking(float elapsedTime);
    // 平行移動しながら追う
    void TranslationTracking(float elapsedTime);
    // カメラシェイク
    void ShakeXY();

public:
    // テスト用カメラの直接回転操作           ※ カメラの挙動の前に処理させる
    void PadControl(float elapsedTime);

private:
    void ShakeInit();


public:
    // ゲッター & セッター
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
    void SetPosition(const DirectX::XMFLOAT3& pos) { this->position = pos; }
    void SetCameraShake() { camera_shake = true; }
    bool GetCameraShake() { return camera_shake; }


private:
    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 target = { 0,0,0 };
    DirectX::XMFLOAT3 up = { 0.0f,1.0f,0.0f };

    DirectX::XMFLOAT3 angle = { 0.718f,0,0 };           // 0.718f　は　斜め下にカメラを傾けるための値
    float range = 170.0f;

    CAMERA now_camera_state;
    DirectX::XMFLOAT3 last_position = {};               // 切り替え前のカメラ位置

    bool camera_shake = false;                          // カメラシェイク
    const float DEFAULT_SHAKE_TIMER = 0.1f;
    float shake_timer = DEFAULT_SHAKE_TIMER;
    DirectX::XMFLOAT3 shake_power = {};
};