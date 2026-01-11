#include "king.hh"
#include <vector>

// Constructor: Initializes a King with its color, type, and name
King::King(ChessColor col) : ChessPiece(col, KING, "king") {}

set<Coord> King::get_allowed_moves(const ChessBoard &board) const
{
    set<Coord> moves;
    auto [row, col] = get_position();  // Get king's current position

    // King can move one square in any of the 8 directions
    const vector<pair<int, int>> directions = {
        {-1, -1}, {-1,  0}, {-1,  1},
        { 0, -1},          { 0,  1},
        { 1, -1}, { 1,  0}, { 1,  1},
    };

    // Try moving to each neighboring square
    for (const auto& [dr, dc] : directions)
    {
        Coord dest{row + dr, col + dc};
        check_move(board, dest, moves);  // check legality and possibly add move
    }

    return moves;
}

// King-specific check_move:
// - Validates if a square is empty or has an enemy piece
// - Adds move if legal (does not consider check or castling here)
bool King::check_move(const ChessBoard &board, const Coord &dest, set<Coord> &moves) const
{
    if (board.coord_in_bounds(dest))
    {
        shared_ptr<ChessPiece> piece = board.get_piece_at(dest);
        if (piece == nullptr || piece->get_color() != get_color())
        {
            moves.insert(dest);
        }
    }

    // King never continues in a direction, so we always return false
    return false;
}
