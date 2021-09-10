

#include "title.h"
#include "game.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"

// 追加
#include "gameSystem.h"

extern bool game_exit;


void Title::Update(float elapsedTime)
{

	// "Game"にシーン変更
	if(selecting == START)
	ChangeNextScene(new Game(), GamePad::BTN_SPACE);

	GamePad& gamePad = Input::Instance().GetGamePad();
	if(selecting == END)
	if (gamePad.GetButtonDown() & GamePad::BTN_SPACE) game_exit = true;


	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: タイトル処理
	if (gamePad.GetButtonDown() & GamePad::BTN_UP)
	{
		selecting -= 1;

		if (selecting < 0) selecting = START;
		else select_timer = 0;
	}
	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
	{
		selecting += 1;

		if (selecting > 1) selecting = END;
		else select_timer = 0;
	}
	if (elapsedTime) select_timer++;

}


void Title::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */
	
}


void Title::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */
	Graphics& graphics = Graphics::Instance();

	float screenWidth = CAST_F(graphics.GetScreenWidth());
	float screenHeight = CAST_F(graphics.GetScreenHeight());
	float title_logoWidth = CAST_F(spr_title_logo->GetTextureWidth());
	float title_logoHeight = CAST_F(spr_title_logo->GetTextureHeight());
	float spr_playWidth = CAST_F(spr_play->GetTextureWidth());
	float spr_playHeight = CAST_F(spr_play->GetTextureHeight());
	float spr_endWidth = CAST_F(spr_end->GetTextureWidth());
	float spr_endHeight = CAST_F(spr_end->GetTextureHeight());

	spr_title_logo->Render2(dc,
		0, 0,						// 表示位置
		1.0f, 1.0f,									// スケール
		0, 0,										// 画像切り抜き位置
		title_logoWidth, title_logoHeight,				// 画像切り抜きサイズ
		0, 0,	// 画像基準点
		angle,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)

	if (selecting == START)
	{
		if (select_timer >> 5 & 0x01)
		{
			spr_play->Render2(dc,
				screenWidth / 2 - spr_playWidth / 2, screenHeight - spr_playHeight * 3,						// 表示位置
				1.0f, 1.0f,									// スケール
				0, 0,										// 画像切り抜き位置
				spr_playWidth, spr_playHeight,				// 画像切り抜きサイズ
				0, 0,	// 画像基準点
				angle,										// 角度
				1, 1, 1, 1);								// 色情報(r,g,b,a)
		}
	}
	else
	{
		spr_play->Render2(dc,
			screenWidth / 2 - spr_playWidth / 2, screenHeight - spr_playHeight * 3,						// 表示位置
			1.0f, 1.0f,									// スケール
			0, 0,										// 画像切り抜き位置
			spr_playWidth, spr_playHeight,				// 画像切り抜きサイズ
			0, 0,	// 画像基準点
			angle,										// 角度
			1, 1, 1, 1);								// 色情報(r,g,b,a)
	}

	if (selecting == END)
	{
		if (select_timer >> 5 & 0x01)
		{
			spr_end->Render2(dc,
				screenWidth / 2 - spr_endWidth / 2, screenHeight - spr_endHeight * 1.75f,						// 表示位置
				1.0f, 1.0f,									// スケール
				0, 0,										// 画像切り抜き位置
				spr_endWidth, spr_endHeight,				// 画像切り抜きサイズ
				0, 0,	// 画像基準点
				angle,										// 角度
				1, 1, 1, 1);								// 色情報(r,g,b,a)
		}
	}
	else
	{
		spr_end->Render2(dc,
			screenWidth / 2 - spr_endWidth / 2, screenHeight - spr_endHeight * 1.75f,						// 表示位置
			1.0f, 1.0f,									// スケール
			0, 0,										// 画像切り抜き位置
			spr_endWidth, spr_endHeight,				// 画像切り抜きサイズ
			0, 0,	// 画像基準点
			angle,										// 角度
			1, 1, 1, 1);								// 色情報(r,g,b,a)
	}


	if (GameSystem::Instance().DataRanking().Size() <= 0) return;

	for (int i = 0; i < GameSystem::Instance().DataRanking().Size(); ++i)
	{
		spr_font->Textout_Number(dc, i + 1,
			{ 0, 0 + static_cast<float>(60 * i) },
			{ 50,50 },
			{ 0,0,0,1 });

		spr_font->Textout_Number(dc, GameSystem::Instance().DataRanking().data_array.at(i),
			{ 70, 0 + static_cast<float>(60 * i) },
			{ 50,50 },
			{ 1,0,0,1 });
	}


}


void Title::DeInit()
{
	
}


void Title::Set()
{
	
}


void Title::Load()
{
	spr_title_logo	= std::make_unique<Sprite>("Data/Sprite/タイトルロゴ.jpg");
	spr_play		= std::make_unique<Sprite>("Data/Sprite/play.png");
	spr_end 		= std::make_unique<Sprite>("Data/Sprite/title.png");
	spr_font		= std::make_unique<Sprite>("Data/Font/font2_a.png");
}


void Title::ImGui()
{
	ImGui::Text("scene : TiTle");

	for (size_t i = 0; i < GameSystem::Instance().DataRanking().Size(); ++i)
	{
		ImGui::Text("%d place : %d score", i + 1, GameSystem::Instance().DataRanking().data_array.at(i));
	}
}