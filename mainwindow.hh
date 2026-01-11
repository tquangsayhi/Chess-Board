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
#ifndef MAIN_WINDOW_HH
#define MAIN_WINDOW_HH

#include "chess.hh"
#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QIcon>
#include <set>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // Function for handling the button clicks
    void onBoardSquareClicked();
    // Function for resetting the game
    void onRestartButtonClicked();

private:
    Ui::MainWindow* ui;

    // Constants for layout

    // Space between elements
    const int kMargin = 20;
    // Square's size
    const int kSquareSize = 60;
    // Thickness of border highlight
    const int kHighlightBorder = 4;
    // Info text browser size
    const int kInfoBoxWidth = 256;
    const int kInfoBoxHeight = 128;
    // Button size
    const int kSideButtonWidth = 256;
    const int kSideButtonHeight = 64;

    // Game logic
    Chess* chess_game_;

    // GUI components
    QPushButton* board_buttons_[8][8];
    QIcon piece_icons_[2][6][2]; // [piece_color][piece_type][square_color]
    QIcon black_square_icon_;
    QIcon white_square_icon_;
    QTextBrowser* info_display_;
    QPushButton* restart_button_;
    QPushButton* exit_button_;

    // State tracking
    Coord selected_coord_ = {-1, -1};
    std::set<Coord> valid_moves_;

    // Initialization methods
    void loadIcons();
    void setupBoard();
    void setupInfoDisplay();
    void setupRestartButton();
    void setupExitButton();

    // Helper methods
    void initializeGame();
    void updateSquare(int row, int col);
    void displayValidMoves(const std::set<Coord>& moves);
    void highlightSquare(const Coord& coord);
    void highlightSquares(const std::set<Coord>& coords);
    void clearHighlights();
    void updateInfoText(const QString& text);
};

#endif // MAIN_WINDOW_HH
