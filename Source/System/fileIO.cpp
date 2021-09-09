

#include "fileIO.h"
#include <iostream>
#include <fstream>




void FileIO::Open(const char* filename, FileData& data_)
{
    std::ifstream fin(filename, std::ios::in | std::ios::binary);
    //  ファイルを開く
    //  ios::in は読み込み専用  ios::binary はバイナリ形式


    if (!fin) return;
    //  ファイルが開けなかったときの対策


    size_t i = 0;

    while (!fin.eof()) {  //ファイルの最後まで続ける

        if (data_.data_array.size() <= i) break;

        fin.read((char*)&data_.data_array.at(i), sizeof(int));
        //文字列ではないデータを読みこむ
        std::cout << data_.data_array.at(i) << std::endl;

        i++;
    }

    fin.close();  //ファイルを閉じる
}


void FileIO::Write(const char* filename, FileData& data_)
{
    std::ofstream fout;
    fout.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    //  ファイルを開く
    //  ios::out は書き込み専用（省略可）
    //  ios::binary はバイナリ形式で出力（省略するとアスキー形式で出力）
    //  ios::truncはファイルを新規作成（省略可）
    //  ios::addにすると追記になる

    if (!fout) return;
    //  ファイルが開けなかったときの対策

    for (size_t i = 0; i < data_.data_array.size(); ++i) {

        fout.write((char*)&data_.data_array.at(i), sizeof(int));
        //文字列ではないデータをかきこむ
        // 「sizeof( double )」バイトの「char *」ポインタ「a[i]」をデータとして出力
    }

    fout.close();  //ファイルを閉じる
}