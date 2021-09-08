

#include "game.h"
#include "result.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"
#include "Camera.h"
#include "CameraController.h"



void Game::Update(float elapsedTime)
{

	// シーン変更
	ChangeNextScene(new Result(), GamePad::BTN_SPACE, false);


	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: ゲーム処理
	timer->Update(elapsedTime);

	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_R)
	{
		score->AddScore(1);
	}

	constexpr DirectX::XMFLOAT4X4 cube_trandform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	mdl_cube->UpdateTransform(cube_trandform);
	mdl_room->UpdateTransform(cube_trandform);
	mdl_enemy1->UpdateTransform(cube_trandform);
	mdl_enemy2->UpdateTransform(cube_trandform);
	mdl_sky->UpdateTransform(cube_trandform);

	CameraController::Instance()->Update(elapsedTime);
}


void Game::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */
	shader->Draw(dc, mdl_cube.get());
	shader->Draw(dc, mdl_room.get());
	//shader->Draw(dc, mdl_enemy1.get());
	//shader->Draw(dc, mdl_enemy2.get());
	shader->Draw(dc, mdl_sky.get());
}


void Game::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */
	timer->SpriteRender(dc);
	score->SpriteRender(dc);
}


void Game::DeInit()
{

}
 

void Game::Set()
{
	back_color = { 1,0,0,1 };

	CameraSet();
}


void Game::Load()
{
	timer		= std::make_unique<Timer>(COUNT::UP, true);
	score		= std::make_unique<Score>(true);

	mdl_cube	= std::make_unique<Model>("Data/Model/Test/test_chara.mdl");
	mdl_room	= std::make_unique<Model>("Data/Model/Test/test_wall_floor.mdl");
	mdl_enemy1	= std::make_unique<Model>("Data/Model/Test/test_enemy1.mdl");
	mdl_enemy2	= std::make_unique<Model>("Data/Model/Test/test_enemy2.mdl");
	mdl_sky		= std::make_unique<Model>("Data/Model/Test/test_sky.mdl");
}


void Game::ImGui()
{
	ImGui::Text("scene : Game");

	ImGui::SliderFloat("camera range", &CameraController::Instance()->GerRange(), 1, 1000);

	ImGui::Spacing();

	ImGui::Text("now_time : %.1f", timer->NowTime());
	ImGui::Text("score : %d", score->NowScore());
}


void Game::DebugRender()
{
	
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