#ifndef _PIECE_H_
#define _PIECE_H_

#include <allegro5/allegro_image.h>
#include <string>
#include "color.h"

const inline std::string listpiecesymbols = "rbnkqp";

struct piece_details
{
    piece_details(bool n, bool b, bool r, bool k, bool p);
    piece_details();

    bool knight;
    bool diagonal;
    bool line;
    bool king;
    bool pawn;
};

namespace piece_literals
{
    inline const piece_details Queen = piece_details(false, true, true, false, false);
    inline const piece_details King = piece_details(false, false, false, true, false);
    inline const piece_details Rook = piece_details(false, false, true, false, false);
    inline const piece_details Bishop = piece_details(false, true, false, false, false);
    inline const piece_details Knight = piece_details(true, false, false, false, false);
    inline const piece_details Pawn = piece_details(false, false, false, false, true);
};

struct piece
{
    bool has_moved;
    bool draw;
    bool white;
    int x, y;
    piece_details move;
    ALLEGRO_BITMAP* image;
};

struct hold_piece
{
    piece* hold_piece;
    int x, y;
    int ox, oy;

    int mx, my;
};

piece create_piece(int x, int y, char key, const color_pal& palette);
void draw_hold_piece(const hold_piece& p, int squaresize);
void draw_piece(const piece& p, int squaresize);

#endif