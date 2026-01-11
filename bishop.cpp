#include "bishop.hh"

// Constructor: Initializes a Bishop with its color and predefined type/name.
Bishop::Bishop(ChessColor col) : ChessPiece(col, BISHOP, "bishop") {}

set<Coord> Bishop::get_allowed_moves(const ChessBoard &board) const
{
    set<Coord> moves;
    auto [row, col] = get_position();  // Get current position of the bishop

    Coord base;
    base.row = row;
    base.col = col;

    // Try all four diagonal directions (bishop moves only diagonally)

    // Top-left diagonal
    row = base.row - 1;
    col = base.col - 1;
    while (check_move(board, {row, col}, moves))  // keep checking while path is free
    {
        row--;
        col--;
    }

    // Top-right diagonal
    row = base.row - 1;
    col = base.col + 1;
    while (check_move(board, {row, col}, moves))
    {
        row--;
        col++;
    }

    // Bottom-left diagonal
    row = base.row + 1;
    col = base.col - 1;
    while (check_move(board, {row, col}, moves))
    {
        row++;
        col--;
    }

    // Bottom-right diagonal
    row = base.row + 1;
    col = base.col + 1;
    while (check_move(board, {row, col}, moves))
    {
        row++;
        col++;
    }

    return moves;
}

// Helper function to check if a bishop can move to 'dest'
// - Adds valid move to 'moves' set
// - Returns true if bishop can continue moving in the same direction (i.e., square is empty)
// - Returns false if blocked by same color or captures opposite color (stopping point)
bool Bishop::check_move(const ChessBoard &board, const Coord &dest, set<Coord> &moves) const
{
    if (board.coord_in_bounds(dest))  // Skip if out of board bounds
    {
        shared_ptr<ChessPiece> piece = board.get_piece_at(dest);
        if (piece == nullptr)  // Empty square: valid move, continue in this direction
        {
            moves.insert(dest);
            return true;
        }
        else if (piece->get_color() != get_color())  // Opponent's piece: can capture, but must stop
        {
            moves.insert(dest);
            return false;
        }
        else  // Same-color piece: blocked
        {
            return false;
        }
    }
    return false;  // Destination is outside board bounds
}
