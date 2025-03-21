#pragma once

#include <raylib.h>
#include <vector>

#include "defines.h"

void initWindow(int fieldsize);
void initLevel(std::vector<Texture2D>& texture);
void initTexture(std::vector<Texture2D>& texture);
void initrabbit(rabbit_t& rabbit);