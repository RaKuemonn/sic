#pragma once

#include <DirectXMath.h>
#include "common.h"

enum class CAMERA
{
    NONE,
    NORMAL_TRACKING,
    TRANSLATION_TRACKING,
};



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
    void PadControl(float elapsedTime); // テスト用カメラの直接回転操作           ※ カメラの挙動の前に処理させる

public: // Get関数
    bool GetCameraShake() { return camera_shake; }

public: // Set関数
    void Set(const DirectX::XMFLOAT3 position_, const DirectX::XMFLOAT3 target_, const DirectX::XMFLOAT3 up_);
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
    void SetPosition(const DirectX::XMFLOAT3& pos) { this->position = pos; }
    void SetCameraShake() { camera_shake = true; }
    void SetCameraBehavior(CAMERA next_camera);

private: //      カメラの挙動      //
    void Behavior(float elapsedTime);
    void NormalTracking(float elapsedTime);         // 追うだけ
    void TranslationTracking(float elapsedTime);    // 平行移動しながら追う

private: //      カメラに掛ける効果    //
    void Shake(float elapsedTime);                  // カメラシェイク
    void ShakeXY();
    void ShakeInit();

private: // 変数

    // 姿勢表現
    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 target = { 0,0,0 };
    DirectX::XMFLOAT3 up = { 0.0f,1.0f,0.0f };
    DirectX::XMFLOAT3 angle = { 0.718f,0,0 };           // 0.718f　は　斜め下にカメラを傾けるための値

    // 注視点とカメラの距離感
    float range = 170.0f;

    // カメラの挙動状態
    CAMERA now_camera_state;
    CAMERA next_camera_state;
    DirectX::XMFLOAT3 last_position = {};               // 切り替え前のカメラ位置

    // カメラの効果用
    bool camera_shake = false;                          // カメラシェイク
    const float DEFAULT_SHAKE_TIMER = 0.1f;
    float shake_timer = DEFAULT_SHAKE_TIMER;
    DirectX::XMFLOAT3 shake_power = {};
};