#include <allegro5/allegro.h>

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "board.h"
#include "piece.h"

bool init_all_allegro()
{
    if (!al_init()) return false;
    if (!al_init_primitives_addon()) return false;
    if (!al_init_image_addon()) return false;
    return true;
}

static xypair get_click_square(int cx, int cy, int pixelscale)
{
    cx /= pixelscale;
    cy /= pixelscale;
    return xypair(cx, cy);
}

void clear_to_background(const board_details& board)
{
    for(int x = 0; x < board.width; x++)
    {
        for(int y = 0; y < board.height; y++)
        {
            bool lightsquare = (y % 2 == 0) != (x % 2 == 0);
            al_draw_filled_rectangle(
                x * board.pixelscale, 
                y * board.pixelscale,
                (x + 1) * board.pixelscale,
                (y + 1) * board.pixelscale,
                !lightsquare ? board.palette->lightsquare : board.palette->darksquare
            );
        }
    }
}

void draw_piece_set(const std::vector<piece>& pieces, int pixelscale)
{
    for(auto p : pieces)
    {
        if(!p.draw) continue;
        draw_piece(p, pixelscale);
    }
}

int game_loop(board_details board)
{

    al_install_mouse();

    ALLEGRO_DISPLAY* disp = al_create_display(board.width * board.pixelscale, board.height * board.pixelscale);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_mouse_event_source());

    // loop variables
    bool redraw = true;
    ALLEGRO_EVENT e;

    active_board active(board);
    hold_piece hold;
    hold.hold_piece = nullptr;

    while(true)
    {
        al_wait_for_event(queue, &e);

        if(e.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        else if(e.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            xypair pos = get_click_square(e.mouse.x, e.mouse.y, board.pixelscale);
            piece* clickpiece = active.get_piece_at_position(pos.x, pos.y);
            
            if(hold.hold_piece == nullptr && clickpiece != nullptr && active.is_white_turn == clickpiece->white)
            {
                clickpiece->draw = false;

                hold.hold_piece = clickpiece;
                
                hold.x = clickpiece->x * board.pixelscale;
                hold.y = clickpiece->y * board.pixelscale;
                
                hold.ox = e.mouse.x - hold.x;
                hold.oy = e.mouse.y - hold.y;

                hold.mx = e.mouse.x;
                hold.my = e.mouse.y;

                redraw = true;
            }

        }
        else if(e.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if(hold.hold_piece != nullptr)
            {

                xypair pos = get_click_square(e.mouse.x, e.mouse.y, board.pixelscale);
                hold.hold_piece->draw = true;

                if(active.is_move_legal(hold.hold_piece, pos))
                {
                    active.is_white_turn = !active.is_white_turn;
                    piece* takepiece = active.get_piece_at_position(pos.x, pos.y);

                    hold.hold_piece->x = pos.x;
                    hold.hold_piece->y = pos.y;
                    hold.hold_piece->has_moved = true;

                    if(takepiece != nullptr)
                        active.delete_piece(takepiece);
                }

                hold.hold_piece = nullptr;

                redraw = true;
            }
        }
        else if(e.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            if(hold.hold_piece != nullptr)
            {
                hold.mx = e.mouse.x;
                hold.my = e.mouse.y;

                redraw = true;
            }
        }

        if(redraw && al_is_event_queue_empty(queue))
        {
            clear_to_background(board);
            draw_piece_set(active.pieces, board.pixelscale);
            
            if(hold.hold_piece != nullptr)
            {
                draw_hold_piece(hold, board.pixelscale);
            }

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_event_queue(queue);

    return 0;

}