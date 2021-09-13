

#include "tutorial.h"
#include "game.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"
#include "Camera.h"
#include "CameraController.h"

// �ǉ�
#include "gameSystem.h"
#include "stageManager.h"
#include "easy_math.h"



void Tutorial::Update(float elapsedTime)
{

	// �V�[���ύX
	//ChangeNextScene(new Game(), GamePad::BTN_SPACE);


	//	��	�@���͏����Ƃ����낢�돑���@	��	�@//

	// TODO: �`���[�g���A������

	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_SPACE) explaining = false;

	constexpr DirectX::XMFLOAT4X4 cube_trandform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	mdl_sky->UpdateTransform(cube_trandform);

	StageManager::Instance().Update(elapsedTime);

	// �G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

	player->Update(elapsedTime, explaining);

	CameraController::Instance()->SetTarget(float3SUM(player->GetPosition(), float3Scaling(player->GetFront(), 5.0f)));
	CameraController::Instance()->Update(elapsedTime, explaining);

	switch (tutorial_state)
	{
	case PLAYER_MOVE:
		if (explaining == false) End_of_explanation();
		break;
	case CAMERA_MOVE:
		if (explaining == false) End_of_explanation();
		break;
	case PLAYER_AND_CAMERA_MOVE:
		if (explaining == false)
		{
			End_of_explanation();
			enemy_Arrangement->enemy_produce(Enemy_Arrangement::csv_file_num::TUTORIAL_NORMAL);
		}
		break;
	case MERIT:
		if (EnemyManager::Instance().GetEnemyCount() <= 0)
		{
			End_of_explanation();
			enemy_Arrangement->enemy_produce(Enemy_Arrangement::csv_file_num::TUTORIAL_BOMB);
		}
		break;
	case DEMERIT:
		if (EnemyManager::Instance().GetEnemyCount() <= 0) End_of_explanation();
		break;
	case SHELL_SIZE:
		if (explaining == false) End_of_explanation();
		break;
	case END:
		//ChangeNextScene(new Game());
		ChangeNextScene(new Game(), GamePad::BTN_SPACE); // �}�ɃV�[�����ς��ƕs���R�Ȃ̂ŔC�ӂ̃^�C�~���O�ŕς���
		break;
	default:
		break;
	}
}


void Tutorial::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3D���f���̕`�� */
	StageManager::Instance().ModelRender(dc, shader);
	shader->Draw(dc, mdl_sky.get());

	player->Render(dc, shader);

	// �G�l�~�[�`��
	EnemyManager::Instance().Render(dc, shader);
}


void Tutorial::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2D�X�v���C�g�̕`�� */

	float spr_explanationWidth = CAST_F(spr_explanation->GetTextureWidth());
	float spr_explanationHeight = CAST_F(spr_explanation->GetTextureHeight());

	if(explaining)
	spr_explanation->Render2(dc,
		0, 0,						// �\���ʒu
		//1.0f, 1.0f,									// �X�P�[��
		0.25f, 0.25f,
		0, 0,										// �摜�؂蔲���ʒu
		spr_explanationWidth, spr_explanationHeight,				// �摜�؂蔲���T�C�Y
		0, 0,	// �摜��_
		angle,										// �p�x
		1, 1, 1, 1);								// �F���(r,g,b,a)
}


void Tutorial::DeInit()
{
	// �v���C���[�I����
	safe_delete(player);

	// �G�̔z�u�I����
	safe_delete(enemy_Arrangement);

	// �G�l�~�[�I����
	EnemyManager::Instance().Clear();

	StageManager::Instance().AllClear();
}


void Tutorial::Set()
{
	back_color = { 1,0,0,1 };

	CameraSet();

	GameSystem::Instance().GameStart();
}


void Tutorial::Load()
{
	mdl_sky = std::make_unique<Model>("Data/Model/Test/test_sky.mdl");
	spr_explanation = std::make_unique<Sprite>("Data/Sprite/explanation.jpg");

	// �v���C���[������
	player = new Player();
	player->SetPosition(DirectX::XMFLOAT3(0, foot_length, 0));

	enemy_Arrangement = new Enemy_Arrangement();

	StageManager::Instance().AddStage(new StageRoom());
}


void Tutorial::ImGui()
{
	ImGui::Text("scene : Tutorial");

	ImGui::SliderFloat("camera range", &CameraController::Instance()->GerRange(), 1, 1000);

	ImGui::Spacing();

	ImGui::Text("now_time : %.1f", GameSystem::Instance().NowTime());
	ImGui::Text("score : %d", GameSystem::Instance().NowScore());
	ImGui::Text("tutorial_state : %d", tutorial_state);
}


void Tutorial::DebugRender()
{
	player->DrawDebugPrimitive();
	EnemyManager::Instance().DrawDebugPrimitive();
}


void Tutorial::CameraSet()
{
	//�J���������ݒ�
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
	CameraController::Instance()->SetRange(15.0f);
}

void Tutorial::End_of_explanation()
{
	tutorial_state++;
	player->SetPosition(DirectX::XMFLOAT3(0, foot_length, 0));
	explaining = true;
}