

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


    // buttom‚ª‰Ÿ‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î
    else
    {
        // À‚Í ChangeNextSceneŠÖ”‚ªŒÄ‚Î‚ê‚é“x‚É,ˆø”‚Ånew‚É‚æ‚Á‚Ä¶¬‚³‚ê‚Ä‚¢‚é‚½‚ßíœ‚µ‚È‚¯‚ê‚Î‚È‚ç‚È‚¢ (ƒJƒX)
        safe_delete(next_scene_);
    }
}