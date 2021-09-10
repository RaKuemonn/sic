

#include "stage_room.h"



StageRoom::StageRoom()
{
    mdl_stage = std::make_unique<Model>("Data/Model/Test/test_wall_floor.mdl");
}

StageRoom::~StageRoom()
{

}