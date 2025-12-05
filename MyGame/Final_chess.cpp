#include <SFML/Graphics.hpp>
#include<iostream>
#include<string>
using namespace std;

bool canMove(int board[8][8], int startingRow, int startingCol, int destinationRow, int destinationCol);
bool KingInCheck(int board[8][8], int kingColor);
bool hasAnyLegalMove(int board[8][8], int turn);

void drawBoard(sf::RenderWindow& window, float squareX, float squareY, float boardOffsetX, float  boardOffsetY)
{
    for (int row = 0; row < 8; row++)
    {
        for (int column = 0; column < 8; column++)
        {
            sf::RectangleShape square(sf::Vector2f(squareX, squareY));
            square.setPosition(boardOffsetX + column * squareX, boardOffsetY + row * squareY);

            if ((row + column) % 2 == 0)
                square.setFillColor(sf::Color::Color(210, 200, 170)); //beige
            else
                square.setFillColor(sf::Color::Color(139, 69, 19));  //brown

            window.draw(square);
        }
    }
}
void loadTextures(sf::Texture& whitepawn, sf::Texture& blackpawn, sf::Texture& whiterook,
    sf::Texture& blackrook, sf::Texture& whitehorse, sf::Texture& blackhorse, sf::Texture& whitebishop,
    sf::Texture& blackbishop, sf::Texture& whitequeen, sf::Texture& blackqueen, sf::Texture& whiteking,
    sf::Texture& blackking)
{
    whitepawn.loadFromFile("Chess pieces/white_pawn.png");
    blackpawn.loadFromFile("Chess pieces/black_pawn.png");
    whiterook.loadFromFile("Chess pieces/white_rook.png");
    blackrook.loadFromFile("Chess pieces/black_rook.png");
    whitehorse.loadFromFile("Chess pieces/white_horse.png");
    blackhorse.loadFromFile("Chess pieces/black_horse.png");
    whitebishop.loadFromFile("Chess pieces/white_bishop.png");
    blackbishop.loadFromFile("Chess pieces/black_bishop.png");
    whitequeen.loadFromFile("Chess pieces/white_queen.png");
    blackqueen.loadFromFile("Chess pieces/black_queen.png");
    whiteking.loadFromFile("Chess pieces/white_king.png");
    blackking.loadFromFile("Chess pieces/black_king.png");
}
void drawPieces(sf::RenderWindow& window, int board[8][8], float squareX, float squareY, float scaleX,
    float scaleY, sf::Texture& whitepawn, sf::Texture& blackpawn, sf::Texture& whiterook,
    sf::Texture& blackrook, sf::Texture& whitehorse, sf::Texture& blackhorse, sf::Texture& whitebishop,
    sf::Texture& blackbishop, sf::Texture& whitequeen, sf::Texture& blackqueen, sf::Texture& whiteking,
    sf::Texture& blackking, float boardOffsetX, float  boardOffsetY)
{
    sf::Sprite piece;
    for (int row = 0; row < 8; row++)
    {
        for (int column = 0; column < 8; column++)
        {
            int p = board[row][column];
            if (p == 0)
            {
                continue;
            }
            switch (p)
            {
            case 1: piece.setTexture(whitepawn);
                break;
            case -1: piece.setTexture(blackpawn);
                break;
            case 2: piece.setTexture(whiterook);
                break;
            case -2: piece.setTexture(blackrook);
                break;
            case 3: piece.setTexture(whitehorse);
                break;
            case -3: piece.setTexture(blackhorse);
                break;
            case 4: piece.setTexture(whitebishop);
                break;
            case -4: piece.setTexture(blackbishop);
                break;
            case 5: piece.setTexture(whitequeen);
                break;
            case -5: piece.setTexture(blackqueen);
                break;
            case 6: piece.setTexture(whiteking);
                break;
            case -6: piece.setTexture(blackking);
                break;
            }
            piece.setScale(scaleX, scaleY);
            piece.setPosition(boardOffsetX + column * squareX, boardOffsetY + row * squareY);
            window.draw(piece);
        }
    }
}
void initializeBoard(int board[8][8])
{
    for (int row = 0; row < 8; row++)
    {
        for (int column = 0; column < 8; column++)
        {
            board[row][column] = 0;
        }
    }
    //black pieces
    board[0][0] = -2; board[0][7] = -2;    //rook
    board[0][1] = -3; board[0][6] = -3;    //knight
    board[0][2] = -4; board[0][5] = -4;    //bishop
    board[0][3] = -5;                      //queen
    board[0][4] = -6;                      //king
    for (int i = 0; i < 8; i++)            //pawns
    {
        board[1][i] = -1;
    }
    //white pieces
    board[7][0] = 2; board[7][7] = 2;      //rook
    board[7][1] = 3; board[7][6] = 3;      //knight
    board[7][2] = 4; board[7][5] = 4;      //bishop
    board[7][3] = 5;                       //queen
    board[7][4] = 6;                       //king
    for (int i = 0; i < 8; i++)            //pawns
    {
        board[6][i] = 1;
    }
}
bool onBoard(int row, int column)
{
    if (row >= 0 && row < 8 && column >= 0 && column < 8)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool ispathClear(int board[8][8], int startingRow, int startingCol, int destinationRow, int destinationCol)
{
    int rowStep = 0;
    int colStep = 0;

    if (destinationRow > startingRow)         //moving forward(down)
    {
        rowStep = 1;
    }
    else if (destinationRow < startingRow)    //moving backward(up)
    {
        rowStep = -1;
    }
    if (destinationCol > startingCol)         //moving right
    {
        colStep = 1;
    }
    else if (destinationCol < startingCol)    //moving left
    {
        colStep = -1;
    }
    int row = startingRow + rowStep;         //start with 1 step next toward the destination
    int col = startingCol + colStep;
    while (row != destinationRow || col != destinationCol)     //check till the checked boxes does not reach the destination
    {
        if (board[row][col] != 0)         //if not empty, path not clear
        {
            return false;
        }
        row = row + rowStep;            //check one step further from the checked box
        col = col + colStep;
    }
    return true;              //all boxes till destination empty
}
bool canMove(int board[8][8], int startingRow, int startingCol, int destinationRow, int destinationCol)
{
    if (onBoard(startingRow, startingCol) == false || onBoard(destinationRow, destinationCol) == false)
    {
        return false;            //if clicked anywhere outside the chess board, cannot move
    }

    int piece = board[startingRow][startingCol];     //selected piece which is to be moved

    if (piece == 0)          //empty box selected, cannot move nothing
    {
        return false;
    }
    if (startingRow == destinationRow && startingCol == destinationCol)       //moving to same box
    {
        return false;
    }
    int color;
    if (piece > 0)        //white
    {
        color = 1;
    }
    else if (piece < 0)      //black
    {
        color = -1;
    }
    int target = board[destinationRow][destinationCol]; //clicked box, where selected piece is to be moved

    if (target * piece > 0)     //the starting and destination places belong to same color
    {                           //+ into + also +, - into - also +
        return false;
    }
    int typeofpiece;
    if (piece > 0)
    {
        typeofpiece = piece;
    }
    else if (piece < 0)
    {
        typeofpiece = -piece;
    }

    int rows_moved = destinationRow - startingRow;
    int cols_moved = destinationCol - startingCol;

    //checking valid invalid pawn moves
    if (typeofpiece == 1)
    {
        int move;
        int originalrow = 0;
        if (color == 1)
        {
            move = -1;         //white pawn moves up (number of row decrease)
        }
        else if (color == -1)
        {
            move = 1;          //black pawn moves down (number of row increase)
        }
        if (color == 1)        //to move 2 boxes at thier first move
        {
            originalrow = 6;
        }
        else if (color == -1)
        {
            originalrow = 1;
        }
        if (startingRow == originalrow && cols_moved == 0 && rows_moved == 2 * move &&
            board[startingRow + move][startingCol] == 0 && board[destinationRow][destinationCol] == 0)
        {
            return true;    //can move 2 if its thier first move
        }
        if (cols_moved == 0 && rows_moved == move && board[destinationRow][destinationCol] == 0)
        {
            return true;           //move one step forward
        }
        if (rows_moved == move && (cols_moved == 1 || cols_moved == -1) && target != 0 && (target * piece < 0))
        {
            return true;           //can move diagnal only if any opponent piece is there
        }
        return false;              //cannot move if none of above cases is true
    }

    //checking valid invalid knight moves
    if (typeofpiece == 3)
    {
        if ((abs(rows_moved) == 2 && abs(cols_moved) == 1) || (abs(rows_moved) == 1 && abs(cols_moved) == 2))
        {
            return true;           //can move in L shape only in any direction
        }
        return false;
    }

    //checking valid invalid rook moves
    if (typeofpiece == 2)
    {
        if (rows_moved == 0 || cols_moved == 0)       //can only move in straight lines
        {
            return ispathClear(board, startingRow, startingCol, destinationRow, destinationCol);
        }
        return false;
    }

    //checking valid invalid bishop moves
    if (typeofpiece == 4)
    {
        if (abs(rows_moved) == abs(cols_moved))    //for diagnal movement, number of row and column must be same
        {
            return ispathClear(board, startingRow, startingCol, destinationRow, destinationCol);
        }
        return false;
    }

    //checking valid invalid queen moves
    if (typeofpiece == 5)
    {
        if (rows_moved == 0 || cols_moved == 0)   //if moving straight (like rook)
        {
            return ispathClear(board, startingRow, startingCol, destinationRow, destinationCol);
        }

        if (abs(rows_moved) == abs(cols_moved))   //if moving diagnal (like bishop)
        {
            return ispathClear(board, startingRow, startingCol, destinationRow, destinationCol);
        }

        return false;
    }

    //checking valid invalid king moves
    if (typeofpiece == 6)
    {
        if (abs(rows_moved) <= 1 && abs(cols_moved) <= 1)     //to move one in any direction
        {                                     //<= used because if one of these is 1, the other can be 0 to move in straight line
            int temp[8][8];
            for (int row = 0; row < 8; row++)        //copying the board temporarily
            {
                for (int column = 0; column < 8; column++)
                {
                    temp[row][column] = board[row][column];
                }
            }
            temp[destinationRow][destinationCol] = temp[startingRow][startingCol];
            temp[startingRow][startingCol] = 0;     //immitating the move, what will happen if king makes this move

            if (KingInCheck(temp, piece > 0 ? 1 : -1) == true)   //if the move brings it in check, it cannot move
            {
                return false;
            }
            return true;               //else it can move 1 in any direction 
        }
        return false;             //trying to move more than 1
    }
    return false;           //none of pieces, or none of above true
}
int findKingRow(int board[8][8], int kingColor)
{
    int kingnumber;
    if (kingColor == 1)
    {
        kingnumber = 6;
    }
    else
    {
        kingnumber = -6;
    }
    for (int row = 0; row < 8; ++row)
    {
        for (int column = 0; column < 8; ++column)
        {
            if (board[row][column] == kingnumber)
            {
                return row;
            }
        }
    }
    return -1;
}
bool KingInCheck(int board[8][8], int kingColor)
{
    int kingnumber = 0;
    if (kingColor == 1)
    {
        kingnumber = 6;
    }
    else if (kingColor == -1)
    {
        kingnumber = -6;
    }
    int kingrow = -1, kingcol = -1;
    for (int row = 0; row < 8; ++row)
    {
        for (int column = 0; column < 8; ++column)
        {
            if (board[row][column] == kingnumber)
            {
                kingrow = row;
                kingcol = column;
                break;
            }
        }
        if (kingrow != -1)
        {
            break;
        }
    }
    if (kingrow == -1)        //if king not found
    {
        return false;
    }
    //check every opponent piece, if it can move to king square then king is in check
    for (int row = 0; row < 8; row++)
    {
        for (int column = 0; column < 8; column++)
        {
            if (board[row][column] == 0)
            {
                continue;
            }
            if ((board[row][column] > 0 && kingColor == 1) || (board[row][column] < 0 && kingColor == -1))
            {
                continue;             //same color pieces
            }
            if (canMove(board, row, column, kingrow, kingcol) == true)
            {
                return true;
            }
        }
    }
    return false;               //not in check
}
bool hasAnyLegalMove(int board[8][8], int turn)
{
    int tempBoard[8][8];
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            int piece = board[row][col];
            if (piece == 0)
            {
                continue;
            }
            if ((turn == 1 && piece < 0) || (turn == -1 && piece > 0))
            {
                continue;               //only look at pieces belonging to the same color
            }
            for (int targetR = 0; targetR < 8; targetR++)  //try to move this piece to every possible place
            {
                for (int targetC = 0; targetC < 8; targetC++)
                {
                    if (canMove(board, row, col, targetR, targetC) == false)
                    {
                        continue;
                    }
                    for (int rr = 0; rr < 8; rr++)         //making temporary board
                    {
                        for (int cc = 0; cc < 8; cc++)
                        {
                            tempBoard[rr][cc] = board[rr][cc];
                        }
                    }
                    int movingPiece = tempBoard[row][col];
                    tempBoard[targetR][targetC] = movingPiece;
                    tempBoard[row][col] = 0;

                    if (KingInCheck(tempBoard, turn) == false) //if king is not in check after move
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;       //if king is in check after every move
}
bool Checkmate(int board[8][8], int turn)
{
    if (KingInCheck(board, turn) == false)
    {
        return false;
    }
    if (hasAnyLegalMove(board, turn) == true)
    {
        return false;
    }
    return true;         //checkmate!
}
bool Stalemate(int board[8][8], int turn)
{
    if (KingInCheck(board, turn) == true)
    {
        return false;
    }
    if (hasAnyLegalMove(board, turn) == true)
    {
        return false;
    }
    return true;     //game draw
}
void clearLegalMoves(bool legalMoves[8][8])
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            legalMoves[row][col] = false;
        }
    }
}
void drawLabels(sf::RenderWindow& window, sf::Font& font, float squareX, float squareY,
    float boardOffsetX, float boardOffsetY)
{
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(20);

    for (int row = 0; row < 8; row++)
    {
        int number = 8 - row;      //starting from 8 till 1
        text.setString(std::to_string(number));
        text.setPosition(boardOffsetX - 25, boardOffsetY + row * squareY + squareY / 3);
        window.draw(text);
    }
    for (int col = 0; col < 8; col++)
    {
        char letter = 'A' + col;
        text.setString(letter);
        text.setPosition(boardOffsetX + col * squareX + squareX / 3, boardOffsetY + 8 * squareY + 5);
        window.draw(text);
    }
}

//MAIN
int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Chess Game", sf::Style::Fullscreen);

    sf::Font font;
    font.loadFromFile("Font/ARIAL.ttf");

    float width = window.getSize().x;
    float height = window.getSize().y;
    height = height - (height * 0.05);

    float boardWidth = width * 0.70f;

    float squareY = height / 8.0f;
    float squareX = 105;

    float boardOffsetX = 100;   // distance from left edge
    float boardOffsetY = (height - 8 * squareY) / 2;

    int board[8][8];
    initializeBoard(board);

    bool legalMoves[8][8];

    // All textures
    sf::Texture whitepawn, blackpawn, whiterook, blackrook;
    sf::Texture whitehorse, blackhorse, whitebishop, blackbishop;
    sf::Texture whitequeen, blackqueen, whiteking, blackking;

    loadTextures(whitepawn, blackpawn, whiterook, blackrook, whitehorse, blackhorse, whitebishop, blackbishop,
        whitequeen, blackqueen, whiteking, blackking);

    float scaleX = squareX / whitepawn.getSize().x;
    float scaleY = squareY / whitepawn.getSize().y;

    int turn = 1;     //start with white's turn, then -1 for black's turn
    bool selected = false;
    int selectedrow = -1, selectedcol = -1;    //when none selected at first
    bool gameOver = false;
    string statusText = "White's turn";
    int whiteScore = 0;
    int blackScore = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();

            if (gameOver == false && event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                float mx = event.mouseButton.x;
                float my = event.mouseButton.y;
                int clickedCol = (mx - boardOffsetX) / squareX;
                int clickedRow = (my - boardOffsetY) / squareY;

                if (onBoard(clickedRow, clickedCol) == false)
                {
                    continue;
                }
                int clickedPiece = board[clickedRow][clickedCol];
                if (selected == false)
                {
                    //try to select a piece of current player's color
                    if (clickedPiece != 0 && ((clickedPiece > 0 && turn == 1) || (clickedPiece < 0 && turn == -1)))
                    {
                        selected = true;
                        selectedrow = clickedRow;
                        selectedcol = clickedCol;
                        clearLegalMoves(legalMoves);

                        for (int r = 0; r < 8; r++)
                        {
                            for (int c = 0; c < 8; c++)
                            {
                                if (canMove(board, selectedrow, selectedcol, r, c))
                                {
                                    legalMoves[r][c] = true;
                                }
                            }
                        }
                    }
                }
                else
                {
                    //if user clicks the same square, deselect
                    if (clickedRow == selectedrow && clickedCol == selectedcol)
                    {
                        selected = false;
                        selectedrow = -1;
                        selectedcol = -1;
                        clearLegalMoves(legalMoves);
                        continue;
                    }
                    if (canMove(board, selectedrow, selectedcol, clickedRow, clickedCol) == true)
                    {
                        int move = board[selectedrow][selectedcol];      //pick the piece from starting box
                        int captured = board[clickedRow][clickedCol];    //capture if any other piece exists there
                        if (captured != 0)
                        {
                            int value = 0;
                            if (captured == 1 || captured == -1)      //pawns
                            {
                                value = 1;
                            }
                            else if (captured == 3 || captured == -3)    //bishop
                            {
                                value = 3;
                            }
                            else if (captured == 4 || captured == -4)    //knight
                            {
                                value = 3;
                            }
                            else if (captured == 2 || captured == -2)     //rook
                            {
                                value = 5;
                            }
                            else if (captured == 5 || captured == -5)     //queen
                            {
                                value = 9;
                            }
                            if (captured > 0)   //white piece was captured, black gets points
                            {
                                blackScore = blackScore + value;
                            }
                            else                //black piece captured, white gets points
                            {
                                whiteScore = whiteScore + value;
                            }
                        }
                        board[clickedRow][clickedCol] = move;            //move the piece to destination
                        board[selectedrow][selectedcol] = 0;             //empty the place from where piece moved

                        bool whiteKingAlive = false;          //checking if any king is captured
                        bool blackKingAlive = false;
                        for (int rr = 0; rr < 8; rr++)
                        {
                            for (int cc = 0; cc < 8; cc++)
                            {
                                if (board[rr][cc] == 6)
                                {
                                    whiteKingAlive = true;
                                }
                                if (board[rr][cc] == -6)
                                {
                                    blackKingAlive = true;
                                }
                            }
                        }
                        if (whiteKingAlive == false || blackKingAlive == false)
                        {
                            gameOver = true;         //if captured,game over
                            if (whiteKingAlive == false)
                            {
                                statusText = "White king captured!\nWinner: Black";
                            }
                            else
                            {
                                statusText = "Black king captured!\nWinner: White";
                            }
                        }

                        int nextTurn = -1 * turn;   //checking if this move causes checkmate on enemy
                        if (Checkmate(board, nextTurn) == true)
                        {
                            gameOver = true;
                            if (nextTurn == 1)
                            {
                                statusText = "White is checkmated!\nWinner: Black";
                            }
                            else
                            {
                                statusText = "Black is checkmated!\nWinner: White";
                            }
                        }
                        else if (Stalemate(board, nextTurn))    //checking stalemate
                        {
                            gameOver = true;
                            statusText = "Stalemate\nGame Draw!";
                        }
                        else
                        {
                            turn = -1 * turn;
                        }
                        //pawn promotion (white pawn to white queen)
                        if (board[clickedRow][clickedCol] == 1 && clickedRow == 0)
                        {
                            board[clickedRow][clickedCol] = 5;
                        }
                        //pawn promotion (black pawn to black queen)
                        if (board[clickedRow][clickedCol] == -1 && clickedRow == 7)
                        {
                            board[clickedRow][clickedCol] = -5;
                        }

                        if (gameOver == false)
                        {
                            if (turn == 1)
                            {
                                statusText = "White's turn";
                            }
                            else
                            {
                                statusText = "Black's turn";
                            }
                        }
                        selected = false;
                        selectedrow = -1;
                        selectedcol = -1;
                        clearLegalMoves(legalMoves);
                    }
                    else     //if move invalid, then just check if it is trying to select any other of its piece
                    {
                        if (clickedPiece != 0 && ((clickedPiece > 0 && turn == 1) || (clickedPiece < 0 && turn == -1)))
                        {
                            selectedrow = clickedRow;
                            selectedcol = clickedCol;

                            clearLegalMoves(legalMoves);
                            for (int r = 0; r < 8; r++)
                            {
                                for (int c = 0; c < 8; c++)
                                {
                                    if (canMove(board, selectedrow, selectedcol, r, c))
                                    {
                                        legalMoves[r][c] = true;
                                    }
                                }
                            }
                            selected = true;
                        }
                    }
                }
            }
        }
        window.clear();
        drawBoard(window, squareX, squareY, boardOffsetX, boardOffsetY);

        drawLabels(window, font, squareX, squareY, boardOffsetX, boardOffsetY);

        bool whiteinCheck = KingInCheck(board, +1);
        bool blackinCheck = KingInCheck(board, -1);

        if (whiteinCheck)
        {
            int kingrow = -1, kingcol = -1;

            //first find the white king's position
            for (int row = 0; row < 8; ++row)
            {
                for (int col = 0; col < 8; ++col)
                {
                    if (board[row][col] == 6)
                    {
                        kingrow = row;
                        kingcol = col;
                        break;
                    }
                }
                if (kingrow != -1) break;   //stop scanning once king is found
            }

            //highlighting king in check
            if (kingrow != -1)
            {
                sf::RectangleShape rect(sf::Vector2f(squareX, squareY));
                rect.setPosition(boardOffsetX + kingcol * squareX, boardOffsetY + kingrow * squareY);
                rect.setFillColor(sf::Color(255, 0, 0, 100));
                window.draw(rect);
            }
        }
        if (blackinCheck)
        {
            int kingrow = -1, kingcol = -1;

            //first find the white king's position
            for (int row = 0; row < 8; ++row)
            {
                for (int col = 0; col < 8; ++col)
                {
                    if (board[row][col] == -6)
                    {
                        kingrow = row;
                        kingcol = col;
                        break;
                    }
                }
                if (kingrow != -1)
                {
                    break;   //stop scanning once king is found
                }
            }

            //highlight king in check
            if (kingrow != -1)
            {
                sf::RectangleShape rect(sf::Vector2f(squareX, squareY));
                rect.setPosition(boardOffsetX + kingcol * squareX, boardOffsetY + kingrow * squareY);
                rect.setFillColor(sf::Color(255, 0, 0, 100));
                window.draw(rect);
            }
        }
        if (selected && onBoard(selectedrow, selectedcol))
        {
            sf::RectangleShape selRect(sf::Vector2f(squareX, squareY));
            selRect.setPosition(boardOffsetX + selectedcol * squareX, boardOffsetY + selectedrow * squareY);
            selRect.setFillColor(sf::Color(0, 0, 0, 0));
            selRect.setOutlineThickness(4.0f);
            selRect.setOutlineColor(sf::Color::Green);
            window.draw(selRect);
        }
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                if (legalMoves[r][c] == true)
                {
                    sf::RectangleShape rect(sf::Vector2f(squareX, squareY));
                    rect.setPosition(boardOffsetX + c * squareX, boardOffsetY + r * squareY);
                    rect.setFillColor(sf::Color(0, 255, 0, 60)); // green highlight
                    window.draw(rect);
                }
            }
        }

        drawPieces(window, board, squareX, squareY, scaleX, scaleY, whitepawn, blackpawn, whiterook, blackrook,
            whitehorse, blackhorse, whitebishop, blackbishop, whitequeen, blackqueen, whiteking, blackking, boardOffsetX, boardOffsetY);

        float textX = boardOffsetX + 8 * squareX + 30;  // right of board
        float textY = 350;

        float text_scoreX = boardOffsetX + 8 * squareX + 30;  // right of board
        float text_scoreY = 420;

        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(30);
        t.setFillColor(sf::Color::White);
        t.setPosition(textX, textY);
        t.setString(
            statusText +
            (whiteinCheck || blackinCheck ? "\n\n\n\nCHECK!" : "")
        );
        window.draw(t);

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(text_scoreX, text_scoreY);
        scoreText.setString("White Score: " + to_string(whiteScore) +
            "\nBlack Score: " + to_string(blackScore));
        window.draw(scoreText);

        window.display();
    }
    return 0;
}