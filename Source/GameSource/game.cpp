

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
	ChangeNextScene(new Result(sceneManager), GamePad::BTN_SPACE, false);


	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: ゲーム処理

	number += elapsedTime;

	constexpr DirectX::XMFLOAT4X4 cube_trandform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	mdl_cube->UpdateTransform(cube_trandform);
	mdl_room->UpdateTransform(cube_trandform);

	CameraController::Instance()->Update(elapsedTime);
}


void Game::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */
	shader->Draw(dc, mdl_cube.get());
	shader->Draw(dc, mdl_room.get());
}


void Game::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */
	spr_font->Textout_Number(dc, (int)number, { 0,0 }, { 50,50 }, { 1,1,1,1 });
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
	spr_font = std::make_unique<Sprite>("Data/Font/font2_a.png");
	mdl_cube = std::make_unique<Model>("Data/Model/Cube.mdl");
	mdl_room = std::make_unique<Model>("Data/Model/test_wall_floor.mdl");
}


void Game::ImGui()
{
	ImGui::Text("scene : Game");

	ImGui::SliderFloat("camera range", &CameraController::Instance()->GerRange(), 1, 1000);
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