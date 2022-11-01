#ifndef _COLOR_H_
#define _COLOR_H_

#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <string>

struct color_pal
{
    ALLEGRO_COLOR lightsquare;
    ALLEGRO_COLOR darksquare;

    ALLEGRO_BITMAP* wKing;
    ALLEGRO_BITMAP* wPawn;
    ALLEGRO_BITMAP* wQueen;
    ALLEGRO_BITMAP* wKnight;
    ALLEGRO_BITMAP* wBishop;
    ALLEGRO_BITMAP* wRook;

    ALLEGRO_BITMAP* bKing;
    ALLEGRO_BITMAP* bPawn;
    ALLEGRO_BITMAP* bQueen;
    ALLEGRO_BITMAP* bKnight;
    ALLEGRO_BITMAP* bBishop;
    ALLEGRO_BITMAP* bRook;
};

void load_bitmaps(const char* directory, color_pal* pal, const std::string& f_extension = ".png");

#endif