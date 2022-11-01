#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include <vector>

#include "piece.h"
#include "color.h"

struct xypair
{
    int x, y;
    inline xypair(int x, int y) : x(x), y(y) { }

    inline xypair operator+(xypair r) { return xypair(x + r.x, y + r.y); }
    inline xypair operator*(int r) { return xypair(x * r, y * r); }

    inline bool operator==(xypair r) { return x == r.x && y == r.y; }
    inline bool operator!=(xypair r) { return x != r.x || y != r.y; }

    inline xypair operator-() { return xypair(-x, -y); }
};

struct board_details
{
    color_pal* palette;
    unsigned char width;
    unsigned char height;
    unsigned int pixelscale;
    std::string fen;
};

board_details default_board_details();

class active_board
{
public:
    active_board(const board_details& bd);

    piece* get_piece_at_position(int x, int y);
    void delete_piece(piece* p);

    bool is_valid_boardpos(int x, int y);
    void get_list_of_moves(piece* p, std::vector<xypair>& moves);
    bool is_move_legal(piece* p, xypair move);
private:
    void add_if_valid(xypair pos, std::vector<xypair>& input, bool white);
    void iterate_offset(xypair origin, xypair offset, std::vector<xypair>& input, bool white);
    void iterate_kinglike(xypair origin, std::vector<xypair>& input, bool white);
    void iterate_knightlike(xypair origin, std::vector<xypair>& input, bool white);
public:
    bool is_white_turn;
    board_details details;
    std::vector<piece> pieces;
};

#endif