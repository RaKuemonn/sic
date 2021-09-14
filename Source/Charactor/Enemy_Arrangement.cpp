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

void Enemy_Arrangement::enemy_produce(int csv_file)
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

            if (Arrangement[y][x].num == Enemy::ENEMYTAG::NORMAL)
            {
                NormalEnemy* normalEnemy = new NormalEnemy();
                normalEnemy->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));
                enemyManager.Register(normalEnemy, Enemy::ENEMYTAG::NORMAL);
            }
            if (Arrangement[y][x].num == Enemy::ENEMYTAG::BOMB)
            {       
                BombEnemy* bombEnemy = new BombEnemy();
                bombEnemy->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));
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


