#pragma once

#include "defines.h"

bool isJump(rabbit_t& rabbit);
bool rabbitOnWater(rabbit_t& rabbit);
int  rabbitOnTree(rabbit_t& rabbit, std::vector<tree_t>& trees);
void rabbitToStart(rabbit_t& rabbit);

void spawn_V(simulation_t& sim);
void spawn_V_Start(simulation_t& sim);
void spawn_Tree(simulation_t& sim);
void spawn_Tree_Start(simulation_t& sim);
void updateVehicles(simulation_t& sim);
void updateTrees(simulation_t& sim);
