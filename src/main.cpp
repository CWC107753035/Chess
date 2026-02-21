#include "Board.h"

int main() {
    Board myBoard;
    std::cout << "White Pawns Bitboard:" << std::endl;
    myBoard.printBitboard(myBoard.pieceBB[0]);
    return 0;
}