#include <iostream>
#include <cstdint>
#include <vector>
#include "constants.h"

using namespace std;

// Each square is represented by an index from 0 (a1) to 63 (h8)
typedef uint64_t Bitboard;

class Board {
public:
    // 12 pieces
    Bitboard pieceBB[12];
    
    // Combined bitboards
    Bitboard whitePieces;
    Bitboard blackPieces;
    Bitboard allPieces;

    bool whiteToMove;

    Board() {
        resetBoard();
    }

    void resetBoard() {
        // white pieces in bit 
        pieceBB[0] = 0x000000000000FF00; // White Pawns (Rank 2)
        pieceBB[1] = 0x0000000000000042; // White Knights (b1, g1)
        pieceBB[2] = 0x0000000000000024; // White Bishops (c1, f1)
        pieceBB[3] = 0x0000000000000081; // White Rooks (a1, h1)
        pieceBB[4] = 0x0000000000000008; // White Queen (d1)
        pieceBB[5] = 0x0000000000000010; // White King (e1)

        //black pieces in bit
        pieceBB[6] = 0x00FF000000000000; // Pawns (Rank 7)
        pieceBB[7] = 0x4200000000000000; // Knights (b8, g8)
        pieceBB[8] = 0x2400000000000000; // Bishops (c8, f8)
        pieceBB[9] = 0x8100000000000000; // Rooks (a8, h8)
        pieceBB[10] = 0x0800000000000000; // Queen (d8)
        pieceBB[11] = 0x1000000000000000; // King (e8)

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

    void clearBoard(){
        for ( auto &i : pieceBB ){
            i = 0;
        }
        updateCombinedBitboards();
    }

    // A helper function to print a bitboard (dibug))
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

    //Moving function
    Bitboard getKnightMoves(Bitboard knight) {
    Bitboard moves = 0;

    // Up 2, Right 1 (+17) - Can't be on File H
    moves |= (knight << 17) & FILE_A;
    // Up 2, Left 1 (+15) - Can't be on File A
    moves |= (knight << 15) & FILE_H;
    // Up 1, Right 2 (+10) - Can't be on File G or H
    moves |= (knight << 10) & FILE_AB;
    // Up 1, Left 2 (+6) - Can't be on File A or B
    moves |= (knight << 6) & FILE_GH;

    // Down 2, Left 1 (-17) - Can't be on File A
    moves |= (knight >> 17) & FILE_H;
    // Down 2, Right 1 (-15) - Can't be on File H
    moves |= (knight >> 15) & FILE_A;
    // Down 1, Left 2 (-10) - Can't be on File A or B
    moves |= (knight >> 10) & FILE_GH;
    // Down 1, Right 2 (-6) - Can't be on File G or H
    moves |= (knight >> 6) & FILE_AB;

    return moves;
    }

    Bitboard getKingMoves(Bitboard king) {
        Bitboard moves = 0;

        // Vertical moves (No file masks needed as they don't wrap ranks)
        moves |= (king << 8); // Up
        moves |= (king >> 8); // Down

        // Left-side moves: Result cannot be on File H
        moves |= (king << 7) & FILE_H; // Up-Left (Wraps to H if not masked)
        moves |= (king >> 1) & FILE_H; // Left
        moves |= (king >> 9) & FILE_H; // Down-Left

        // Right-side moves: Result cannot be on File A
        moves |= (king << 9) & FILE_A; // Up-Right (Wraps to A if not masked)
        moves |= (king << 1) & FILE_A; // Right
        moves |= (king >> 7) & FILE_A; // Down-Right

        return moves;
    }

    Bitboard getRookMoves(int square, Bitboard allOccupancy) {
        Bitboard moves = 0;
        // North (+8)
        for (int s = square + 8; s < 64; s += 8) {
            moves |= (1ULL << s);
            if ((1ULL << s) & allOccupancy) break; // Stop if we hit any piece
        }
        // South (-8)
        for (int s = square - 8; s >= 0; s -= 8) {
            moves |= (1ULL << s);
            if ((1ULL << s) & allOccupancy) break; // Stop if we hit any piece
        }
        // East (+1) - Must stop at File H
        for (int s = square + 1; s < 64 && (s % 8 != 0); s++){
            moves |= (1ULL << s);
            if ((1ULL << s) & allOccupancy) break; // Stop if we hit any piece
        }
        // West (-1) - Must stop at File A
        for (int s = square - 1; s >= 0 && (s % 8 != 7); s--) {
            moves |= (1ULL << s);
            if ((1ULL << s) & allOccupancy) break; // Stop if we hit any piece
        }
        return moves;
    }

    Bitboard getBishopMoves(int square, Bitboard allOccupancy) {
        Bitboard moves = 0;
        
        // North-East (+9)
        for (int s = square + 9; s < 64 && (s % 8 != 0); s += 9){
            moves |= (1ULL << s);
            if ((1ULL << s) & allOccupancy) break; // Stop if we hit any piece
        }
        // North-West (+7)
        for (int s = square + 7; s < 64 && (s % 8 != 7); s += 7){
            moves |= (1ULL << s);
            if ((1ULL << s) & allOccupancy) break; // Stop if we hit any piece
        }
        // South-East (-7)
        for (int s = square - 7; s >= 0 && (s % 8 != 0); s -= 7){
            moves |= (1ULL << s);
            if ((1ULL << s) & allOccupancy) break; // Stop if we hit any piece
        }
        // South-West (-9)
        for (int s = square - 9; s >= 0 && (s % 8 != 7); s -= 9){
            moves |= (1ULL << s);
            if ((1ULL << s) & allOccupancy) break; // Stop if we hit any piece
        }
        return moves;
    }
    
    Bitboard getQueenMoves(int square, Bitboard allOccupancy) {
    return getRookMoves(square, allOccupancy) | getBishopMoves(square, allOccupancy);
    }

    Bitboard getWhitePawnMoves(Bitboard pawn) {
        Bitboard moves = 0;
        
        // Single Push (+8)
        moves |= (pawn << 8);

        // Double Push (+16) - Only if on Rank 2
        // We check this by seeing if the pawn is between index 8 and 15
        Bitboard rank2Mask = 0x000000000000FF00;
        if (pawn & rank2Mask) {
            moves |= (pawn << 16);
        }

        // Captures (Diagonal)
        moves |= (pawn << 7) & FILE_H; // Capture Left (Destination can't be H)
        moves |= (pawn << 9) & FILE_A; // Capture Right (Destination can't be A)

        return moves;
    }

    Bitboard getBlackPawnMoves(Bitboard pawn) {
        Bitboard moves = 0;

        // Single Push (-8)
        moves |= (pawn >> 8);

        // Double Push (-16) - Only if on Rank 7
        Bitboard rank7Mask = 0x00FF000000000000;
        if (pawn & rank7Mask) {
            moves |= (pawn >> 16);
        }

        // Captures (Diagonal)
        moves |= (pawn >> 7) & FILE_A; // Capture Left (Destination can't be A)
        moves |= (pawn >> 9) & FILE_H; // Capture Right (Destination can't be H)

        return moves;
    }
    
    Bitboard getPawnMoves(Bitboard pawn, bool isWhite) {
        return isWhite ? getWhitePawnMoves(pawn) : getBlackPawnMoves(pawn);
    }

    void generateAllMoves() {
        Bitboard friendly = whiteToMove ? whitePieces : blackPieces;
        Bitboard enemy = whiteToMove ? blackPieces : whitePieces;
        Bitboard allMoves = 0;
        Bitboard allOccupancy = whitePieces | blackPieces;

        // We need to iterate through each piece type (0-5 for White, 6-11 for Black)
        int start = whiteToMove ? 0 : 6;
        int end = start + 6;

        for (int type = start; type < end; type++) {
            Bitboard pieces = pieceBB[type];
            
            // While there are still pieces of this type on the board
            while (pieces) {
                int sq = getLSBIndex(pieces); // Get the index of the first piece
                Bitboard moves = 0;

                // Call the correct logic based on piece type
                // Note: type % 6 gives us 0=Pawn, 1=Knight, etc. regardless of color
                int pieceType = type % 6;
                if (pieceType == 1) moves = getKnightMoves(1ULL << sq);
                else if (pieceType == 2) moves = getBishopMoves(sq,allOccupancy);
                else if (pieceType == 3) moves = getRookMoves(sq,allOccupancy);
                else if (pieceType == 4) moves = getQueenMoves(sq,allOccupancy);
                else if (pieceType == 5) moves = getKingMoves(1ULL << sq);
                else if (pieceType == 0) moves = getPawnMoves(1ULL << sq, whiteToMove);
                // Apply the "No Friendly Fire" filter
                moves &= ~friendly;
                
                allMoves |= moves;
                
                // Remove the piece we just processed so we can find the next one
                pieces &= (pieces - 1); 
            }
        }
        
        std::cout << (whiteToMove ? "White" : "Black") << " total reach:" << std::endl;
        printBitboard(allMoves);
    }

    void makeMove(int from, int to, int pieceType) {
        Bitboard fromBB = 1ULL << from;
        Bitboard toBB = 1ULL << to;
        Bitboard moveMask = fromBB | toBB;

        // 1. Capture Logic: If the 'to' square has an enemy, we must delete it
        Bitboard enemyPieces = whiteToMove ? blackPieces : whitePieces;
        if (toBB & enemyPieces) {
            // Find which specific enemy piece is there and 'pop' it
            int start = whiteToMove ? 6 : 0; // If white moves, look at black pieces (6-11)
            int end = start + 6;
            for (int i = start; i < end; i++) {
                if (pieceBB[i] & toBB) {
                    pieceBB[i] ^= toBB; // XOR again to flip the '1' to '0'
                    break;
                }
            }
        }

        // 2. Move our piece: Toggle 'from' off and 'to' on
        pieceBB[pieceType] ^= moveMask;

        // 3. Update the state
        updateCombinedBitboards();
        whiteToMove = !whiteToMove; // Switch the turn!
    }
};