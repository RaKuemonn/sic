

#include "result.h"
#include "game.h"
#include "title.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"

// 追加
#include "gameSystem.h"
#include "audioManager.h"



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
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Play(false);

		selecting -= 1;  

		if (selecting < 0) selecting = RETRY;
		else select_timer = 0;
	}
	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
	{
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Play(false);

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

	spr_back->Render2(dc,
		0, 0,						// 表示位置
		1.0f, 1.0f,									// スケール
		0, 0,										// 画像切り抜き位置
		spr_classWidth, spr_classHeight,				// 画像切り抜きサイズ
		0, 0,	// 画像基準点
		angle,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)

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

	// フグのアイコンとフォントの表示
	spr_hugu_icon->Render2(dc, { 0,0 }, { 1,1 }, { 0,0 }, { screenWidth,screenHeight }, { 0,0 }, 0, { 1,1,1,1 });
	spr_hugu_font->Render2(dc, { 0,0 }, { 1,1 }, { 0,0 }, { screenWidth,screenHeight }, { 0,0 }, 0, { 1,1,1,1 });




	GameSystem::Instance().ScoreSpriteRender(dc, { 710,390 }, { 1,1 });


	// ランキングの表示
	for (int i = 0; i < GameSystem::Instance().DataRanking().Size(); ++i)
	{
		constexpr DirectX::XMFLOAT2 scale = { 0.6f, 0.6f };

		//　1文字の幅と高さを計算
		float sw = static_cast<float>(spr_font->GetTextureWidth() / 10);
		float sh = static_cast<float>(spr_font->GetTextureHeight());

		// 現在の文字位置(相対位置)
		float carriage = 0;

		// 数値を文字に変換
		std::string str_number = std::to_string(static_cast<int>(GameSystem::Instance().DataRanking().data_array.at(i)));

		DirectX::XMFLOAT4 color = { 1,1,1,1 };

		DirectX::XMFLOAT2 pos = { 350.0f ,690.0f };

		constexpr float mozikan = 150.0f;

		// 一文字づつRender()する
		for (const char c : str_number)
		{
			//文字を表示。アスキーコードの位置にある文字位置を切り抜いて表示
			spr_font->Render2(dc, { pos.x + carriage, pos.y + mozikan * i }, { scale.x, scale.y }, { sw * (c & 0x0F), sh }, { sw, sh }, { sw * 0.5f, sh * 0.5f }, 0, { color.x, color.y, color.z, color.w });
			//文字位置を幅分ずらす
			carriage += sw * scale.x;
		}
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
	did_first = false;
	did = false;

	SetHuguRank();

	AudioManager::Instance().GetAudio(Audio_INDEX::SE_RESULT)->Play(false);
}


void Result::Load()
{
	spr_back = std::make_unique<Sprite>("Data/Sprite/タイトルロゴなし背景画像（リザルト）.png");
	spr_class = std::make_unique<Sprite>("Data/Sprite/あなたは〜です。＆スコア＆ランキング（リザルト）.png");
	spr_retry = std::make_unique<Sprite>("Data/Sprite/リトライ（リザルト）.png");
	spr_end = std::make_unique<Sprite>("Data/Sprite/やめる（リザルト）.png");
	spr_font = std::make_unique<Sprite>("Data/Sprite/数字.png");


	black_band = std::make_unique<Sprite>();
}


void Result::ImGui()
{
	ImGui::Text("scene : Result");
}


void Result::ChangeScene(float elapsedTime)
{
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
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_RESULT)->Stop();

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


void Result::SetHuguRank()
{
	char* icon_filename = {};
	char* font_filename = {};


	float total_scale_value = GameSystem::Instance().TotalScaleValue();


	constexpr float default_start_value = 3.0f;


	// くさふぐ
	if (total_scale_value <= kusa_hugu_scale || total_scale_value > default_start_value)
	{
		icon_filename = "Data/Sprite/くさふぐ画像（リザルト）.png";
		font_filename = "Data/Sprite/くさふぐ（リザルト）.png";
	}

	// しょうさいふぐ
	if (total_scale_value > shousai_hugu_scale)
	{
		icon_filename = "Data/Sprite/しょうさいふぐ画像（リザルト）.png";
		font_filename = "Data/Sprite/しょうさいふぐ（リザルト）.png";
	}

	// まふぐ
	if (total_scale_value > ma_hugu_scale)
	{
		icon_filename = "Data/Sprite/まふぐ画像（リザルト）.png";
		font_filename = "Data/Sprite/まふぐ（リザルト）.png";
	}

	// しまふぐ
	if (total_scale_value > shima_hugu_scale)
	{
		icon_filename = "Data/Sprite/しまふぐ画像（リザルト）.png";
		font_filename = "Data/Sprite/しまふぐ（リザルト）.png";
	}

	// とらふぐ
	if (total_scale_value > tora_hugu_scale)
	{
		icon_filename = "Data/Sprite/とらふぐ画像（リザルト）.png";
		font_filename = "Data/Sprite/とらふぐ（リザルト）.png";
	}


	spr_hugu_icon = std::make_unique<Sprite>(icon_filename);
	spr_hugu_font = std::make_unique<Sprite>(font_filename);
}