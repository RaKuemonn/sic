#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "collision.h"

// コンストラクタ
Player::Player()
{
	model = new Model("Data/Model/Test/test_chara.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 1.0f;

	//音
	/*Audio& audio1 = Audio::Instance();
	Shot = audio1.LoadAudioSource("Data/Audio/Shot.wav");

	Audio& audio2 = Audio::Instance();
	Hit = audio2.LoadAudioSource("Data/Audio/Hit.wav");*/

}

// デストラクタ
Player::~Player()
{
	delete model;
	model = NULL;
}


DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	// 入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// カメラ方向とスティックの入力値によって進行方向を計算
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// 移動ベクトルはXZ平面に水平なベクトルになるようにする

	// カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX		= cameraRight.x;
	float cameraRightZ		= cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// 単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX		= cameraFront.x;
	float cameraFrontZ		= cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// 単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// スティックの水平入力値をカメラ右方向に反映し、
	// スティックの垂直入力値をカメラ前方向に反映し、
	// 進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = cameraRightX * ax + cameraFrontX * ay;
	vec.z = cameraRightZ * ax + cameraFrontZ * ay;

	// Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;
}

// 更新処理
void Player::Update(float elapsedTime)
{
	InputMove(elapsedTime);

	// 速力更新処理
	UpdateVelocity(elapsedTime, KIND::PLAYER);

	// オブジェクト行列を更新
	UpdateTransform();
		
	// モデル行列更新
	model->UpdateTransform(transform);

}

// 移動処理
void Player::Move(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	position.x += vx * speed;
	position.z += vz * speed;
}

// 移動入力処理
void Player::InputMove(float elapsedTime)
{
	// 進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	// 移動処理
	Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);

	// プレイヤーとエネミーの衝突処理
	CollisionPlayerVsEnemies();
}

// 描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

// プレイヤーとエネミーの衝突処理
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < 1; ++i)
	{
		Player* player = {};

		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);

			// 衝突処理
			DirectX::XMFLOAT3 outPosition;
			if (Collision3D::BallVsBallAndExtrusion/*collision::IntersectSqhereVsXYCircle*/(
				player->GetPosition(),
				player->GetRadius(),
				enemy->GetPosition(),
				enemy->GetRadius(),
				/*enemy->GetHeight(),*/
				outPosition
			))
			{
				enemy->inhale();
				//switch (enemy->enemy_tag)
				//{
				//case Enemy::ENEMYTAG::NORMAL:
				//	//Hit->Play(false, HIT_VOLUME);
				//	break;
				//case Enemy::ENEMYTAG::RARE:
				//	break;
				//case Enemy::ENEMYTAG::BOMB:
				//	break;
				//default:
				//	break;
				//}
			}
		}

	}
}

// デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	
	// 衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

// デバッグ用GUI描画
void Player::DrawDebugGUI(DirectX::XMFLOAT3 wind_velocity)
{

	ImGui::SetNextWindowPos (ImVec2(10, 10)  , ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		// トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// 位置
			ImGui::InputFloat3("Position", &position.x);
			// 回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			// スケール
			ImGui::InputFloat3("Scale", &scale.x);
		}
	}
	ImGui::End();

}