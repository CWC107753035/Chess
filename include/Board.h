#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

// Each square is represented by an index from 0 (a1) to 63 (h8)
typedef uint64_t Bitboard;

class Board {
public:
    // We need 12 bitboards: (6 for white pieces, 6 for black pieces)
    Bitboard pieceBB[12];
    
    // Combined bitboards for easier calculation
    Bitboard whitePieces;
    Bitboard blackPieces;
    Bitboard allPieces;

    bool whiteToMove;

    Board() {
        resetBoard();
    }

    void resetBoard() {
        // Initialize the starting position using hex constants
        // Each hex represents 64 bits (the whole board)
        pieceBB[0] = 0x000000000000FF00; // White Pawns (Rank 2)
        pieceBB[1] = 0x0000000000000042; // White Knights (b1, g1)
        pieceBB[2] = 0x0000000000000024; // White Bishops (c1, f1)
        pieceBB[3] = 0x0000000000000081; // White Rooks (a1, h1)
        pieceBB[4] = 0x0000000000000008; // White Queen (d1)
        pieceBB[5] = 0x0000000000000010; // White King (e1)

        // ... we will fill in the Black pieces and the rest in the next step
        whiteToMove = true;
        updateCombinedBitboards();
    }

    void updateCombinedBitboards() {
        whitePieces = 0;
        for (int i = 0; i < 6; i++) whitePieces |= pieceBB[i];

        blackPieces = 0;
        for (int i = 6; i < 12; i++) blackPieces |= pieceBB[i];

        allPieces = whitePieces | blackPieces;
    }

    // A helper function to print a bitboard (vital for debugging!)
    void printBitboard(Bitboard bb) {
        for (int rank = 7; rank >= 0; rank--) {
            for (int file = 0; file < 8; file++) {
                int square = rank * 8 + file;
                if ((bb >> square) & 1) cout << " 1 ";
                else cout << " . ";
            }
            cout << endl;
        }
        cout << endl;
    }
};