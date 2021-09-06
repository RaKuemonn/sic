

#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"



void CameraController::Update(float elapsedTime)
{

    // カメラ挙動の切り替えの有無
    //if (now_camera_state != next_camera_state)
    //{
    //    // 切り替え
    //    now_camera_state = next_camera_state;
    //
    //    // 前回のカメラ位置を保存
    //    last_position = position;
    //}



    // 通常追尾
    if (now_camera_state == CAMERA::NORMAL_TRACKING)
        NormalTracking(elapsedTime);

    // 平行移動追尾
    if (now_camera_state == CAMERA::TRANSLATION_TRACKING)
        TranslationTracking(elapsedTime);


    // カメラシェイク
    if (camera_shake)
    {
        ShakeXY();

        shake_timer += -1 * elapsedTime;

        if (shake_timer < 0.0f) ShakeInit();
    }

    // シェイク分の加算     ※基本 0
    position.x += shake_power.x;
    position.y += shake_power.y;
    //position.z += shake_power.z;



    Camera::Instance().SetLookAt(position, target, up);

}


void CameraController::init()
{
    position = {};
    target = {};
    up = { 0.0f,1.0f,0.0f };
    angle = { 0.718f,0,0 };

    DirectX::XMFLOAT3 last_position = {};

    camera_shake = false;
    shake_timer = DEFAULT_SHAKE_TIMER;
    shake_power = {};
}


void CameraController::PadControl(float elapsedTime)
{

    constexpr float rollspeed = DirectX::XMConvertToRadians(90);
    float speed = rollspeed * elapsedTime;


    //回転操作
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        float ax = gamePad.GetAxisRX();
        float ay = gamePad.GetAxisRY();

        angle.x += ay * speed;
        angle.y += ax * speed;
    }


    constexpr float maxAngle = DirectX::XMConvertToRadians(45);
    constexpr float minAngle = DirectX::XMConvertToRadians(-45);

    //X軸カメラ回転の制限
    if (angle.x > maxAngle) angle.x = maxAngle;
    if (angle.x < minAngle) angle.x = minAngle;

    //X軸カメラ回転の制限
    if (angle.y < -DirectX::XM_PI) angle.y += DirectX::XM_2PI;
    if (angle.y > DirectX::XM_PI)  angle.y -= DirectX::XM_2PI;
}


void CameraController::NormalTracking(float elapsedTime)
{
    // 軸回転から姿勢行列を作り直しているので、向きを変えたい場合は軸角度を変更する
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);


    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);


    // 前回位置からrange分離れた位置
    position.x = last_position.x;
    position.y = last_position.y;
    position.z = last_position.z;


    //  ※前回がなければ原点
    if (last_position.x == 0.0f && last_position.y == 0.0f && last_position.z == 0.0f)
    {
        position.x = 0.0f - front.x * range;
        position.y = 0.0f - front.y * range;
        position.z = 0.0f - front.z * range;
    }


}


void CameraController::TranslationTracking(float elapsedTime)
{
    // 軸回転から姿勢行列を作り直しているので、向きを変えたい場合は軸角度を変更する
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);


    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //DirectX::XMFLOAT3 eye;
    // ターゲットからrange分離れた位置
    position.x = target.x - front.x * range;
    position.y = target.y - front.y * range;
    position.z = target.z - front.z * range;
}


void CameraController::ShakeXY()
{
    constexpr float shake_value_range_X = 15.0f;    // x軸 15の幅 (-7.5f ~ 7.5f)
    constexpr float shake_value_range_Y = 5.0f;     // y軸  5の幅 (-2.5f ~ 2.5f)

    // 0x7fff = 定数 RAND_MAX
    float rand_max = 1.0f / (float)0x7fff;

    // 実数値で乱数生成
    shake_power.x = (float)rand() * rand_max * shake_value_range_X - shake_value_range_X * 0.5f;
    shake_power.y = (float)rand() * rand_max * shake_value_range_Y - shake_value_range_Y * 0.5f;

}


void CameraController::ShakeInit()
{
    camera_shake = false;
    shake_timer = DEFAULT_SHAKE_TIMER;
    shake_power = {};
}