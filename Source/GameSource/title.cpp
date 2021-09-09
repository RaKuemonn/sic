

#include "title.h"
#include "game.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"

// 追加
#include "gameSystem.h"



void Title::Update(float elapsedTime)
{

	// "Game"にシーン変更
	ChangeNextScene(new Game(), GamePad::BTN_SPACE);

	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: タイトル処理

}


void Title::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */
	
}


void Title::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */

}


void Title::DeInit()
{
	
}


void Title::Set()
{
	
}


void Title::Load()
{

}


void Title::ImGui()
{
	ImGui::Text("scene : TiTle");

	for (size_t i = 0; i < GameSystem::Instance().DataRanking().Size(); ++i)
	{
		ImGui::Text("%d place : %d score", i + 1, GameSystem::Instance().DataRanking().data_array.at(i));
	}
}