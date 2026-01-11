#ifndef QUEEN_HH
#define QUEEN_HH

#include "rook.hh"

/**
 * @brief The Queen class
 * The queen moves any number of squares vertically, horizontally, or diagonally
 * through unoccupied squares. The queen captures by moving to the square occupied
 * by an enemy piece.
 */
class Queen : public ChessPiece
{
public:
    Queen(ChessColor col);
    virtual ~Queen() = default;

    virtual set<Coord> get_allowed_moves(ChessBoard const& board) const override;

private:
    bool check_move(const ChessBoard &board, const Coord &dest, set<Coord> &moves) const;

};

#endif // QUEEN_HH
