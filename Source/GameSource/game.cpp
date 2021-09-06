

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

	
	
}


void Game::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */

}


void Game::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */

}


void Game::DeInit()
{
	safe_delete(a);
}
 

void Game::Set()
{
	back_color = { 1,0,0,1 };
	a = new int(9);
}


void Game::Load()
{
	
}


void Game::ImGui()
{
	ImGui::Text("scene : Game");
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

	//CameraController::Instance()->init();

}