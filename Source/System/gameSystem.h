#pragma once

#include "Graphics/Graphics.h"
#include <memory>

// �ǉ�
#include "timer.h"
#include "score.h"



// �Q�[���V�X�e�� ���V���O���g���p�^�[��

class GameSystem
{
private:
    GameSystem() {};
    ~GameSystem() {};

public:
    static GameSystem& Instance()
    {
        static GameSystem instance;
        return instance;
    }

public:
    void Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc);

    void Init();
    void DeInit();

public: // Get�֐�
    float NowTime() { return timer->NowTime(); }
    int NowScore() { return score->NowScore(); }

public: // Set�֐�
    void AddScore(const int add) { score->AddScore(add); }
    void StopTime() { timer->Stop(); }
    void UnlockStopTime() { timer->UnlockStop(); }


private:
    std::unique_ptr<Timer> timer = nullptr;     // �c�莞��
    std::unique_ptr<Score> score = nullptr;     // ���X�R�A

};