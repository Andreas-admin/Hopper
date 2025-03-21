#include "defines.h"

Font fontTtf;

float   window_width;
float   window_height;
float   field_size;
float   top_height;
float   bottom_hight;
int     border_width;

Rectangle rec_top;
Rectangle rec_left;
Rectangle rec_right;
Rectangle rec_bottom;

Rectangle rec_GameOver;
Rectangle rec_pause;
Rectangle rec_timeOver;

Rectangle rec_menuWindow;
Rectangle rec_menuButton1;
Rectangle rec_menuButton2;
Rectangle rec_menuButton3;
Rectangle rec_bestenliste;
Rectangle rec_menuButtonBack;
Rectangle rec_textBox;

Vector2 posPoints;
Vector2 posLife;
Vector2 posGameOver;
Vector2 posYourPoints;
Vector2 posNameText;

Rectangle timeDisplay;
Color timeColor;

std::string name = "";

float   startLevel_x;
float   startLevel_y;

Vector2 start_level;
float   speed_slow;
float   speed_fast;

float   speed_slow_Tree;
float   speed_fast_Tree;

int     spawnstep;

bool stop = true;
bool goHome = false;
bool menuOn = true;
bool bestenlisteOn = false;
bool bestenlisteEintrag = false;
bool mouseOnText = false;

Color colorB1;
Color colorB2;
Color colorB3;


Bestenliste::Eintrag::Eintrag(const std::string& name, int punkte) : name(name), punkte(punkte)
{
    datum.setDatum();
}

Bestenliste::Eintrag::Eintrag(const std::string& name, int punkte, Datum datum) : name(name), punkte(punkte), datum(datum)
{}

std::string Bestenliste::Eintrag::speichern() const
{
    return name + ',' + std::to_string(punkte) + ',' + datum.asString() + '\n';
}

std::string Bestenliste::Eintrag::anzeigen() const
{
    std::stringstream sstr;
    sstr << std::setfill('0') << std::setw(2) << datum.getTag() << '.' << std::setw(2) << datum.getMonat() << '.' << datum.getJahr() % 2000;
    sstr << std::setw(0) << " " << std::setw(5) << punkte << " : " << name;
    return sstr.str();
}

Bestenliste::Bestenliste(const std::string& dateiname) : dateiname(dateiname)
{
    laden();
}

Bestenliste::~Bestenliste()
{
    speichern();
}

void Bestenliste::laden()
{
    std::ifstream ifs(dateiname);
    if (!ifs.is_open())
    {
        return;
    }

    std::string zeile;
    std::string name;
    int punkte;
    int tag, monat, jahr;
    Datum datum;

    while (std::getline(ifs, zeile))
    {
        std::stringstream sstr(zeile);
        getline(sstr, name, ',');
        sstr >> punkte;
        sstr.ignore();
        sstr >> tag;
        sstr.ignore();
        sstr >> monat;
        sstr.ignore();
        sstr >> jahr;

        datum.setDatum(tag, monat, jahr);
        eintraege.emplace_back(name, punkte, datum);
    }
}

void Bestenliste::speichern() const
{
    std::ofstream ofs(dateiname);
    int zaehler = 0;
    for(auto it = eintraege.begin(); it != eintraege.end(); ++it)
    {
        ofs << it->speichern();

        if (++zaehler == 10)
        {
            break;
        }
    }
}

void Bestenliste::neuerEintrag(const std::string& name, int punkte)
{
    eintraege.emplace_back(name, punkte);
    std::sort(eintraege.begin(), eintraege.end(), [](Eintrag& a, Eintrag& b){return a.punkte > b.punkte;});
}

void Bestenliste::ausgeben() const
{
    int zaehler = 0;
    for(auto it = eintraege.begin(); it != eintraege.end(); ++it)
    {
        DrawTextEx(fontTtf,"BESTENLISTE", {rec_bestenliste.x + (field_size * 1.5f), rec_bestenliste.y + (field_size)}, field_size * 2, 4, BLACK);
        DrawTextEx(fontTtf, it->anzeigen().c_str(), {rec_bestenliste.x, rec_bestenliste.y + (field_size * 4) + (field_size * zaehler)}, field_size, 4, BLACK);

        if (++zaehler == 10)
        {
            break;
        }
    }
}