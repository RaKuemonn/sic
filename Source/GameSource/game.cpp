

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



void Game::Update(float elapsedTime)
{

	// シーン変更
	ChangeNextScene(new Result(), GamePad::BTN_SPACE, false);


	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: ゲーム処理
	GameSystem::Instance().Update(elapsedTime);

	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_R)
	{
		GameSystem::Instance().AddScore(1);
	}

	constexpr DirectX::XMFLOAT4X4 cube_trandform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	//mdl_cube->UpdateTransform(cube_trandform);
	//mdl_enemy1->UpdateTransform(cube_trandform);
	//mdl_enemy2->UpdateTransform(cube_trandform);
	mdl_sky->UpdateTransform(cube_trandform);

	StageManager::Instance().Update(elapsedTime);

	// エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	player->Update(elapsedTime);

	CameraController::Instance()->SetTarget(player->GetPosition());
	CameraController::Instance()->Update(elapsedTime);
}


void Game::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */
	//shader->Draw(dc, mdl_cube.get());
	StageManager::Instance().ModelRender(dc, shader);
	//shader->Draw(dc, mdl_enemy1.get());
	//shader->Draw(dc, mdl_enemy2.get());
	shader->Draw(dc, mdl_sky.get());

	player->Render(dc, shader);

	// エネミー描画
	EnemyManager::Instance().Render(dc, shader);
}


void Game::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */
	GameSystem::Instance().SpriteRender(dc);
}


void Game::DeInit()
{
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
	back_color = { 1,0,0,1 };

	CameraSet();

	GameSystem::Instance().GameStart();
}


void Game::Load()
{
	//mdl_cube	= std::make_unique<Model>("Data/Model/Test/test_chara.mdl");
	//mdl_enemy1	= std::make_unique<Model>("Data/Model/Test/test_enemy1.mdl");
	//mdl_enemy2	= std::make_unique<Model>("Data/Model/Test/test_enemy2.mdl");
	mdl_sky		= std::make_unique<Model>("Data/Model/Test/test_sky.mdl");

	// プレイヤー初期化
	player = new Player();
	player->SetPosition(DirectX::XMFLOAT3(0, 0, 0));

	// エネミー初期化
	/*EnemyManager& enemyManager = EnemyManager::Instance();

	for (int i = 0; i < 2; ++i)
	{
		NormalEnemy* normalEnemy = new NormalEnemy();
		normalEnemy->SetPosition(DirectX::XMFLOAT3(-2.0f + i * 4.0f, 0, 5));
		enemyManager.Register(normalEnemy, Enemy::ENEMYTAG::NORMAL);
	}

	for (int i = 0; i < 2; ++i)
	{
		BombEnemy* bombEnemy = new BombEnemy();
		bombEnemy->SetPosition(DirectX::XMFLOAT3(-2.0f + i * 4.0f, 0, 10));
		enemyManager.Register(bombEnemy, Enemy::ENEMYTAG::BOMB);
	}*/

	enemy_Arrangement = new Enemy_Arrangement();
	enemy_Arrangement->enemy_produce();

	StageManager::Instance().AddStage(new StageRoom());
}


void Game::ImGui()
{
	ImGui::Text("scene : Game");

	ImGui::SliderFloat("camera range", &CameraController::Instance()->GerRange(), 1, 1000);

	ImGui::Spacing();

	ImGui::Text("now_time : %.1f", GameSystem::Instance().NowTime());
	ImGui::Text("score : %d", GameSystem::Instance().NowScore());
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
}