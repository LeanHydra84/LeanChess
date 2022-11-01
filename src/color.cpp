#include <filesystem>
#include <allegro5/allegro.h>
#include "color.h"

using fp = std::filesystem::path;

static const std::string pc_filename[] = 
{
    "wK", "bK", 
    "wQ", "bQ",
    "wR", "bR",
    "wB", "bB",
    "wN", "bN",
    "wP", "bP"
};

static ALLEGRO_BITMAP** get_ptr_to_palette_index(color_pal* pal, int index)
{
    // one of the worst pieces of code I will ever make
    switch(index)
    {
        case 0: return &pal->wKing;
        case 1: return &pal->bKing;

        case 2: return &pal->wQueen;
        case 3: return &pal->bQueen;

        case 4: return &pal->wRook;
        case 5: return &pal->bRook;

        case 6: return &pal->wBishop;
        case 7: return &pal->bBishop;

        case 8: return &pal->wKnight;
        case 9: return &pal->bKnight;

        case 10: return &pal->wPawn;
        case 11: return &pal->bPawn;

        default: return nullptr;
    }
}

void load_bitmaps(const char* directory, color_pal* pal, const std::string& f_extension)
{
    if (!al_is_image_addon_initialized())
        al_init_image_addon();

    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP);

    fp path(directory);
    
    int counter = 0;
    for(const auto & pn : pc_filename)
    {
        fp fn(pn + f_extension);
        fp full_path = path / fn;

        const std::string path_str = full_path.generic_string();
        
        ALLEGRO_BITMAP* bmap = al_load_bitmap(path_str.c_str());
        ALLEGRO_BITMAP** bmaploc = get_ptr_to_palette_index(pal, counter++);

        if(bmaploc == nullptr) continue;
        *bmaploc = bmap;
    }

}