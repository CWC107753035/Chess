#include "Board.h"
#include <iostream>

int main() {
    Board myBoard;
    // Manually set up a test state
    myBoard.clearBoard(); // You might need to write this small helper to set all pieceBB to 0
    myBoard.pieceBB[3] = 1ULL << 27; // White Rook on d4
    myBoard.pieceBB[6] = 1ULL << 43; // Black Pawn on d6 (Blocks the Rook's North ray)
    myBoard.updateCombinedBitboards();
    myBoard.printBitboard(myBoard.allPieces);
    myBoard.makeMove(27,43,3);
    // myBoard.updateCombinedBitboards();

    myBoard.printBitboard(myBoard.whitePieces);

    return 0;
}