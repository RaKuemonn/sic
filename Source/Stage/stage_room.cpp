

#include "stage_room.h"



StageRoom::StageRoom()
{
    mdl_stage = std::make_unique<Model>("Data/Model/Test/test_wall_floor.mdl");
    mdl_stage->UpdateTransform({ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 });
}

StageRoom::~StageRoom()
{

}