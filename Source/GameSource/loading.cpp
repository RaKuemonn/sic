

#include <thread>
#include "loading.h"
#include "common.h"
#include "Input/Input.h"



void Loading::Update(float elapsedTime)
{

	// シーン変更
	{

	}


	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: 読み込み処理

	angle += elapsedTime * 600.0f;/* TEST: 後で消す */

	if (scene->IsSceneLoaded() && angle > 360.0f * 2.0f/* TEST: 後で消す */)
	{
		sceneManager.SetNextScene(scene);
		scene = nullptr;
	}
}


void Loading::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{

}


void Loading::SpriteRender(ID3D11DeviceContext* dc)
{
	Graphics& graphics = Graphics::Instance();

	float screenWidth = CAST_F(graphics.GetScreenWidth());
	float screenHeight = CAST_F(graphics.GetScreenHeight());
	float textureWidth = CAST_F(spr_icon->GetTextureWidth());
	float textureHeight = CAST_F(spr_icon->GetTextureHeight());
	float positionX = screenWidth - textureWidth;
	float positionY = screenHeight - textureHeight;

	spr_icon->Render2(dc,
		positionX, positionY,						// 表示位置
		1.0f, 1.0f,									// スケール
		0, 0,										// 画像切り抜き位置
		textureWidth, textureHeight,				// 画像切り抜きサイズ
		textureWidth * 0.5f, textureHeight * 0.5f,	// 画像基準点
		angle,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)
}


void Loading::DeInit()
{
	safe_delete(spr_icon);

	if (scene == nullptr) return;
	scene->DeInit();
	safe_delete(scene);
}


void Loading::Set()
{
	std::thread thread(LoadingThread, this);

	thread.detach();
}


void Loading::Load()
{
	spr_icon = new Sprite("Data/Sprite/LoadingIcon.png");
}


void Loading::ImGui()
{
	ImGui::Text("scene : Loading");
}

void Loading::LoadingThread(Loading* load)
{
	CoInitialize(nullptr);

	load->scene->Init();

	CoUninitialize();

	load->scene->SetSceneLoaded();
}