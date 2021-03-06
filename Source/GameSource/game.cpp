

#include "game.h"
#include "result.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"
#include "Camera.h"
#include "CameraController.h"

// 追加
#include "gameSystem.h"
#include "stageManager.h"
#include "easy_math.h"
#include "audioManager.h"



void Game::Update(float elapsedTime)
{

	// シーン変更
	ChangeScene(elapsedTime);


	// ポーズ
	if (pause->Update(elapsedTime)) return;
	// カウントダウン
	countdown->Update(elapsedTime);

	BGMStart();

	//	↓	　入力処理とかいろいろ書く　	↓	　//


	// TODO: ゲーム処理

	GameSystem::Instance().HitStopUpdate(elapsedTime);

	StageManager::Instance().Update(elapsedTime);

	EnemyManager::Instance().Update(elapsedTime);

	if (countdown->NowCountDown() == false && black_band_timer == 0.0f) player->Update(elapsedTime);


	float average_scale_value = player->GetScaleManager()->TotalScaleValue() / 3 /* 3次元 x,y,z */;
	DirectX::XMFLOAT3 player_pos = player->GetPosition();
	CameraController::Instance()->SetTarget(float3SUM({player_pos.x, player_pos.y + average_scale_value + average_scale_value * 1.1f, player_pos.z}, float3Scaling(player->GetFront(), average_scale_value + average_scale_value * 3.5f)));
	CameraController::Instance()->Update(elapsedTime);


	if (countdown->NowCountDown() == false) GameSystem::Instance().Update(elapsedTime);

	SmallOut();
}


void Game::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */
	StageManager::Instance().ModelRender(dc, shader);

	player->Render(dc, shader);

	// エネミー描画
	EnemyManager::Instance().Render(dc, shader);
}


void Game::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */
	GameSystem::Instance().SpriteRender(dc);

	countdown->SpriteRender(dc, { 960,396 }, { 1,1 });

	pause->SpriteRender(dc);

	ClearedSpriteRender(dc);
}


void Game::DeInit()
{
	GameSystem::Instance().SetTotalScaleValue(player->GetScaleManager()->TotalScaleValue());

	// プレイヤー終了化
	safe_delete(player);

	// 敵の配置終了化
	safe_delete(enemy_Arrangement);

	// エネミー終了化
	EnemyManager::Instance().Clear();

	StageManager::Instance().AllClear();
}
 

void Game::Set()
{
	back_color = { 1,1,1,1 };

	CameraSet();

	GameSystem::Instance().GameStart();

	black_band_timer = 0.0f;

	smallest = false;
}


void Game::Load()
{
	pause		= std::make_unique<Pause>(this);
	countdown	= std::make_unique<CountDown>();

	// プレイヤー初期化
	player = new Player();
	player->SetPosition(DirectX::XMFLOAT3(0, 0, 0));

	enemy_Arrangement = new Enemy_Arrangement();
	enemy_Arrangement->enemy_produce();

	black_band = std::make_unique<Sprite>();

	StageManager::Instance().AddStage(new StageRoom());
}


void Game::ImGui()
{
	ImGui::Text("scene : Game");

	ImGui::SliderFloat("camera range", &CameraController::Instance()->GerRange(), 1, 1000);

	ImGui::Spacing();

	ImGui::Text("now_time : %.1f", GameSystem::Instance().NowTime());
	ImGui::Text("score : %d", GameSystem::Instance().NowScore());

	DirectX::XMFLOAT3 pos = player->GetPosition();
	ImGui::Text("player pos %.1f, %.1f, %.1f", pos.x, pos.y, pos.z);

	ImGui::Text("player total_scale %.1f", player->GetScaleManager()->TotalScaleValue());

	float length;
	DirectX::XMStoreFloat(&length, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&player->GetVelocity())));
	ImGui::Text("velo length : %.1f", length);

	DirectX::XMFLOAT3 velo = player->GetVelocity();
	ImGui::Text("player velo %.1f, %.1f, %.1f", velo.x, velo.y, velo.z);
}


void Game::DebugRender()
{
	player->DrawDebugPrimitive();
	EnemyManager::Instance().DrawDebugPrimitive();
}


void Game::CameraSet()
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


void Game::ChangeScene(float elapsedTime)
{
#if _DEBUG
	ChangeNextScene(new Result(), GamePad::BTN_A, false);
#endif

	if (GameSystem::Instance().NowTime() > 0.0f && smallest == false) return;


	// 黒帯の更新
	black_band_timer += 1.0f * elapsedTime;


	// 黒帯が降りきったら
	if (black_band_timer >= 1.4f)
	{
		// 残り時間がゼロになった際シーン遷移をする
		ChangeNextScene(new Result(), false);

		if(bgm_caution == false)AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Stop();
		if (bgm_caution == true)AudioManager::Instance().GetAudio(Audio_INDEX::BGM_SPEED)->Stop();
	}
}


void Game::ClearedSpriteRender(ID3D11DeviceContext* dc)
{
	if (black_band_timer == 0.0f) return;

	// 黒帯
	constexpr float scale = 300.0f;

	black_band->Render(dc, 0, 0, 1920, scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 1, 1, 1, 1);
	black_band->Render(dc, 0, 1080, 1920, -scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 1, 1, 1, 1);
}


void Game::BGMStart()
{
	if (bgm_normal == false && countdown->NowCountDown() == false && bgm_caution == false)
	{
		bgm_normal = true;
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Play(true);
	}

	if (bgm_normal == true && bgm_caution == false && GameSystem::Instance().NowTime() <= 20.0f)
	{
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Stop();
		bgm_caution = true;
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_SPEED)->Play(true);
	}
}

void Game::SmallOut()
{
	if (player->GetScaleManager()->TotalScaleValue() > 2.4f) return;

	smallest = true;
}