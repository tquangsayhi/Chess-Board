#include "queen.hh"
#include <vector>

// Constructor: Initializes a Queen with its color, type, and name
Queen::Queen(ChessColor col) : ChessPiece(col, QUEEN, "queen") {}

set<Coord> Queen::get_allowed_moves(const ChessBoard &board) const
{
    set<Coord> moves;
    auto [row, col] = get_position();  // Get queen's current position
    Coord base{row, col};

    // Queen can move in 8 directions: straight + diagonals
    const vector<pair<int, int>> directions = {
        {-1,  0}, // up
        { 1,  0}, // down
        { 0, -1}, // left
        { 0,  1}, // right
        {-1, -1}, // up-left
        {-1,  1}, // up-right
        { 1, -1}, // down-left
        { 1,  1}, // down-right
    };

    // For each direction, continue moving until blocked
    for (const auto& [dr, dc] : directions)
    {
        int r = base.row + dr;
        int c = base.col + dc;
        while (check_move(board, {r, c}, moves))
        {
            r += dr;
            c += dc;
        }
    }

    return moves;
}

// Queen-specific check_move:
// - Adds legal squares to moves
// - Returns whether movement should continue in that direction
bool Queen::check_move(const ChessBoard &board, const Coord &dest, set<Coord> &moves) const
{
    if (board.coord_in_bounds(dest))
    {
        shared_ptr<ChessPiece> piece = board.get_piece_at(dest);
        if (piece == nullptr)
        {
            moves.insert(dest);
            return true; // can keep going
        }
        else if (piece->get_color() != get_color())
        {
            moves.insert(dest);
            return false; // can capture but must stop
        }
        else
        {
            return false; // blocked by same color
        }
    }
    return false; // out of bounds
}
