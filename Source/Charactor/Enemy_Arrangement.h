#pragma once

#include "Charactor/Enemy.h"
#include "Charactor/EnemyManager.h"
#include "Charactor/NormalEnemy.h"
#include "Charactor/BombEnemy.h"

using namespace std;
#include <string>
#include <fstream>

class Block
{
public:
    int num;
};

class Enemy_Arrangement
{
public:

    //------< �萔 >------------------------------------------------------------
    static const int CHIP_NUM_X = 250;       // �}�b�v�̉������̃`�b�v��
    static const int CHIP_NUM_Y = 250;       // �}�b�v�̏c�����̃`�b�v��
    static const int CHIP_SIZE = 2;       // %���Z�q���g�p���邽��int�^���g�p����

private:
    //------< �ϐ� >------------------------------------------------------------

    Block Arrangement[CHIP_NUM_Y][CHIP_NUM_X];                   // �G�̔z�u�f�[�^

public:
    Enemy_Arrangement() {};
    ~Enemy_Arrangement() {};

    // �X�V
    /*void update();

    void clear();*/

    void enemy_produce(int csv_file = GAME);

    enum csv_file_num
    {
        TUTORIAL_NORMAL,
        TUTORIAL_BOMB,
        GAME,
    };

};

void csv_load(Block data[][Enemy_Arrangement::CHIP_NUM_X], const char* filename);