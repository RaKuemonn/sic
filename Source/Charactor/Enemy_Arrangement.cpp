#include "Charactor/Enemy_Arrangement.h"

void csv_load(Block data[][Enemy_Arrangement::CHIP_NUM_X], const char* filename)
{
    ifstream stream(filename);
    string line;
    //int data[45][15];
    const string delim = ",";

    int row = 0;
    int col;
    while (getline(stream, line))
    {
        col = 0;

        //delimを区切り文字として切り分け、intに変換してdata[][]に格納する
        for (string::size_type spos, epos = 0; (spos = line.find_first_not_of(delim, epos)) != string::npos;)
        {
            string token = line.substr(spos, (epos = line.find_first_of(delim, spos)) - spos);
            data[row][col++].num = stoi(token);
        }

        ++row;
    }
}

//// 更新
//void Enemy_Arrangement::update()
//{
//
//}

//void Enemy_Arrangement::clear()
//{
//
//}

void Enemy_Arrangement::enemy_produce(csv_file_num csv_file)
{
    // エネミー初期化
    EnemyManager& enemyManager = EnemyManager::Instance();
    
    switch (csv_file)
    {
    case TUTORIAL_NORMAL:
        csv_load(Arrangement,"Data/map/enemy_arrangement._チュートリアル(normal).csv");
        break;
    case TUTORIAL_BOMB:
        csv_load(Arrangement,"Data/map/enemy_arrangement._チュートリアル(bomb).csv");
        break;
    case GAME:
        csv_load(Arrangement,"Data/map/enemy_arrangement._本番用.csv");
        break;
    default:
        break;
    }

    for (int y = 0; y < CHIP_NUM_Y; y++)
    {
        for (int x = 0; x < CHIP_NUM_X; x++)
        {
            // 基準点補正　ステージの左上の座標から始まるように
            constexpr float Reference_point_correction = -249;
            //float randam_scale = 1;

            srand((unsigned int)time(NULL));

            if (Arrangement[y][x].num == Enemy::ENEMYTAG::NORMAL)
            {
                if ((y < 100 || y > 150) /*&& (x < 100 || x > 150)*/)
                    if (csv_file == csv_file_num::GAME) randam_scaling(Arrangement[y][x].num);

                NormalEnemy* normalEnemy = new NormalEnemy();
                normalEnemy->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));
                normalEnemy->SetScale({ randam_scale, randam_scale, randam_scale });
                enemyManager.Register(normalEnemy, Enemy::ENEMYTAG::NORMAL);
            }
            if (Arrangement[y][x].num == Enemy::ENEMYTAG::BOMB)
            {       
                if ((y < 100 || y > 150) /*&& (x < 100 || x > 150)*/)
                    if (csv_file == csv_file_num::GAME) randam_scaling(Arrangement[y][x].num);

                BombEnemy* bombEnemy = new BombEnemy();
                bombEnemy->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));
                bombEnemy->SetScale({ randam_scale, randam_scale, randam_scale });
                enemyManager.Register(bombEnemy, Enemy::ENEMYTAG::BOMB);
            }
            if (Arrangement[y][x].num == Enemy::ENEMYTAG::RARE)
            {

                RareEnemy* rareEnemy = new RareEnemy();
                rareEnemy->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));
                enemyManager.Register(rareEnemy, Enemy::ENEMYTAG::RARE);
            }
        }
    }
}

void Enemy_Arrangement::randam_scaling(int Arrangement_num)
{
    if (Enemy::ENEMYTAG::NORMAL == Arrangement_num)
    {
        /*if (normal_randam == true)
        {
            randam_num = rand() % 3;
            normal_randam = false;

            if ((normal_enemy_num[SMALL_SIZE] != normal_enemy_num[MEDIUM_SIZE]) || (normal_enemy_num[SMALL_SIZE] != normal_enemy_num[LARGE_SIZE]))
            {
                if (normal_enemy_num[SMALL_SIZE] <= normal_enemy_num[MEDIUM_SIZE])
                {
                    if (normal_enemy_num[SMALL_SIZE] < normal_enemy_num[LARGE_SIZE]) randam_num = SMALL_SIZE;
                    else randam_num = LARGE_SIZE;
                }
                else if(normal_enemy_num[MEDIUM_SIZE] <= normal_enemy_num[LARGE_SIZE]) randam_num = MEDIUM_SIZE;
                else randam_num = LARGE_SIZE;
            }
        }
        else
        {
            randam_num = -1;
            normal_randam = true;
        }*/
        
        if (normal_randam >= 2)
        {
            randam_num = -1;
            normal_randam = 0;
        }
        else
        {
            randam_num = rand() % 3;
            if ((normal_enemy_num[SMALL_SIZE] != normal_enemy_num[MEDIUM_SIZE]) || (normal_enemy_num[SMALL_SIZE] != normal_enemy_num[LARGE_SIZE]))
            {
                if (normal_enemy_num[SMALL_SIZE] <= normal_enemy_num[MEDIUM_SIZE])
                {
                    if (normal_enemy_num[SMALL_SIZE] < normal_enemy_num[LARGE_SIZE]) randam_num = SMALL_SIZE;
                    else randam_num = LARGE_SIZE;
                }
                else if (normal_enemy_num[MEDIUM_SIZE] <= normal_enemy_num[LARGE_SIZE]) randam_num = MEDIUM_SIZE;
                else randam_num = LARGE_SIZE;
            }
            normal_randam++;
        }

        switch (randam_num)
        {
        case SMALL_SIZE:
            if (normal_enemy_num[SMALL_SIZE] < normal_enemy_limit_num)
            {
                randam_scale = 2;
                normal_enemy_num[SMALL_SIZE]++;
                 break;
            }
        case MEDIUM_SIZE:
            if (normal_enemy_num[MEDIUM_SIZE] < normal_enemy_limit_num)
            {
                randam_scale = 3;
                normal_enemy_num[MEDIUM_SIZE]++;
                break;
            }
        case LARGE_SIZE:
            if (normal_enemy_num[LARGE_SIZE] < normal_enemy_limit_num)
            {
                randam_scale = 4;
                normal_enemy_num[LARGE_SIZE]++;
                break;
            }
        default:
            randam_scale = 1;
            break;
        }

    }

    if (Enemy::ENEMYTAG::BOMB == Arrangement_num)
    {
        if (bomb_randam == true)
        {
            randam_num = rand() % 3;
            bomb_randam = false;
        }
        else
        {
            randam_num = -1;
            bomb_randam = true;
        }

        /*if (bomb_randam >= 2)
        {
            randam_num = rand() % 3;
            bomb_randam = 0;
        }
        else
        {
            randam_num = -1;
            bomb_randam++;
        }*/

        switch (randam_num)
        {
        case SMALL_SIZE:
            if (bomb_enemy_num[SMALL_SIZE] < bomb_enemy_limit_num)
            {
                randam_scale = 2;
                bomb_enemy_num[SMALL_SIZE]++;
                break;
            }
        case MEDIUM_SIZE:
            if (bomb_enemy_num[MEDIUM_SIZE] < bomb_enemy_limit_num)
            {
                randam_scale = 3;
                bomb_enemy_num[MEDIUM_SIZE]++;
                break;
            }
        case LARGE_SIZE:
            if (bomb_enemy_num[LARGE_SIZE] < bomb_enemy_limit_num)
            {
                randam_scale = 4;
                bomb_enemy_num[LARGE_SIZE]++;
                break;
            }
        default:
            randam_scale = 1;
            break;
        }
    }
}
