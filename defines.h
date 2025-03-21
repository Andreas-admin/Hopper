#pragma once

#include <stdint.h>
#include <raylib.h>
#include <vector>
#include <fstream>
#include "datum.hpp"

#define LEVEL_WIDTH 15
#define LEVEL_HEIGHT 13

#define BORDERCOLOR LIGHTGRAY

#define rabbit_LIFE 5

#define ADD_SPEED 1.3f
#define SPAWNTIME 180
#define PLAYTIME 30000

#define DISPLAY_TEXT
#define MAX_INPUT_CHARS 10

extern Font fontTtf;
extern float window_width;
extern float window_height;
extern float field_size;
extern float top_height;
extern float bottom_hight;
extern int border_width;

extern Rectangle rec_top;
extern Rectangle rec_left;
extern Rectangle rec_right;
extern Rectangle rec_bottom;


extern Rectangle rec_GameOver;
extern Rectangle rec_pause;
extern Rectangle rec_timeOver;
extern Rectangle rec_menuWindow;
extern Rectangle rec_menuButton1;
extern Rectangle rec_menuButton2;
extern Rectangle rec_menuButton3;
extern Rectangle rec_menuButtonBack;
extern Rectangle rec_textBox;

extern std::string name;

extern Rectangle rec_bestenliste;

extern Vector2 posPoints;
extern Vector2 posLife;
extern Vector2 posGameOver;
extern Vector2 posYourPoints;
extern Vector2 posNameText;

extern Rectangle timeDisplay;
extern Color timeColor;
extern Color colorB1;
extern Color colorB2;
extern Color colorB3;


extern Vector2 start_level;
extern float speed_slow;
extern float speed_fast;

extern float speed_slow_Tree;
extern float speed_fast_Tree;

extern int spawnstep;

extern bool stop;
extern bool goHome;
extern bool menuOn;
extern bool bestenlisteOn;
extern bool bestenlisteEintrag;
extern bool mouseOnText;

struct Bestenliste
{
    struct Eintrag
    {
        std::string name;
        int punkte;
        Datum datum;
        Eintrag(const std::string& name, int punkte);
        Eintrag(const std::string& name, int punkte, Datum datum);
    
        std::string speichern() const;
        std::string anzeigen() const;
    };
    
    std::vector<Eintrag> eintraege;
    std::string dateiname;

    void laden();
    void speichern() const;

    public:
    Bestenliste(const std::string& dateiname = "bestenliste.csv");
    ~Bestenliste();
    
    void ausgeben() const;
    void neuerEintrag(const std::string& name, int punkte);
};



enum texture_data
{
    CAR1,
    CAR2,
    CAR3,
    LIMO,
    TRUCK,
    CAR1_L,
    CAR2_L,
    CAR3_L,
    LIMO_L,
    TRUCK_L,

    GRASS,
    WATER,
    ROAD,
    DEAD,

    TREE_L,
    TREE_M,
    TREE_S,

    BORDER,
    RABBIT_DOWN,
    RABBIT_UP,
    RABBIT_RIGHT,
    RABBIT_LEFT
};

enum street
{
    STREET_1 = 1,
    STREET_2,
    STREET_3,
    STREET_4,
    STREET_5,
    STREET_6
};

enum waterline
{
    LINE_1 = 1,
    LINE_2,
    LINE_3,
    LINE_4,
    LINE_5
};

struct rabbit_t
{
    Vector2 startPos;
    Vector2 startPosColision;
    Rectangle rec;
    Rectangle recColision;
    int life;
    int points;
    texture_data direction;
};

struct vehicle_t
{
    Rectangle rec;
    float speed;
    int length;
    int t_index;
    int street;
};

struct tree_t
{
    Rectangle rec;
    float speed;
    int length;
    int t_index;
    int line;
};

struct simulation_t
{
    rabbit_t rabbit;
    std::vector<Texture2D> textures;
    std::vector<vehicle_t> vehicles;
    std::vector<tree_t>    trees;
};

