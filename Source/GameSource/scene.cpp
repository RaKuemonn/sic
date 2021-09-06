

#include "scene.h"
#include "sceneManager.h"



void Scene::Init()
{

    Load();
    Set();
}

void Scene::ChangeNextScene(Scene* next_scene_, GamePadButton buttom, bool isloading)
{
    if (Input::Instance().GetGamePad().GetButtonDown() & buttom)
    {
        isloading ? /* true */sceneManager.SetLoadNextScene(next_scene_) : /* false */sceneManager.SetNextScene(next_scene_);

        next_scene_ = nullptr;
    }


    // buttomが押されていなければ
    else
    {
        // 実は ChangeNextScene関数が呼ばれる度に,引数でnewによって生成されているため削除しなければならない (カス)
        safe_delete(next_scene_);
    }
}