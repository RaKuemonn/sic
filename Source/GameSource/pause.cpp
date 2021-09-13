

#include "pause.h"
#include "Input/Input.h"



Pause::Pause()
{
	now_pause = false;
}


Pause::~Pause()
{

}


bool Pause::Update(float elapsedTime)
{
	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_R)
	{
		now_pause = !now_pause;
	}

	return now_pause;
}


void Pause::SpriteRender(ID3D11DeviceContext* dc)
{
	if (now_pause == false) return;


}