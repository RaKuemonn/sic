

#include "result.h"
#include "game.h"
#include "title.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"



void Result::Update(float elapsedTime)
{

	// シーン変更
	{
		ChangeNextScene(new Title(sceneManager), GamePad::BTN_SPACE);
		
		ChangeNextScene(new Game(sceneManager), GamePad::BTN_R);
	}


	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: 結果処理

}


void Result::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{

}


void Result::SpriteRender(ID3D11DeviceContext* dc)
{
	
}


void Result::DeInit()
{

}


void Result::Set()
{
	
}


void Result::Load()
{

}


void Result::ImGui()
{
	ImGui::Text("scene : Result");
}