#pragma once

#include "Charactor/Enemy.h"
#include "Charactor/EnemyManager.h"
#include "Charactor/NormalEnemy.h"
#include "Charactor/BombEnemy.h"
#include "Charactor/RareEnemy.h"

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

    //------< 定数 >------------------------------------------------------------
    static const int CHIP_NUM_X = 250;       // マップの横方向のチップ数
    static const int CHIP_NUM_Y = 250;       // マップの縦方向のチップ数
    static const int CHIP_SIZE = 2;       // %演算子を使用するためint型を使用する

private:
    //------< 変数 >------------------------------------------------------------

    Block Arrangement[CHIP_NUM_Y][CHIP_NUM_X];                   // 敵の配置データ

public:
    Enemy_Arrangement() {};
    ~Enemy_Arrangement() {};

    // 更新
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