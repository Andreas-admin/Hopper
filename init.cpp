#include "init.h"

void initWindow(int fieldsize)
{
    border_width = 3;
    field_size = fieldsize;
    window_width = (field_size * LEVEL_WIDTH) + (field_size * (2 * border_width));
    top_height = field_size * 2;
    bottom_hight = field_size * border_width;
    window_height = top_height + (field_size * (LEVEL_HEIGHT + 1)) + bottom_hight;

    start_level = {((window_width - (LEVEL_WIDTH * field_size)) / 2), top_height};

    rec_top = {0, 0, window_width, top_height};
    rec_left = {0, top_height, (field_size * border_width), (window_height - top_height)};
    rec_right = {(window_width - (field_size * border_width)), top_height, (field_size * border_width), (window_height - top_height)};
    rec_bottom = {start_level.x, (window_height - (field_size * border_width)), start_level.x + (field_size * LEVEL_WIDTH), (field_size * border_width)};
    rec_pause = {(window_width / 2) - (field_size * 4), (window_height/2) + field_size * 2, field_size * 8, field_size * 4};

    posPoints = {start_level.x, start_level.y - field_size};
    posLife = {start_level.x + (field_size * (LEVEL_WIDTH * (2 / 3.0f))), start_level.y - field_size};

    timeDisplay = {start_level.x, rec_bottom.y + (field_size * 0.8f), field_size * LEVEL_WIDTH, field_size};

    rec_GameOver = {start_level.x, (start_level.y + (field_size * (LEVEL_HEIGHT / 2)) - ((field_size * 3)/2)), LEVEL_WIDTH * field_size, field_size * 6};
    posGameOver = {(window_width / 2) - (field_size * 10), (window_height/2) - field_size};
    posYourPoints = {(window_width / 2) - (field_size * 6), (window_height/2) + field_size};

    rec_pause = {(window_width / 2) - (field_size * 4), (window_height/2) - field_size * 5, field_size * 8, field_size * 10};
    rec_timeOver = {(window_width / 2) - (field_size * 6), (window_height / 2) - (field_size * 2), field_size * 12, field_size * 4};

    rec_menuWindow = {(window_width / 2) - (field_size * 6), (window_height/2) - field_size * 6, field_size * 12, field_size * 10};
    rec_menuButton1 = {rec_menuWindow.x + field_size, rec_menuWindow.y + field_size, field_size * 10, field_size * 2};
    rec_menuButton2 = {rec_menuButton1.x, rec_menuButton1.y + (field_size * 3), rec_menuButton1.width, rec_menuButton1.height};
    rec_menuButton3 = {rec_menuButton1.x, rec_menuButton1.y + (field_size * 6), rec_menuButton1.width, rec_menuButton1.height};
    
    rec_bestenliste = {(window_width / 2) - (field_size * 9), 0, field_size * 18, field_size * 18};
    rec_menuButtonBack = {(window_width / 2) - (field_size * 3), rec_bestenliste.y + rec_bestenliste.height - (field_size * 1.5f), field_size * 6, field_size};
    
    rec_textBox = {(window_width / 2) - (field_size * 5), (window_height/2) + (field_size), field_size * 10, field_size * 2};
    posNameText = {rec_textBox.x, rec_textBox.y - (field_size * 2)};
}

void initLevel(std::vector<Texture2D>& texture)
{
    int line = 0;
    int index = 0;
    
    Rectangle rec{0, 0, field_size * (LEVEL_WIDTH / 2 +1 ), field_size * 4};
    DrawTextureRec(texture[ROAD], rec, {(float)(start_level.x + (field_size * (LEVEL_WIDTH / 2))) ,(float)(start_level.y + (field_size * 7))}, WHITE);
    DrawTextureRec(texture[ROAD], rec, {(float)(start_level.x + (field_size * (LEVEL_WIDTH / 2))) ,(float)(start_level.y + (field_size * 10))}, WHITE);
    DrawTextureRec(texture[ROAD], rec, {(float)(start_level.x + (field_size * 0)) ,(float)(start_level.y + (field_size * 7))}, WHITE);
    DrawTextureRec(texture[ROAD], rec, {(float)(start_level.x + (field_size * 0)) ,(float)(start_level.y + (field_size * 10))}, WHITE);
    
    
    rec = {0,0,field_size,field_size};

    while (line <= LEVEL_HEIGHT)
    {
        if (line == 0 || line == 6 || line == LEVEL_HEIGHT)
        {
            index = GRASS;
        }
        else if (line > 0 && line < 6)
        {
            index = WATER;
        }

        for (int i = 0; i < LEVEL_WIDTH; i++)
        {
            DrawTextureRec(texture[index], rec, {(float)(start_level.x + (field_size * i)) ,(float)(start_level.y + (field_size * line))}, WHITE);
            //DrawRectangle((float)(start_levelX + (field_size * i)), (float)(start_levelY + (field_size * line)), field_size, field_size, BLUE);
        }

        if (line == 6)
        {
            line = LEVEL_HEIGHT;
        }
        else
        {
            line++;
        }
    }
}

void initTexture(std::vector<Texture2D>& texture)
{ 
    //// CARS
    Image image = LoadImage("images/car2.png");
    ImageResizeNN(&image, (field_size * 2),field_size - 6);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);  
    
    image  = LoadImage("images/car5.png");
    ImageResizeNN(&image, field_size * 2,field_size - 6);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image  = LoadImage("images/car6.png");
    ImageResizeNN(&image, field_size * 2,field_size - 6);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image  = LoadImage("images/limo.png");
    ImageResizeNN(&image, field_size * 3,field_size - 6);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image  = LoadImage("images/truck.png");
    ImageResizeNN(&image, field_size * 4,field_size - 6);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    // GESPIEGELT ///////////////////////////////////
    image = LoadImage("images/car2.png");
    ImageResizeNN(&image, (field_size * 2),field_size - 6);
    ImageRotate(&image, 180);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image  = LoadImage("images/car5.png");
    ImageResizeNN(&image, field_size * 2,field_size - 6);
    ImageRotate(&image, 180);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image  = LoadImage("images/car6.png");
    ImageResizeNN(&image, field_size * 2,field_size - 6);
    ImageRotate(&image, 180);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image  = LoadImage("images/limo.png");
    ImageResizeNN(&image, field_size * 3,field_size - 6);
    ImageRotate(&image, 180);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image  = LoadImage("images/truck.png");
    ImageResizeNN(&image, field_size * 4,field_size - 6);
    ImageRotate(&image, 180);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    // REST
    image  = LoadImage("images/grass.jpg");
    ImageResizeNN(&image, field_size,field_size);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image  = LoadImage("images/water.jpg");
    ImageResizeNN(&image, field_size,field_size);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image  = LoadImage("images/road.png");
    ImageResize(&image, field_size * (LEVEL_WIDTH / 2 + 2), (field_size * 3) + 4);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image = LoadImage("images/skull.png");
    ImageResizeNN(&image, field_size,field_size);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    // TREES
    image = LoadImage("images/long.png");
    ImageResizeNN(&image, field_size,field_size * 3);
    ImageRotate(&image, 90);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image = LoadImage("images/medium.png");
    ImageResizeNN(&image, field_size - 6,field_size * 2);
    ImageRotate(&image, 90);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);

    image = LoadImage("images/short.png");
    ImageResizeNN(&image, field_size - 6,field_size);
    ImageRotate(&image, 90);
    texture.push_back(LoadTextureFromImage(image));
    UnloadImage(image);


    image = LoadImage("images/background.jpg");
    //ImageResizeNN(&image, image.width * 0.5,image.height * 0.5);
    Image tmp = ImageFromImage(image, {image.width/2 - window_width / 2, image.height/2 - window_height / 2, window_width, window_height});
    texture.push_back(LoadTextureFromImage(tmp));

    image = LoadImage("images/rabbit_down.png");
    ImageResizeNN(&image, field_size, field_size);
    texture.push_back(LoadTextureFromImage(image));

    image = LoadImage("images/rabbit_up.png");
    ImageResizeNN(&image, field_size, field_size);
    texture.push_back(LoadTextureFromImage(image));

    image = LoadImage("images/rabbit_right.png");
    ImageResizeNN(&image, field_size, field_size);
    texture.push_back(LoadTextureFromImage(image));

    image = LoadImage("images/rabbit_left.png");
    ImageResizeNN(&image, field_size, field_size);
    texture.push_back(LoadTextureFromImage(image));
    
    UnloadImage(image);
    UnloadImage(tmp);
}

void initrabbit(rabbit_t& rabbit)
{
    rabbit.startPos = {start_level.x + (field_size * (LEVEL_WIDTH / 2)), start_level.y + (field_size * LEVEL_HEIGHT)};
    rabbit.startPosColision = {rabbit.startPos.x + (field_size * 0.25f), rabbit.startPos.y};
    rabbit.rec = {rabbit.startPos.x,rabbit.startPos.y, field_size, field_size};
    rabbit.recColision = {rabbit.startPosColision.x, rabbit.startPosColision.y, rabbit.rec.width * 0.55f, rabbit.rec.height};
    rabbit.life = rabbit_LIFE;
    rabbit.points = 0;
    rabbit.direction = RABBIT_DOWN;
}
