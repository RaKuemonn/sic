

#include "scene.h"
#include "sceneManager.h"



void Scene::Init(SceneManager* scene_manager_)
{
    scene_manager = scene_manager_;
    Load();
    Set();
}

void Scene::ChangeNextScene(Scene* next_scene_, GamePadButton buttom, bool isloading)
{
    if (Input::Instance().GetGamePad().GetButtonDown() & buttom)
    {
        isloading ? /* true */scene_manager->SetLoadNextScene(next_scene_) : /* false */scene_manager->SetNextScene(next_scene_);

        next_scene_ = nullptr;
    }


    // buttomが押されていなければ
    else
    {
        // 実は ChangeNextScene関数が呼ばれる度に,引数でnewによって生成されているため削除しなければならない (カス)
        safe_delete(next_scene_);
    }
}


void Scene::ChangeNextScene(Scene* next_scene_, bool isloading)
{
    isloading ? /* true */scene_manager->SetLoadNextScene(next_scene_) : /* false */scene_manager->SetNextScene(next_scene_);

    next_scene_ = nullptr;
}