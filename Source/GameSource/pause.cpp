

#include "pause.h"
#include "common.h"
#include "Input/Input.h"

#include "title.h"

Pause::Pause(Scene* scene_) : scene(scene_)
{
	now_pause = false;

	spr_back = std::make_unique<Sprite>("Data/Sprite/枠（ポーズ）.png");
	spr_play = std::make_unique<Sprite>("Data/Sprite/つづける（ポーズ）.png");
	spr_end	 = std::make_unique<Sprite>("Data/Sprite/やめる（ポーズ）.png");
}


Pause::~Pause()
{

}


bool Pause::Update(float elapsedTime)
{
	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_R)
	{
		now_pause = !now_pause;

		// TODO: コマンド入力音
	}

	if (now_pause == true)
	{
		GamePad& gamePad = Input::Instance().GetGamePad();

		if (selecting == CONTINUE)
			if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_SPACE)
			{
				now_pause = false;
				// TODO: コマンド入力音
			}


		if (selecting == END)
			if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_SPACE)
			{
				scene->ChangeNextScene(new Title());
			}

		if (gamePad.GetButtonDown() & GamePad::BTN_UP)
		{
			selecting -= 1;

			if (selecting < 0) selecting = CONTINUE;
			else select_timer = 0;

			// TODO: コマンド入力音
		}
		if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
		{
			selecting += 1;

			if (selecting > 1) selecting = END;
			else select_timer = 0;

			// TODO: コマンド入力音
		}
		if (elapsedTime) select_timer++;
	}

	return now_pause;
}


void Pause::SpriteRender(ID3D11DeviceContext* dc)
{
	if (now_pause == false) return;

	/* 2Dスプライトの描画 */
	float spr_playWidth = CAST_F(spr_play->GetTextureWidth());
	float spr_playHeight = CAST_F(spr_play->GetTextureHeight());
	float spr_endWidth = CAST_F(spr_end->GetTextureWidth());
	float spr_endHeight = CAST_F(spr_end->GetTextureHeight());

	spr_back->Render2(dc,
		0, 0,						// 表示位置
		1.0f, 1.0f,									// スケール
		0, 0,										// 画像切り抜き位置
		spr_playWidth, spr_playHeight,				// 画像切り抜きサイズ
		0, 0,	// 画像基準点
		angle,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)

	if (selecting == CONTINUE)
	{
		if (select_timer >> 5 & 0x01)
		{
			spr_play->Render2(dc,
				0,0,						// 表示位置
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
			0, 0,						// 表示位置
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
}