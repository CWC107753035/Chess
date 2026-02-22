#include "Board.h"
#include <iostream>

int main() {
    Board myBoard;

    // Test 1: Knight in the middle (d4 = index 27)
    // We expect a full "circle" of 8 moves.
    Bitboard b4bishop = 1ULL << 27; 
    std::cout << "Bishop on d4 (Middle):" << std::endl;
    myBoard.printBitboard(myBoard.getBishopMoves(27));

    // Test 2: Knight on the edge (a1 = index 0)
    // We expect ONLY 2 moves (b3 and c2). 
    // If you see more than 2, your masks are broken!
    // Bitboard edgeKnight = 1ULL << 0;
    // std::cout << "Knight on a1 (Corner):" << std::endl;
    // myBoard.printBitboard(myBoard.getKnightMoves(edgeKnight));

    return 0;
}