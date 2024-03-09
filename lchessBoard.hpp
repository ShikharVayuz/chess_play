/*
use at own risk
*/
#pragma once

#include "lchess_includes.hpp"

#include "lchessThreatMap.hpp"
class lchessThreatMap;



#define EMPTY 0x00

#define WHITE 0x10
#define BLACK 0x20

#define WHITE_PAWN 0x10
#define WHITE_ROOK 0x11
#define WHITE_KNIGHT 0x12
#define WHITE_BISHOP 0x13
#define WHITE_QUEEN 0x14
#define WHITE_KING 0x15

#define BLACK_PAWN 0x20
#define BLACK_ROOK 0x21
#define BLACK_KNIGHT 0x22
#define BLACK_BISHOP 0x23
#define BLACK_QUEEN 0x24
#define BLACK_KING 0x25



class lchessMove
{
public:
	BYTE from;
	BYTE to;
	BYTE piece;
	BYTE enPassant;

	inline void update( const BYTE from , const BYTE to , const BYTE piece , const BYTE enPassant = false )
	{
		this->from = from;
		this->to = to;
		this->piece = piece;
		this->enPassant = enPassant;
	}

	inline void update( const lchessMove& move )
	{
		this->from = move.from;
		this->to = move.to;
		this->piece = move.piece;
		this->enPassant = move.enPassant;
	}

	inline bool equals( const lchessMove& move ) const
	{
		if ( this->from != move.from ) return false;
		if ( this->to != move.to ) return false;
		if ( this->piece != move.piece ) return false;
		if ( this->enPassant != move.enPassant ) return false;
		return true;
	}

	inline int fromX() const
	{
		return this->from%8;
	}

	inline int fromY() const
	{
		return this->from/8;
	}

	inline int toX() const
	{
		return this->to%8;
	}

	inline int toY() const
	{
		return this->to/8;
	}

	// returns true if the move is a castle move
	inline bool isCastle() const
	{
		if ( this->piece == WHITE_KING && this->from == 4 && this->to == 2 ) return true;
		if ( this->piece == WHITE_KING && this->from == 4 && this->to == 6 ) return true;
		if ( this->piece == BLACK_KING && this->from == 60 && this->to == 58 ) return true;
		if ( this->piece == BLACK_KING && this->from == 60 && this->to == 62 ) return true;
		return false;
	}

	void print() const;
private:

};



/*
a simple chess board representation with a single byte per piece
*/
class lchessBoard
{
public:
	lchessBoard();
	virtual ~lchessBoard();

	void init();

	void getLegalMoves( std::vector< lchessMove >& moves , int& numberOfMoves , const BYTE color );

	int evaluatePosition() const;

	void move( const lchessMove& move );

	bool isWhiteInCheck() const;
	bool isBlackInCheck() const;

	int toX( const int index ) const;
	int toY( const int index ) const;

	bool isEmpty( const int index ) const;
	bool isColor( const int index , const BYTE color ) const;
	bool isWhite( const int index ) const;
	bool isBlack( const int index ) const;

	BYTE getPiece( const int index ) const;
	BYTE getPiece( const int x , const int y ) const;
	BYTE getColor( const int x , const int y ) const;
	lchessGameState getGameState() const;

	static std::string toChessCoords( const int index );
	static std::string toChessCoords( const int x , const int y );
	void print() const;

	static void allocateMemory();

private:
	// the position
    BYTE board[64];

	lchessGameState gameState;

	// the threat map of the current position
	lchessThreatMap threatMap;

	// have the castle pieces moved
	bool b_whiteKingMoved;
	bool b_blackKingMoved;
	bool b_a1RookMoved;
	bool b_h1RookMoved;
	bool b_a8RookMoved;
	bool b_h8RookMoved;

	// these will be true for one move after a pawn has moved 2 squares for en passant captures
	bool b_whitePawnMoved[8];
	bool b_blackPawnMoved[8];

	void printPiece( const BYTE pieceType ) const;

	// a list to temporarily store all possible moves in a position before removing all illegal moves
	static std::vector< lchessMove > possibleMoves;
	static uint numberOfPossibleMoves;
};