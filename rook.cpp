#include "rook.hh"

// Constructor: Initializes a Rook with its color and predefined type/name.
Rook::Rook(ChessColor col) : ChessPiece(col, ROOK, "rook") {}

set<Coord> Rook::get_allowed_moves(const ChessBoard &board) const
{
    set<Coord> moves;
    auto [row, col] = get_position();  // Get current position of the rook

    Coord base;
    base.row = row;
    base.col = col;

    // Rook moves in straight lines: up, down, left, right

    // Move up (decreasing row index)
    row = base.row - 1;
    col = base.col;
    while (check_move(board, {row, col}, moves))  // continue while square is free
    {
        row--;
    }

    // Move down (increasing row index)
    row = base.row + 1;
    col = base.col;
    while (check_move(board, {row, col}, moves))
    {
        row++;
    }

    // Move left (decreasing column index)
    row = base.row;
    col = base.col - 1;
    while (check_move(board, {row, col}, moves))
    {
        col--;
    }

    // Move right (increasing column index)
    row = base.row;
    col = base.col + 1;
    while (check_move(board, {row, col}, moves))
    {
        col++;
    }

    return moves;
}

// Helper method to check if the rook can move to 'dest'
// - Adds valid move to 'moves' set
// - Returns true to keep scanning in same direction (empty square)
// - Returns false to stop (either captured or blocked)
bool Rook::check_move(const ChessBoard &board, const Coord &dest, set<Coord> &moves) const
{
    if (board.coord_in_bounds(dest))  // Skip if destination is outside the board
    {
        shared_ptr<ChessPiece> piece = board.get_piece_at(dest);
        if (piece == nullptr)  // Empty square: valid move, continue
        {
            moves.insert(dest);
            return true;
        }
        else if (piece->get_color() != get_color())  // Enemy piece: capture and stop
        {
            moves.insert(dest);
            return false;
        }
        else  // Same-color piece: blocked
        {
            return false;
        }
    }
    return false;  // Out of bounds
}
