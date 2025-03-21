#include "logic.h"


bool isJump(rabbit_t& rabbit)
{
    bool jump = false;
    if (IsKeyDown(KEY_UP))
    {
        rabbit.direction = RABBIT_UP;
        float y = rabbit.rec.y;
        y--;
        if (y - (field_size) > (start_level.y - field_size))
        {
            rabbit.rec.y -= field_size;
            rabbit.recColision.y -= field_size;
        }
        jump = true;
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        rabbit.direction = RABBIT_DOWN;
        float y = rabbit.rec.y;
        y++;
        if ((y + field_size) <= (start_level.y + (field_size * (LEVEL_HEIGHT + 1))))
        {
            rabbit.rec.y += field_size;
            rabbit.recColision.y += field_size;
        } 
        jump = true;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        rabbit.direction = RABBIT_LEFT;
        float x = rabbit.rec.x;
        x--;
        if ((x - field_size) > (start_level.x - field_size))
        {
            rabbit.rec.x -= field_size;
            rabbit.recColision.x -= field_size;
        } 
        jump = true;
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        rabbit.direction = RABBIT_RIGHT;
        float x = rabbit.rec.x;
        x++;
        if (x + (field_size) < start_level.x + (field_size * LEVEL_WIDTH))
        {
            rabbit.rec.x += field_size;
            rabbit.recColision.x += field_size;
        } 
        jump = true;
    }

    if (!rabbitOnWater(rabbit))
    {
        int rabbitPos = rabbit.rec.x - start_level.x;
        int diff = rabbitPos % (int)field_size;
        if (diff < (field_size / 2) && diff > 0)
        {
            rabbit.rec.x -= diff;
            rabbit.recColision.x -= diff;
        }
        else if(diff > 0)
        {
            rabbit.rec.x += (field_size - diff);
            rabbit.recColision.x += (field_size - diff);
        }
    }
    
    
    return jump;
}

bool rabbitOnWater(rabbit_t& rabbit)
{
    if (rabbit.rec.y >= (top_height + field_size) && rabbit.rec.y <= (top_height + (field_size * LINE_5)))
    {
        return true;
    }
    return false;
}

int rabbitOnTree(rabbit_t& rabbit, std::vector<tree_t>& trees)
{
    for (int i = 0; i < trees.size(); i++)
    {
        if (CheckCollisionRecs(rabbit.recColision, trees[i].rec))
        {
            return i;
        }
    }
    return -1;
}

void rabbitToStart(rabbit_t& rabbit)
{
    rabbit.rec.x = rabbit.startPos.x;
    rabbit.rec.y = rabbit.startPos.y;
    rabbit.recColision.x = rabbit.startPosColision.x;
    rabbit.recColision.y = rabbit.startPosColision.y;
}

void spawn_V(simulation_t& sim)
{
    static int streetOld = 0;
    static int street = 0;
    while (street == streetOld)
    {
        street = (rand() % 6) + 1;
    }

    streetOld = street;

    int car = rand() % (TRUCK + 1);
    int length = 2;
    
    if (car == LIMO)
    {
        length = 3;
    }
    else if (car == TRUCK)
    {
        length = 4;
    }

    float posX = start_level.x - (field_size * length);
    float speed = speed_slow;

    if (street == STREET_1 || street == STREET_2 || street == STREET_5)
    {
        car += 5;
        posX = start_level.x + (field_size * LEVEL_WIDTH);
        speed = speed_fast;
    }
    
    vehicle_t v{{posX, start_level.y + (field_size * (LEVEL_HEIGHT - street)), 
                (float)(field_size * length), field_size},
                speed, length, car, street};

    for (auto& vS : sim.vehicles)
    {
        if (vS.street == street)
        {
            if (CheckCollisionRecs(vS.rec, v.rec))
            {
                return;
            }  
        } 
    }
    
    sim.vehicles.push_back(v);
}

void spawn_V_Start(simulation_t& sim)
{
    int counter = 0;
    while (counter < 6)
    {
        int simSize = sim.vehicles.size();
        spawn_V(sim);
        if (sim.vehicles.size() > simSize)
        {
            if (sim.vehicles[simSize].street == STREET_1 || sim.vehicles[simSize].street == STREET_2 || sim.vehicles[simSize].street == STREET_5)
            {
                sim.vehicles[simSize].rec.x -= (rand() % 41) * 10 ;
            }
            else
            {
                sim.vehicles[simSize].rec.x += (rand() % 41) * 10 ;
            }
            counter++;
        }
    }
}

void spawn_Tree(simulation_t& sim)
{
    static int lineOld = 0;
    static int line = 0;
    while (line == lineOld)
    {
        line = (rand() % (LINE_5)) + 1;
    }

    lineOld = line;

    int tree = (rand() % 3) + TREE_L;
    int length = 3;
    
    if (tree == TREE_M)
    {
        length = 2;
    }
    else if (tree == TREE_S)
    {
        length = 1;
    }

    float posX = start_level.x - (field_size * length);
    float speed = speed_slow;

    if (line == LINE_1 || line == LINE_3 || line == LINE_5)
    {
        posX = start_level.x + (field_size * LEVEL_WIDTH);
        speed = speed_fast;
    }
    
    tree_t t{{posX, start_level.y + (field_size * line), 
                (float)(field_size * length), field_size},
                speed, length, tree, line};

    for (auto& tS : sim.trees)
    {
        if (tS.line == line)
        {
            if (CheckCollisionRecs(tS.rec, t.rec))
            {
                return;
            }  
        } 
    }
    
    sim.trees.push_back(t);
}

void spawn_Tree_Start(simulation_t& sim)
{
    int counter = 0;
    while (counter < 10)
    {
        int simSize = sim.trees.size();
        spawn_Tree(sim);
        if (sim.trees.size() > simSize)
        {
            if (sim.trees[simSize].line == LINE_1 || sim.trees[simSize].line == LINE_3 || sim.trees[simSize].line == LINE_5)
            {
                sim.trees[simSize].rec.x -= (rand() % 41) * 10 ;
            }
            else
            {
                sim.trees[simSize].rec.x += (rand() % 41) * 10 ;
            }
            counter++;
        }
    }
}

void updateVehicles(simulation_t& sim)
{
    for (int i = 0; i < sim.vehicles.size(); i++)
        {
            if (sim.vehicles[i].t_index < 5)
            {
                sim.vehicles[i].rec.x += sim.vehicles[i].speed;
                
                if (sim.vehicles[i].rec.x > start_level.x + (field_size * LEVEL_WIDTH))
                {
                    sim.vehicles.erase(std::next(sim.vehicles.begin(), i));
                }
            }
            else
            {
                sim.vehicles[i].rec.x -= sim.vehicles[i].speed;
                
                if (sim.vehicles[i].rec.x < start_level.x - (sim.vehicles[i].length * field_size))
                {
                    sim.vehicles.erase(std::next(sim.vehicles.begin(), i));
                }
            }
        }
}

void updateTrees(simulation_t& sim)
{
    for (int i = 0; i < sim.trees.size(); i++)
        {
            if (sim.trees[i].line == LINE_1 || sim.trees[i].line == LINE_3 || sim.trees[i].line == LINE_5)
            {
                
                sim.trees[i].rec.x -= sim.trees[i].speed;
                
                if (sim.trees[i].rec.x < start_level.x - (sim.trees[i].length * field_size))
                {
                    sim.trees.erase(std::next(sim.trees.begin(), i));
                }
            }
            else
            {
                sim.trees[i].rec.x += sim.trees[i].speed;
                
                if (sim.trees[i].rec.x > start_level.x + (field_size * LEVEL_WIDTH))
                {
                    sim.trees.erase(std::next(sim.trees.begin(), i));
                }
            }
        }
}
