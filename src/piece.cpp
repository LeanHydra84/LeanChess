#include "piece.h"
#include <string>

#include <allegro5/allegro_primitives.h>

piece_details::piece_details(bool n, bool b, bool r, bool k, bool p)
{
    knight = n;
    diagonal = b;
    line = r;
    king = k;
    pawn = p;
}

piece_details::piece_details()
{
    knight = false;
    diagonal = false;
    line = false;
    king = false;
    pawn = false;
}

piece_details get_move_type_from_key(char key)
{
    switch(std::tolower(key))
    {
        case 'r':
            return piece_literals::Rook;
        case 'k':
            return piece_literals::King;
        case 'n':
            return piece_literals::Knight;
        case 'b':
            return piece_literals::Bishop;
        case 'p':
            return piece_literals::Pawn;
        case 'q':
            return piece_literals::Queen;
        default:
            return piece_details(false, false, false, false, false);
    }
}

static ALLEGRO_BITMAP* get_image_from_palette(const color_pal& pal, char key)
{
    switch(key)
    {
        case 'r': return pal.bRook;
        case 'k': return pal.bKing;
        case 'n': return pal.bKnight;
        case 'b': return pal.bBishop;
        case 'p': return pal.bPawn;
        case 'q': return pal.bQueen;
        
        case 'R': return pal.wRook;
        case 'K': return pal.wKing;
        case 'N': return pal.wKnight;
        case 'B': return pal.wBishop;
        case 'P': return pal.wPawn;
        case 'Q': return pal.wQueen;

        default:
            return nullptr;
    }
}

piece create_piece(int x, int y, char key, const color_pal& palette)
{
    piece p;
    p.x = x;
    p.y = y;
    p.white = std::isupper(key);
    p.move = get_move_type_from_key(key);
    p.image = get_image_from_palette(palette, key);
    p.draw = true;
    p.has_moved = false;
    return p;
}

void draw_piece(const piece& p, int px)
{
    int wi = al_get_bitmap_width(p.image);
    int hei = al_get_bitmap_height(p.image);

    al_draw_scaled_bitmap(
        p.image,
        0, 0,
        wi, hei,
        p.x * px,
        p.y * px,
        px, px, 0
    );
}

void draw_hold_piece(const hold_piece& p, int pixescale)
{
    int wi = al_get_bitmap_width(p.hold_piece->image);
    int hei = al_get_bitmap_height(p.hold_piece->image);

    int x = p.mx - p.ox;
    int y = p.my - p.oy;

    al_draw_scaled_bitmap(
        p.hold_piece->image,
        0, 0,
        wi, hei,
        x, y,
        pixescale,
        pixescale,
        0
    );
}