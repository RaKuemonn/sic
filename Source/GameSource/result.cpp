

#include "result.h"
#include "game.h"
#include "title.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"

// 追加
#include "gameSystem.h"



void Result::Update(float elapsedTime)
{

	// シーン変更
	{
		ChangeScene(elapsedTime);
	}


	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: 結果処理
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_UP)
	{
		selecting -= 1;

		if (selecting < 0) selecting = RETRY;
		else select_timer = 0;
	}
	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
	{
		selecting += 1;

		if (selecting > 1) selecting = END;
		else select_timer = 0;
	}
	if (elapsedTime) select_timer++;





	// 黒帯の時間更新
	if (did_first == false)
	{
		black_band_timer += 1.0f * elapsedTime;

		if (ClampMax(black_band_timer, black_band_timer_max))
		{
			did_first = true;
			black_band_timer = 0.0f;

		}
	}

}


void Result::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{

}


void Result::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */
	Graphics& graphics = Graphics::Instance();

	float screenWidth = CAST_F(graphics.GetScreenWidth());
	float screenHeight = CAST_F(graphics.GetScreenHeight());
	float spr_classWidth = CAST_F(spr_class->GetTextureWidth());
	float spr_classHeight = CAST_F(spr_class->GetTextureHeight());
	float spr_retryWidth = CAST_F(spr_retry->GetTextureWidth());
	float spr_retryHeight = CAST_F(spr_retry->GetTextureHeight());
	float spr_endWidth = CAST_F(spr_end->GetTextureWidth());
	float spr_endHeight = CAST_F(spr_end->GetTextureHeight());

	spr_class->Render2(dc,
		0, 0,						// 表示位置
		1.0f, 1.0f,									// スケール
		0, 0,										// 画像切り抜き位置
		spr_classWidth, spr_classHeight,				// 画像切り抜きサイズ
		0, 0,	// 画像基準点
		angle,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)

	if (selecting == RETRY)
	{
		if (select_timer >> 5 & 0x01)
		{
			spr_retry->Render2(dc,
				0, 0,						// 表示位置
				1.0f, 1.0f,									// スケール
				0, 0,										// 画像切り抜き位置
				spr_retryWidth, spr_retryHeight,				// 画像切り抜きサイズ
				0, 0,	// 画像基準点
				angle,										// 角度
				1, 1, 1, 1);								// 色情報(r,g,b,a)
		}
	}
	else
	{
		spr_retry->Render2(dc,
			0, 0,						// 表示位置
			1.0f, 1.0f,									// スケール
			0, 0,										// 画像切り抜き位置
			spr_retryWidth, spr_retryHeight,				// 画像切り抜きサイズ
			0, 0,	// 画像基準点
			angle,										// 角度
			1, 1, 1, 1);								// 色情報(r,g,b,a)
	}

	if (selecting == END)
	{
		if (select_timer >> 5 & 0x01)
		{
			spr_end->Render2(dc,
				0, 0,						// 表示位置
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
			0, 0,						// 表示位置
			1.0f, 1.0f,									// スケール
			0, 0,										// 画像切り抜き位置
			spr_endWidth, spr_endHeight,				// 画像切り抜きサイズ
			0, 0,	// 画像基準点
			angle,										// 角度
			1, 1, 1, 1);								// 色情報(r,g,b,a)
	}



	// 黒帯
	constexpr float scale = 300.0f;

	if(did_first == false)black_band->Render(dc, 0, 0, 1920, 540 - scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 1, 1, 1, 1);
	if(did_first == false)black_band->Render(dc, 0, 540 + scale * pow(black_band_timer, 5), 1920, 540, 0, 0, 0, 0, 0, 1, 1, 1, 1);

	if(did_first)ChangeSceneSpriteRender(dc);

}


void Result::DeInit()
{

}


void Result::Set()
{
	GameSystem::Instance().ResultDataSave();

	black_band_timer = 0.0f;
}


void Result::Load()
{
	spr_class = std::make_unique<Sprite>("Data/Sprite/あなたは〜です。＆スコア＆ランキング（リザルト）.png");
	spr_retry = std::make_unique<Sprite>("Data/Sprite/リトライ（リザルト）.png");
	spr_end = std::make_unique<Sprite>("Data/Sprite/やめる（リザルト）.png");


	black_band = std::make_unique<Sprite>();
}


void Result::ImGui()
{
	ImGui::Text("scene : Result");
}


void Result::ChangeScene(float elapsedTime)
{
	static bool did = false;
	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_SPACE)
	{
		did = true;
	}

	if (did == false) return;

	// 黒帯の更新
	black_band_timer += 1.0f * elapsedTime;


	// 黒帯が降りきったら
	if (black_band_timer >= 1.4f)
	{
		if (selecting == RETRY)
			ChangeNextScene(new Game(), true);

		if (selecting == END)
			ChangeNextScene(new Title(), true);
	}
}



void Result::ChangeSceneSpriteRender(ID3D11DeviceContext* dc)
{
	if (black_band_timer == 0.0f) return;

	// 黒帯
	constexpr float scale = 300.0f;

	black_band->Render(dc, 0, 0, 1920, scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 1, 1, 1, 1);
	black_band->Render(dc, 0, 1080, 1920, -scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 1, 1, 1, 1);
}