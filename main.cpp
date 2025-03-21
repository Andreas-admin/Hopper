#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <chrono>

#include "defines.h"
#include "init.h"
#include "logic.h"

simulation_t sim;
Bestenliste bestenliste{"bestenliste.csv"};

Vector2 killPos;
bool collision = false;
bool rabbitTree = false;

float jumptime = .1f;
float spawntime = 0.f;
double spawn = 0;
float spawntimeTree = 0.f;
double spawnTree = 0;
int frames = 0;

void newGame();
void showBestenliste();
void showBestenlisteEintrag();
void initBestenlisteEintrag();

int main()
{   
    initWindow(30);

    InitWindow(window_width, window_height, "HOPPER");
    SetTargetFPS(60);
    
    initTexture(sim.textures);
    
    newGame();
    spawn_V_Start(sim);
    spawn_Tree_Start(sim);
    
    double timeG = GetFrameTime();

    fontTtf = LoadFontEx("fonds/SuperMario256.ttf", field_size * 2, 0, 250);
    
    auto endG = std::chrono::high_resolution_clock::now();
    long long diffTime = 0;
    float diffSize = 0;
    auto startG = std::chrono::high_resolution_clock::now();

    while (!WindowShouldClose() && !goHome)
    {
        if (sim.rabbit.life > 0 && !stop)
        {
            // Zeitkonstante festlegen
            float delta = GetFrameTime();
            timeG += delta;  

            int spawndelta = SPAWNTIME - (spawnstep*2);
            if (spawndelta < 20)
            {
                spawndelta = 20;
            }
            
            if (timeG - spawntime > spawn)
            {
                spawn_V(sim);
                spawntime = timeG;
                spawn = ((rand() % 100) + spawndelta) / 100;
            }

            spawndelta = (SPAWNTIME / 2) - spawnstep;
            if (spawndelta < 10)
            {
                spawndelta = 10;
            }
            if (timeG - spawntimeTree > spawnTree)
            {
                spawn_Tree(sim);
                spawntimeTree = timeG;
                spawnTree = ((rand() % 100) + spawndelta) / 100;
            }

            if (timeG - jumptime > 0.15)
            {
                if (isJump(sim.rabbit))
                {
                    jumptime = timeG;
                }
            }
                
            updateVehicles(sim);
            
            for (auto v : sim.vehicles)
            {
                collision = CheckCollisionRecs(sim.rabbit.recColision, v.rec);
                if (collision)
                {
                    sim.rabbit.life--;
                    killPos = {sim.rabbit.rec.x, sim.rabbit.rec.y};
                    rabbitToStart(sim.rabbit);
                    break;
                }
            }

            updateTrees(sim);
            if (rabbitOnWater(sim.rabbit))
            {
                int tree = rabbitOnTree(sim.rabbit, sim.trees);
                if (tree != -1)
                {
                    rabbitTree = true;
                    float x = sim.rabbit.rec.x;
                    if (sim.trees[tree].line == LINE_1 || sim.trees[tree].line == LINE_3 || sim.trees[tree].line == LINE_5)
                    {
                        x--;
                        if ((x - field_size) > (start_level.x - field_size))
                        {
                            sim.rabbit.rec.x -= sim.trees[tree].speed;
                            sim.rabbit.recColision.x -= sim.trees[tree].speed;
                        }
                    }
                    else
                    {   
                        x++;
                        if (x + (field_size) < start_level.x + (field_size * LEVEL_WIDTH))
                        {
                            sim.rabbit.rec.x += sim.trees[tree].speed;
                            sim.rabbit.recColision.x += sim.trees[tree].speed;
                        }
                    }
                }
                else
                {
                    collision = true;
                    sim.rabbit.life--;
                    killPos = {sim.rabbit.rec.x, sim.rabbit.rec.y};
                    rabbitToStart(sim.rabbit);
                } 
            }
            
            // RESTZEIT
            endG = std::chrono::high_resolution_clock::now();
            diffTime = std::chrono::duration_cast<std::chrono::milliseconds>(endG - startG).count();
            diffSize = (diffTime / (PLAYTIME / 100.0)) / 100;
            if (diffSize > 0.5)
            {
                timeColor = diffSize > 0.75 ? RED : YELLOW;
            }
            else
            {
                timeColor = GREEN;
            }
            
            // Zeit abgelaufen ? //////////////////////////
            if ((PLAYTIME - diffTime) < 0)
            {
                sim.rabbit.life--;
                killPos = {sim.rabbit.rec.x, sim.rabbit.rec.y};
                rabbitToStart(sim.rabbit);
            }
            
            // Frosch im Ziel ? //////////////////////////
            if (sim.rabbit.rec.y == start_level.y)
            {
                sim.rabbit.points += (PLAYTIME - diffTime) / 10;
                rabbitToStart(sim.rabbit);

                spawnstep += 15;
                
                // Speed anpassen
                speed_slow *= ADD_SPEED;
                speed_fast *= ADD_SPEED;

                speed_slow_Tree *= ADD_SPEED;
                speed_fast_Tree *= ADD_SPEED;
                for(auto& v : sim.vehicles)
                {
                    v.speed *= ADD_SPEED;
                }
                for(auto& t : sim.trees)
                {
                    t.speed *= ADD_SPEED;
                }
                startG = std::chrono::high_resolution_clock::now();
            }
        }

        // Window ////////////////////////
        BeginDrawing();

        ClearBackground(BLACK);

        if (!menuOn && !bestenlisteOn && !bestenlisteEintrag)
        {   
            initLevel(sim.textures);

            // VEHICLES //////////////////////
            for (auto v : sim.vehicles)
            {
                DrawTexture(sim.textures[v.t_index], v.rec.x, v.rec.y + 6, WHITE);
            }

            // TREES //////////////////////
            for (auto t : sim.trees)
            {
                DrawTexture(sim.textures[t.t_index], t.rec.x, t.rec.y + 6, WHITE);
            }

            // rabbit //////////////////////
            DrawTextureV(sim.textures[sim.rabbit.direction], {sim.rabbit.rec.x, sim.rabbit.rec.y}, WHITE);
            
            // DEAD /////////////////////
            if (collision)
            {
                DrawTexture(sim.textures[DEAD], killPos.x, killPos.y, WHITE);
            }
            else if((PLAYTIME - diffTime) < 0)
            {
                DrawTexture(sim.textures[DEAD], killPos.x, killPos.y, WHITE);
                //DrawRectangleRec(rec_timeOver, BORDERCOLOR);
                DrawTextureRec(sim.textures[BORDER], rec_timeOver, {rec_timeOver.x, rec_timeOver.y}, WHITE);
                DrawTextEx(fontTtf, "   Zeit ist\n abgelaufen ", {rec_timeOver.x - 5, rec_timeOver.y + field_size * 0.5f}, field_size * 1.5, 4, BLACK);
            }

            // Umrandung //////////////////
            DrawTextureRec(sim.textures[BORDER], rec_top, {rec_top.x, rec_top.y}, WHITE);
            DrawTextureRec(sim.textures[BORDER], rec_left, {rec_left.x, rec_left.y}, WHITE);
            DrawTextureRec(sim.textures[BORDER], rec_right, {rec_right.x, rec_right.y}, WHITE);
            DrawTextureRec(sim.textures[BORDER], rec_bottom, {rec_bottom.x, rec_bottom.y}, WHITE);

            // Anzeigen //////////////////
            std::string text = "LEBEN:" + std::to_string(sim.rabbit.life);
            DrawTextEx(fontTtf, text.c_str(), posLife, field_size, 4, BLACK);

            text = "Punkte:" + std::to_string(sim.rabbit.points);
            DrawTextEx(fontTtf, text.c_str(), posPoints, field_size, 4, BLACK);
        
            DrawRectangleRec(timeDisplay, BLACK);
            DrawRectangle(timeDisplay.x + 2, timeDisplay.y + 2, (timeDisplay.width - 4) - (timeDisplay.width * diffSize), timeDisplay.height - 4, timeColor);

            DrawTextEx(fontTtf, TextFormat("Zeit:%.2f", (PLAYTIME - (diffTime))/1000.0), {timeDisplay.x + (field_size * 4), (timeDisplay.y + 3)}, field_size, 4, WHITE);

            if (sim.rabbit.life == 0 && !menuOn)
            {
                DrawTextureRec(sim.textures[BORDER], rec_GameOver, {rec_GameOver.x, rec_GameOver.y}, WHITE);
                DrawTextEx(fontTtf, TextFormat("!!! Game Over !!!"), posGameOver, field_size*2, 4, BLACK);
                DrawTextEx(fontTtf, TextFormat("Deine Punkte:%d", sim.rabbit.points), posYourPoints, field_size, 4, BLACK);
            }

        }    
        // MENU ///////////////////////////////////////////////////////////////////
        if (menuOn)
        {
            //DrawRectangleRec({0,0,window_width,window_height}, GREEN);
            DrawTextureRec(sim.textures[BORDER], {0,0,window_width,window_height}, {0, 0}, WHITE);

            if (CheckCollisionPointRec(GetMousePosition(), rec_menuButton1))
            {
                DrawRectangleRec(rec_menuButton1, LIGHTGRAY);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    stop = false;
                    menuOn = false;
                    initrabbit(sim.rabbit);
                    newGame();
                    startG = std::chrono::high_resolution_clock::now();
                }
            }
            else
            {
                DrawRectangleRec(rec_menuButton1, GRAY);;
            }
            
            if (CheckCollisionPointRec(GetMousePosition(), rec_menuButton2))
            {
                DrawRectangleRec(rec_menuButton2, LIGHTGRAY);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    bestenlisteOn = true;
                }
            }
            else
            {
                DrawRectangleRec(rec_menuButton2, GRAY);
            }

            if (CheckCollisionPointRec(GetMousePosition(), rec_menuButton3))
            {
                DrawRectangleRec(rec_menuButton3, LIGHTGRAY);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    goHome = true;
                }
            }
            else
            {
                DrawRectangleRec(rec_menuButton3, GRAY);
            }
            
            DrawTextEx(fontTtf, TextFormat("Neues Spiel"), {rec_menuButton1.x + field_size, rec_menuButton1.y + 3 + (field_size/2)}, field_size, 4, BLACK);
            DrawTextEx(fontTtf, TextFormat("Bestenliste"), {rec_menuButton2.x + field_size, rec_menuButton2.y + 3 + (field_size/2)}, field_size, 4, BLACK);
            DrawTextEx(fontTtf, TextFormat("Beenden"), {rec_menuButton3.x + (field_size * 2) + 20, rec_menuButton3.y + 3 + (field_size/2)}, field_size, 4, BLACK);
        }
        
        if (bestenlisteOn)
        {
            showBestenliste();
            bestenliste.ausgeben();
        }
        
        if (bestenlisteEintrag)
        {
            showBestenlisteEintrag();
        }
        /////////////////////////////////////////////////////////////////////
        
        



        EndDrawing();

        if (collision || ((PLAYTIME - diffTime) < 0))
        {
            if (frames == 120)
            {
                stop = false;
                startG = std::chrono::high_resolution_clock::now();
                frames = 0;
            }
            else
            {
                stop = true;
                frames++;
            } 
        }

        if (sim.rabbit.life == 0 && !menuOn && !bestenlisteEintrag)
        {
            if (frames == 180)
            {
                if (bestenliste.eintraege.size() < 10 && sim.rabbit.points)
                {
                    bestenlisteEintrag = true;
                    frames = 0;
                }
                else if(sim.rabbit.points > bestenliste.eintraege[9].punkte)
                {   
                    bestenlisteEintrag = true;
                    frames = 0;      
                }
            
                if (!bestenlisteEintrag)
                {
                    menuOn = true;
                    frames = 0;
                }
            }
            else
            {
                frames++;
            }
        }
        
        if (bestenlisteEintrag)
        {
            initBestenlisteEintrag();
        }  
    }

    for (auto t : sim.textures)
    {
        UnloadTexture(t);  
    }
    
    return 0;
}

void newGame()
{
    speed_slow = 0.5f;
    speed_fast = 1.0f;

    speed_slow_Tree = 0.2f;
    speed_fast_Tree = 0.8f;
    spawnstep = 0;
}

void showBestenliste()
{
    DrawTextureRec(sim.textures[BORDER], {0,0,window_width,window_height}, {0, 0}, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), rec_menuButtonBack))
    {
        DrawRectangleRec(rec_menuButtonBack, LIGHTGRAY);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
           bestenlisteOn = false;
        }
    }
    else
    {
        DrawRectangleRec(rec_menuButtonBack, GRAY);;
    }
    
    DrawTextEx(fontTtf, TextFormat("Beenden"), {rec_menuButtonBack.x + 15, rec_menuButtonBack.y + 3}, field_size, 4, BLACK);
}

void showBestenlisteEintrag()
{
    bool mouseOnText = false;
    DrawTextureRec(sim.textures[BORDER], {0,0,window_width,window_height}, {0, 0}, WHITE);
    DrawRectangleRec(rec_textBox, GRAY);
    DrawTextEx(fontTtf, name.c_str(), {rec_textBox.x + 5, rec_textBox.y + (field_size / 2)}, field_size, 4, BLACK);
    if (mouseOnText)
    {   
        DrawRectangleLines((int)rec_textBox.x, (int)rec_textBox.y, (int)rec_textBox.width, (int)rec_textBox.height, RED);
    }
    else 
    {
        DrawRectangleLines((int)rec_textBox.x, (int)rec_textBox.y, (int)rec_textBox.width, (int)rec_textBox.height, DARKGRAY);
    }

    DrawTextEx(fontTtf, " Top 10\nErgebnis", {posNameText.x, posNameText.y - (field_size * 6)},field_size * 2, 4, BLACK);
    DrawTextEx(fontTtf, "Bitte Namen\neingeben:", posNameText,field_size, 4, BLACK);
    if (mouseOnText)
    {
        if (name.size() < MAX_INPUT_CHARS)
        {
            if (((frames/20)%2) == 0)
            {
                DrawTextEx(fontTtf, "_", {rec_textBox.x + MeasureText(name.c_str(), field_size), rec_textBox.y + 12}, field_size, 4, MAROON);
            }    
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), rec_menuButtonBack))
    {
        DrawRectangleRec(rec_menuButtonBack, LIGHTGRAY);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            bestenlisteEintrag = false;
           
            bestenliste.neuerEintrag(name, sim.rabbit.points);
           
            name = "";
            menuOn = true;
        }
    }
    else
    {
        DrawRectangleRec(rec_menuButtonBack, GRAY);;
    }
    
    DrawTextEx(fontTtf, TextFormat("OK"), {rec_menuButtonBack.x + (field_size * 2), rec_menuButtonBack.y + 3}, field_size, 4, BLACK);


}

void initBestenlisteEintrag()
{
    mouseOnText = CheckCollisionPointRec(GetMousePosition(), rec_textBox) ? true : false;
    
    if (mouseOnText)
    {
        // Set the window's cursor to the I-Beam
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (name.size() < MAX_INPUT_CHARS))
            {
                name += (char)key;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE) && name.size() > 0)
        {
            name.erase(name.end() - 1);
        }
    }
    else 
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
     
    frames = mouseOnText ? ++frames : 0;
}