

#include "tutorial.h"
#include "game.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"
#include "Camera.h"
#include "CameraController.h"

// 追加
#include "gameSystem.h"
#include "stageManager.h"
#include "easy_math.h"



void Tutorial::Update(float elapsedTime)
{

	// シーン変更
	//ChangeNextScene(new Game(), GamePad::BTN_SPACE);


	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: チュートリアル処理

	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_SPACE) explaining = false;

	constexpr DirectX::XMFLOAT4X4 cube_trandform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	mdl_sky->UpdateTransform(cube_trandform);

	StageManager::Instance().Update(elapsedTime);

	// エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	player->Update(elapsedTime, explaining);

	CameraController::Instance()->SetTarget(float3SUM(player->GetPosition(), float3Scaling(player->GetFront(), 5.0f)));
	CameraController::Instance()->Update(elapsedTime, explaining);

	if (player->GetPosition().x > 15.0f) player->SetPosition({ 15.0f, player->GetPosition().y, player->GetPosition().z });
	if (player->GetPosition().z > 30.0f) player->SetPosition({ player->GetPosition().x, player->GetPosition().y, 30.0f });
	if (player->GetPosition().x < -15.0f) player->SetPosition({ 15.0f, player->GetPosition().y, player->GetPosition().z });
	if (player->GetPosition().z < -15.0f) player->SetPosition({ player->GetPosition().x, player->GetPosition().y, -15.0f });

	switch (tutorial_state)
	{
	case PLAYER_MOVE:
		if (explaining == false) End_of_explanation();
		break;
	case CAMERA_MOVE:
		if (explaining == false) End_of_explanation();
		break;
	case PLAYER_AND_CAMERA_MOVE:
		if (explaining == false)
		{
			End_of_explanation();
			enemy_Arrangement->enemy_produce(Enemy_Arrangement::csv_file_num::TUTORIAL_NORMAL);
		}
		break;
	case MERIT:
		if (EnemyManager::Instance().GetEnemyCount() <= 0)
		{
			End_of_explanation();
			enemy_Arrangement->enemy_produce(Enemy_Arrangement::csv_file_num::TUTORIAL_BOMB);
		}
		break;
	case DEMERIT:
		if (EnemyManager::Instance().GetEnemyCount() <= 0) End_of_explanation();
		break;
	case SHELL_SIZE:
		if (explaining == false) End_of_explanation();
		break;
	case END:
		//ChangeNextScene(new Game());
		ChangeNextScene(new Game(), GamePad::BTN_SPACE); // 急にシーンが変わると不自然なので任意のタイミングで変える
		break;
	default:
		break;
	}
}


void Tutorial::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */
	StageManager::Instance().ModelRender(dc, shader);
	shader->Draw(dc, mdl_sky.get());

	player->Render(dc, shader);

	// エネミー描画
	EnemyManager::Instance().Render(dc, shader);
}


void Tutorial::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */

	float spr_explanationWidth = CAST_F(spr_explanation->GetTextureWidth());
	float spr_explanationHeight = CAST_F(spr_explanation->GetTextureHeight());

	if(explaining)
	spr_explanation->Render2(dc,
		0, 0,						// 表示位置
		//1.0f, 1.0f,									// スケール
		0.25f, 0.25f,
		0, 0,										// 画像切り抜き位置
		spr_explanationWidth, spr_explanationHeight,				// 画像切り抜きサイズ
		0, 0,	// 画像基準点
		angle,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)
}


void Tutorial::DeInit()
{
	// プレイヤー終了化
	safe_delete(player);

	// 敵の配置終了化
	safe_delete(enemy_Arrangement);

	// エネミー終了化
	EnemyManager::Instance().Clear();

	StageManager::Instance().AllClear();
}


void Tutorial::Set()
{
	back_color = { 1,0,0,1 };

	CameraSet();

	GameSystem::Instance().GameStart();
}


void Tutorial::Load()
{
	mdl_sky = std::make_unique<Model>("Data/Model/Test/test_sky.mdl");
	spr_explanation = std::make_unique<Sprite>("Data/Sprite/explanation.jpg");

	// プレイヤー初期化
	player = new Player();
	player->SetPosition(DirectX::XMFLOAT3(0, foot_length, 0));

	enemy_Arrangement = new Enemy_Arrangement();

	StageManager::Instance().AddStage(new StageRoom());
}


void Tutorial::ImGui()
{
	ImGui::Text("scene : Tutorial");

	ImGui::SliderFloat("camera range", &CameraController::Instance()->GerRange(), 1, 1000);

	ImGui::Spacing();

	ImGui::Text("now_time : %.1f", GameSystem::Instance().NowTime());
	ImGui::Text("score : %d", GameSystem::Instance().NowScore());
	ImGui::Text("tutorial_state : %d", tutorial_state);
}


void Tutorial::DebugRender()
{
	player->DrawDebugPrimitive();
	EnemyManager::Instance().DrawDebugPrimitive();
}


void Tutorial::CameraSet()
{
	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	CameraController::Instance()->init();
	CameraController::Instance()->SetCameraBehavior(CAMERA::PADCONTROL);
	CameraController::Instance()->SetRange(15.0f);
}

void Tutorial::End_of_explanation()
{
	tutorial_state++;
	player->SetPosition(DirectX::XMFLOAT3(0, foot_length, 0));
	explaining = true;
}