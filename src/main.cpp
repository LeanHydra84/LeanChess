#include <iostream>

#include "color.h"
#include "board.h"
#include "game.h"

int main()
{

    init_all_allegro();

    color_pal palette;
    palette.darksquare = al_map_rgb(92, 128, 128);
    palette.lightsquare = al_map_rgb(175, 219, 219);
    load_bitmaps("C:\\Users\\Augustus\\Downloads\\bmaps", &palette, ".bmp");

    board_details details = default_board_details();
    details.pixelscale = 100;
    details.palette = &palette;

    game_loop(details);

}