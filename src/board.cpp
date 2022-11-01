#include "board.h"

board_details default_board_details()
{
    board_details details;
    details.width = 8;
    details.height = 8;
    details.fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    return details;
}

piece* active_board::get_piece_at_position(int x, int y)
{
    for(piece& p : pieces)
    {
        if(p.x == x && p.y == y) return &p;
    }
    return nullptr;
}

bool active_board::is_valid_boardpos(int x, int y)
{
    if(x < 0 || x >= details.width) return false;
    if(y < 0 || y >= details.height) return false;
    return true;
}

void active_board::iterate_offset(xypair origin, xypair offset, std::vector<xypair>& input, bool white)
{

    origin = origin + offset;
    while(get_piece_at_position(origin.x, origin.y) == nullptr && is_valid_boardpos(origin.x, origin.y))
    {
        input.push_back(origin);
        origin = origin + offset;
    }
    
    piece* atpos = get_piece_at_position(origin.x, origin.y);
    if(atpos == nullptr) return;

    if(atpos->white != white)
        input.push_back(origin);
}

void active_board::iterate_kinglike(xypair origin, std::vector<xypair>& input, bool white)
{
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(j == 0 && i == 0) continue;
            if(!is_valid_boardpos(origin.x + i, origin.y + j)) continue;

            piece* atpos = get_piece_at_position(origin.x + i, origin.y + j);
            if(atpos == nullptr || atpos->white != white)
                input.push_back(xypair(origin.x + i, origin.y + j));
        }
    }
}

void active_board::add_if_valid(xypair pos, std::vector<xypair>& input, bool white)
{
    if(!is_valid_boardpos(pos.x, pos.y)) return;
    piece* p = get_piece_at_position(pos.x, pos.y);
    if(p == nullptr || p->white != white)
        input.push_back(pos);
}


void active_board::iterate_knightlike(xypair origin, std::vector<xypair>& input, bool white)
{
    const int l = 2;
    const int s = 1;

    add_if_valid(origin + xypair(l, s), input, white);
    add_if_valid(origin + xypair(-l, s), input, white);
    add_if_valid(origin + xypair(l, -s), input, white);
    add_if_valid(origin + xypair(-l, -s), input, white);

    add_if_valid(origin + xypair(s, l), input, white);
    add_if_valid(origin + xypair(-s, l), input, white);
    add_if_valid(origin + xypair(s, -l), input, white);
    add_if_valid(origin + xypair(-s, -l), input, white);
}

void active_board::get_list_of_moves(piece* p, std::vector<xypair>& moves)
{
    piece_details m = p->move;
    xypair origin = xypair(p->x, p->y);

    if(m.diagonal)
    {
        iterate_offset(origin, xypair(1, 1), moves, p->white);
        iterate_offset(origin, xypair(-1, 1), moves, p->white);
        iterate_offset(origin, xypair(1, -1), moves, p->white);
        iterate_offset(origin, xypair(-1, -1), moves, p->white);
    }

    if(m.line)
    {
        iterate_offset(origin, xypair(1, 0), moves, p->white);
        iterate_offset(origin, xypair(-1, 0), moves, p->white);
        iterate_offset(origin, xypair(0, 1), moves, p->white);
        iterate_offset(origin, xypair(0, -1), moves, p->white);
    }

    if(m.pawn)
    {
        // Push moves
        int forward = origin.y + (p->white ? -1 : 1);
        if(is_valid_boardpos(origin.x, forward) && get_piece_at_position(origin.x, forward) == nullptr)
        {
            moves.push_back(xypair(origin.x, forward));

            int doubleforward = origin.y + (p->white ? -2 : 2);
            if(!p->has_moved && get_piece_at_position(origin.x, doubleforward) == nullptr)
                moves.push_back(xypair(origin.x, doubleforward));
        }

        // Take moves
        if(is_valid_boardpos(origin.x - 1, forward))
        {
            piece* possible_take = get_piece_at_position(origin.x - 1, forward);
            if(possible_take != nullptr && possible_take->white != p->white)
                moves.push_back(xypair(origin.x - 1, forward));
        }

        if(is_valid_boardpos(origin.x + 1, forward))
        {
            piece* possible_take = get_piece_at_position(origin.x + 1, forward);
            if(possible_take != nullptr && possible_take->white != p->white)
                moves.push_back(xypair(origin.x + 1, forward));
        }
    }

    if(m.king)
    {
        iterate_kinglike(origin, moves, p->white);
    }

    if(m.knight)
    {
        iterate_knightlike(origin, moves, p->white);
    }

}

bool active_board::is_move_legal(piece* p, xypair move)
{
    std::vector<xypair> moves;
    get_list_of_moves(p, moves);

    return std::find(moves.begin(), moves.end(), move) != moves.end();
}

void active_board::delete_piece(piece* p)
{
    auto iter = std::find_if(pieces.begin(), pieces.end(), [=](piece& r) { return &r == p; });
    pieces.erase(iter);
}

static std::vector<std::string> splitstr(const std::string& str, const std::string& delim)
{
    std::vector<std::string> vec;

    size_t pos = 0;
    size_t pos_start = 0;

    while( (pos = str.find(delim, pos_start)) != std::string::npos )
    {
        vec.push_back(str.substr(pos_start, pos - pos_start));
        pos_start = pos + delim.length();
    }

    vec.push_back(str.substr(pos_start));
    return vec;
}

void load_pieces_from_fen(std::vector<piece>& piecesvec, const std::string& piece_part, const board_details& details)
{
    int rank = 0, file = 0;
    for(int i = 0; i < piece_part.length(); i++)
    {
        char c = piece_part[i];
        if(c == '/')
        {
            rank++;
            file = 0;
        }
        else if(std::isspace(c)) continue;
        else if(std::isdigit(c))
        {
            int n = c - '0';
            file += n;
        }
        else if(listpiecesymbols.find(std::tolower(c)) != std::string::npos)
        {
            piecesvec.push_back( create_piece(file, rank, c, *details.palette) );
            file++;
        }
        else continue;
    }
}

#include <iostream>

active_board::active_board(const board_details& bd) : details(bd), pieces()
{
    std::vector<std::string> split = splitstr(details.fen, " ");
    load_pieces_from_fen(pieces, split[0], details);

    is_white_turn = split[1] == "w";

    // load other weird stuff
}