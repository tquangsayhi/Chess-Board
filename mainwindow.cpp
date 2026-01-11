/*
Author 1: Minh Sai
Student number: 153126022
Email: minh.sai@tuni.fi
Username: nqb766

Autho 2: Quang Le
Student number: 153126132
Email: quang.le@tuni.fi
Username: tcb483

Notes about this project:
Hi TA,
We have tried our best to fulfill all requirements and to complete the project.
Thanks for grading us. Have a nice day !!!
 */
#include "mainwindow.hh"
#include "ui_mainwindow.h"

// Constructor: Initializes UI and starts a new game
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);                       // Set up the user interface
    chess_game_ = new Chess();               // Initialize the chess game instance
    chess_game_->start_game();               // Start a new game

    loadIcons();                             // Load piece and square icons
    setupBoard();                            // Set up the 8x8 board
    setupInfoDisplay();                      // Set up the info text area
    setupRestartButton();                    // Set up the restart button
    setupExitButton();                       // Set up the exit button
    initializeGame();                        // Populate the board and info text
}

// Destructor: Clean up dynamically allocated resources
MainWindow::~MainWindow()
{
    delete ui;
    delete chess_game_;
}

// Slot triggered when a board square is clicked
void MainWindow::onBoardSquareClicked()
{
    // Ignore clicks if the game is over
    if (chess_game_->get_game_state() != IN_PROGRESS)
        return;

    // Determine which square was clicked
    Coord clicked{-1, -1};
    for (int r = 0; r < 8 && clicked.row == -1; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            if (board_buttons_[r][c] == sender())
            {
                clicked = {r, c};
                break;
            }
        }
    }

    const ChessBoard& board = chess_game_->get_board();
    auto piece = board.get_piece_at(clicked);
    ChessColor turn = chess_game_->get_current_turn();

    // If the clicked piece belongs to the current player
    if (piece && piece->get_color() == turn)
    {
        // Deselect if already selected, otherwise show valid moves
        if (clicked == selected_coord_)
        {
            displayValidMoves({});
            selected_coord_ = {-1, -1};
        }
        else
        {
            displayValidMoves(piece->get_allowed_moves(board));
            selected_coord_ = clicked;
        }
        return;
    }

    // If the clicked square is a valid move, perform the move
    if (valid_moves_.count(clicked))
    {
        auto selected_piece = board.get_piece_at(selected_coord_);
        if (chess_game_->make_move(selected_piece, clicked))
        {
            updateSquare(selected_coord_.row, selected_coord_.col);
            updateSquare(clicked.row, clicked.col);
            displayValidMoves({});
            selected_coord_ = {-1, -1};

            GameState state = chess_game_->get_game_state();
            updateInfoText(state == IN_PROGRESS
                               ? (turn == BLACK ? "White's turn!" : "Black's turn!")
                               : (state == BLACK_WIN ? "Black won!" : "White won!"));
        }
    }
}

// Slot for when the restart button is clicked
void MainWindow::onRestartButtonClicked()
{
    chess_game_->start_game();      // Reset the game logic
    initializeGame();               // Refresh the board and info display
}

// Load icons for all piece types, colors, and square backgrounds
void MainWindow::loadIcons()
{
    struct IconInfo {
        PieceType type;
        const char* name;
    } icon_info[] = {
        {PAWN, "pawn"}, {ROOK, "rook"}, {KING, "king"},
        {BISHOP, "bishop"}, {QUEEN, "queen"}, {KNIGHT, "knight"}
    };

    black_square_icon_ = QIcon(":/pieces/empty-bl");
    white_square_icon_ = QIcon(":/pieces/empty-wt");

    for (const auto& info : icon_info)
    {
        for (int color = 0; color <= 1; ++color)
        {
            for (int sq_color = 0; sq_color <= 1; ++sq_color)
            {
                QString path = QString(":/pieces/%1-%2-on-%3.png")
                    .arg(color == BLACK ? "bl" : "wt")
                    .arg(info.name)
                    .arg(sq_color == BLACK ? "bl" : "wt");
                piece_icons_[color][info.type][sq_color] = QIcon(path);
            }
        }
    }
}

// Set up the grid of board buttons
void MainWindow::setupBoard()
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            auto* button = new QPushButton(this);
            button->setGeometry(kMargin + kSquareSize * col, kMargin + kSquareSize * row, kSquareSize, kSquareSize);
            button->setIconSize(QSize(kSquareSize, kSquareSize));
            connect(button, &QPushButton::clicked, this, &MainWindow::onBoardSquareClicked);
            board_buttons_[row][col] = button;
        }
    }
}

// Set up the text browser for displaying game messages
void MainWindow::setupInfoDisplay()
{
    info_display_ = new QTextBrowser(this);
    info_display_->setGeometry(kMargin + 8 * kSquareSize + 2 * kMargin,
                               kMargin + 1.5 * kSquareSize,
                               kInfoBoxWidth,
                               kInfoBoxHeight);
    updateInfoText("");
}

// Set up the restart button and connect it to its slot
void MainWindow::setupRestartButton()
{
    restart_button_ = new QPushButton("Restart", this);
    restart_button_->setGeometry(kMargin + 8 * kSquareSize + 2 * kMargin,
                                 kMargin + kInfoBoxHeight + kMargin + 1.5 * kSquareSize,
                                 kSideButtonWidth,
                                 kSideButtonHeight);
    connect(restart_button_, &QPushButton::clicked, this, &MainWindow::onRestartButtonClicked);
}

// Set up the exit button and connect it to close the window
void MainWindow::setupExitButton()
{
    exit_button_ = new QPushButton("Quit", this);
    exit_button_->setGeometry(kMargin + 8 * kSquareSize + 2 * kMargin,
                              kMargin + kInfoBoxHeight + 2 * kMargin + kSideButtonHeight + 1.5 * kSquareSize,
                              kSideButtonWidth,
                              kSideButtonHeight);
    connect(exit_button_, &QPushButton::clicked, this, &MainWindow::close);
}

// Initializes the board and resets the info text
void MainWindow::initializeGame()
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            updateSquare(row, col);
            board_buttons_[row][col]->setEnabled(true);
        }
    }
    updateInfoText("New game started. White's turn.");
}

// Updates a specific square's icon based on its piece or empty state
void MainWindow::updateSquare(int row, int col)
{
    auto piece = chess_game_->get_board().get_piece_at(row, col);
    if (!piece)
    {
        board_buttons_[row][col]->setIcon(((row + col) % 2 == 0) ? black_square_icon_ : white_square_icon_);
    }
    else
    {
        ChessColor piece_color = piece->get_color();
        PieceType piece_type = piece->get_type();
        ChessColor square_color = ((row + col) % 2 == 0) ? BLACK : WHITE;
        board_buttons_[row][col]->setIcon(piece_icons_[piece_color][piece_type][square_color]);
    }
}

// Highlights all valid move squares and stores the set
void MainWindow::displayValidMoves(const std::set<Coord>& moves)
{
    clearHighlights();
    valid_moves_ = moves;
    highlightSquares(moves);
}

// Highlights one square by changing its border and icon size
void MainWindow::highlightSquare(const Coord& coord)
{
    board_buttons_[coord.row][coord.col]->setStyleSheet("border: 3px solid red;");
    board_buttons_[coord.row][coord.col]->setIconSize(QSize(kSquareSize - kHighlightBorder, kSquareSize - kHighlightBorder));
}

// Highlights a set of squares
void MainWindow::highlightSquares(const std::set<Coord>& coords)
{
    for (const auto& coord : coords)
        highlightSquare(coord);
}

// Removes highlighting from all previously highlighted squares
void MainWindow::clearHighlights()
{
    for (const auto& coord : valid_moves_)
    {
        board_buttons_[coord.row][coord.col]->setStyleSheet("");
        board_buttons_[coord.row][coord.col]->setIconSize(QSize(kSquareSize, kSquareSize));
    }
}

// Updates the info display with the given text
void MainWindow::updateInfoText(const QString& text)
{
    info_display_->setText(text);
}
