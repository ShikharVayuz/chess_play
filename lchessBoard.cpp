/*
use at own risk
*/

// The goal of this assignment is to give you the opportunity to work with tools that help us become more effective in uncovering software defects.
// For this assignment you will implement a move validation engine for the game of chess, use a static analysis tool to help you identify potential defects before you start testing, develop test cases for your program, and collect code coverage data to help you determine the adequacy of your testing effort.
// The purpose of the program that you will develop will be to compute all legal moves for a piece on a given chessboard configuration.
// For example, given the board configuration below, your program should compute that the Rook located on square f1 can legally move only to any of the following squares: e1, d1, c1, b1, a1.
 
// For a detailed description of all the legal chess moves please refer to
// https://en.wikipedia.org/wiki/Rules_of_chess
// Input and Output Formats:
// Your program should accept as its input a board configuration that contains the position of white pieces, the position of black pieces, and the piece whose possible legal moves are to be computed.
// For example, the input for the above board configuration should look something like this:
// WHITE: Rf1, Kg1, Pf2, Ph2, Pg3 BLACK: Kb8, Ne8, Pa7, Pb7, Pc7, Ra5 PIECE TO MOVE: Rf1
// Please note the use of the capital letters K, Q, R, B, N, and P to identify the King, Queen, Rook, Bishop, Knight, and Pawn respectively.
// Given the above input description, your program should produce the following output:
// LEGAL MOVES FOR Rf1: e1, d1, c1, b1, a1
// Deliverables
// Please submit the following items by Saturday, Macrh 9, 2024.
// • The source code for your program.
// • The set of test cases you used to ensure that your program satisfies
// its requirements. (Please provide a Readme file with instructions on how to exercise the tests, and any information regarding the status of the program, e.g., “The program does not work correctly under these conditions...”).
// • A report showing the results from using a static analysis tool and code improvements you made to address some (or all) of the issues reported.
// • A report showing the final code coverage achieved.

#include "lchessBoard.hpp"
#include <cstring>


std::vector< lchessMove > lchessBoard::possibleMoves;
uint lchessBoard::numberOfPossibleMoves;



lchessBoard::lchessBoard()
{

}



lchessBoard::~lchessBoard()
{

}



void lchessBoard::init()
{
    memset( this->board , EMPTY , 64 );

	// white pieces
    this->board[0] = WHITE_ROOK;
    this->board[1] = WHITE_KNIGHT;
    this->board[2] = WHITE_BISHOP;
    this->board[3] = WHITE_QUEEN;
    this->board[4] = WHITE_KING;
    this->board[5] = WHITE_BISHOP;
    this->board[6] = WHITE_KNIGHT;
    this->board[7] = WHITE_ROOK;
	// white pawns
    for ( int i = 8 ; i < 16 ; ++i ) this->board[i] = WHITE_PAWN;

	// black pawns
    for ( int i = 48 ; i < 56 ; ++i ) this->board[i] = BLACK_PAWN;
	// black pieces
    this->board[56] = BLACK_ROOK;
    this->board[57] = BLACK_KNIGHT;
    this->board[58] = BLACK_BISHOP;
    this->board[59] = BLACK_QUEEN;
    this->board[60] = BLACK_KING;
    this->board[61] = BLACK_BISHOP;
    this->board[62] = BLACK_KNIGHT;
    this->board[63] = BLACK_ROOK;

	// initialize the threat map
	this->threatMap = lchessThreatMap::fromBoard( *this );

	// reset the castle moves
	this->b_whiteKingMoved = false;
	this->b_blackKingMoved = false;
	this->b_a1RookMoved = false;
	this->b_h1RookMoved = false;
	this->b_a8RookMoved = false;
	this->b_h8RookMoved = false;

	// reset the en passant moves
	for ( int i = 0 ; i < 8 ; ++i )
	{
		this->b_whitePawnMoved[i] = false;
		this->b_blackPawnMoved[i] = false;
	}

	this->gameState = lchessGameState::ONGOING;
}



void lchessBoard::getLegalMoves( std::vector< lchessMove >& moves , int& numberOfMoves , const BYTE color )
{
	Timer timer;
	timer.start();


	// reset the en passant moves
	if ( color == WHITE )
	{
		for ( int i = 0 ; i < 8 ; ++i )
		{
			this->b_whitePawnMoved[i] = false;
		}
	}
	else if ( color == BLACK )
	{
		for ( int i = 0 ; i < 8 ; ++i )
		{
			this->b_blackPawnMoved[i] = false;
		}
	}

	// count pieces for both colors because king vs king is always a draw
	int numberOfColor1Pieces = 0;
	int numberOfColor2Pieces = 0;

	// find all possible moves, this may include some illegal moves that have to be removed later
	numberOfPossibleMoves = 0;
	for ( int i = 0 ; i < 64 ; ++i )
	{
		BYTE piece = this->board[i];
		int x = this->toX(i);
		int y = this->toY(i);
		if ( this->getColor( x , y ) == color )
		{
			++numberOfColor1Pieces;
			if ( piece == WHITE_PAWN )
			{
				// 1 square up
				if ( i+8 < 64 && this->board[i+8] == EMPTY )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , i+8 , WHITE_PAWN );
				}
				// 2 squares up
				if ( y == 1 && this->board[i+16] == EMPTY )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , i+16 , WHITE_PAWN );
				}
				// captures
				if ( y+1 < 8 && x-1 >= 0 && this->isBlack( (y+1)*8+(x-1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+(x-1) , WHITE_PAWN );
				}
				if ( y+1 < 8 && x+1 < 8 && this->isBlack( (y+1)*8+(x+1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+(x+1) , WHITE_PAWN );
				}
				// en passant
				if ( y == 4 )
				{
					if ( x-1 >= 0 && this->b_blackPawnMoved[x-1] )
					{
						possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+(x-1) , WHITE_PAWN , true );
					}
					if ( x+1 < 8 && this->b_blackPawnMoved[x+1] )
					{
						possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+(x+1) , WHITE_PAWN , true );
					}
				}
			}
			else if ( piece == BLACK_PAWN )
			{
				// 1 square down
				if ( i-8 >= 0 && this->board[i-8] == EMPTY )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , i-8 , BLACK_PAWN );
				}
				// 2 squares down
				if ( y == 6 && this->board[i-16] == EMPTY )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , i-16 , BLACK_PAWN );
				}
				// captures
				if ( y-1 >= 0 && x-1 >= 0 && this->isWhite( (y-1)*8+(x-1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+(x-1) , BLACK_PAWN );
				}
				if ( y-1 >= 0 && x+1 < 8 && this->isWhite( (y-1)*8+(x+1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+(x+1) , BLACK_PAWN );
				}
				// en passant
				if ( y == 3 )
				{
					if ( x-1 >= 0 && this->b_whitePawnMoved[x-1] )
					{
						possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+(x-1) , BLACK_PAWN , true );
					}
					if ( x+1 < 8 && this->b_whitePawnMoved[x+1] )
					{
						possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+(x+1) , BLACK_PAWN , true );
					}
				}
			}
			else if ( piece == WHITE_KNIGHT || piece == BLACK_KNIGHT )
			{
				// down
				if ( x-1 >= 0 && y-2 >= 0 && this->getColor( x-1 , y-2 ) != color )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-2)*8+(x-1) , piece );
				}
				if ( x+1 < 8 && y-2 >= 0 && this->getColor( x+1 , y-2 ) != color )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-2)*8+(x+1) , piece );
				}
				// up
				if ( x-1 >= 0 && y+2 < 8 && this->getColor( x-1 , y+2 ) != color )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+2)*8+(x-1) , piece );
				}
				if ( x+1 < 8 && y+2 < 8 && this->getColor( x+1 , y+2 ) != color )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+2)*8+(x+1) , piece );
				}
				// left
				if ( x-2 >= 0 && y-1 >= 0 && this->getColor( x-2 , y-1 ) != color )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+(x-2) , piece );
				}
				if ( x-2 >= 0 && y+1 < 8 && this->getColor( x-2 , y+1 ) != color )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+(x-2) , piece );
				}
				// right
				if ( x+2 < 8 && y-1 >= 0 && this->getColor( x+2 , y-1 ) != color )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+(x+2) , piece );
				}
				if ( x+2 < 8 && y+1 < 8 && this->getColor( x+2 , y+1 ) != color )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+(x+2) , piece );
				}
			}
			else if ( piece == WHITE_ROOK || piece == WHITE_BISHOP || piece == WHITE_QUEEN || piece == BLACK_ROOK || piece == BLACK_BISHOP || piece == BLACK_QUEEN )
			{
				// rook moves
				if ( piece == WHITE_ROOK || piece == WHITE_QUEEN || piece == BLACK_ROOK || piece == BLACK_QUEEN )
				{
					// down
					for ( int r = y-1 ; r >= 0 ; --r )
					{
						// stop at pieces with the same color
						if ( this->getColor( x , r ) == color ) break;
						possibleMoves[numberOfPossibleMoves++].update( i , r*8+x , piece );
						// stop at enemy pieces
						if ( this->getColor( x , r ) != EMPTY && this->getColor( x , r ) != color ) break;
					}
					// up
					for ( int r = y+1 ; r < 8 ; ++r )
					{
						// stop at pieces with the same color
						if ( this->getColor( x , r ) == color ) break;
						possibleMoves[numberOfPossibleMoves++].update( i , r*8+x , piece );
						// stop at enemy pieces
						if ( this->getColor( x , r ) != EMPTY && this->getColor( x , r ) != color ) break;
					}
					// left
					for ( int f = x-1 ; f >= 0 ; --f )
					{
						// stop at pieces with the same color
						if ( this->getColor( f , y ) == color ) break;
						possibleMoves[numberOfPossibleMoves++].update( i , y*8+f , piece );
						// stop at enemy pieces
						if ( this->getColor( f , y ) != EMPTY && this->getColor( f , y ) != color ) break;
					}
					// right
					for ( int f = x+1 ; f < 8 ; ++f )
					{
						// stop at pieces with the same color
						if ( this->getColor( f , y ) == color ) break;
						possibleMoves[numberOfPossibleMoves++].update( i , y*8+f , piece );
						// stop at enemy pieces
						if ( this->getColor( f , y ) != EMPTY && this->getColor( f , y ) != color ) break;
					}
				}
				// bishop moves
				if ( piece == WHITE_BISHOP || piece == WHITE_QUEEN || piece == BLACK_BISHOP || piece == BLACK_QUEEN )
				{
					// down left
					for ( int r = y-1 , f = x-1 ; r >= 0 && f >= 0 ; --r , --f )
					{
						// stop at pieces with the same color
						if ( this->getColor( f , r ) == color ) break;
						possibleMoves[numberOfPossibleMoves++].update( i , r*8+f , piece );
						// stop at enemy pieces
						if ( this->getColor( f , r ) != EMPTY && this->getColor( f , r ) != color ) break;
					}
					// down right
					for ( int r = y-1 , f = x+1 ; r >= 0 && f < 8 ; --r , ++f )
					{
						// stop at pieces with the same color
						if ( this->getColor( f , r ) == color ) break;
						possibleMoves[numberOfPossibleMoves++].update( i , r*8+f , piece );
						// stop at enemy pieces
						if ( this->getColor( f , r ) != EMPTY && this->getColor( f , r ) != color ) break;
					}
					// up left
					for ( int r = y+1 , f = x-1 ; r < 8 && f >= 0 ; ++r , --f )
					{
						// stop at pieces with the same color
						if ( this->getColor( f , r ) == color ) break;
						possibleMoves[numberOfPossibleMoves++].update( i , r*8+f , piece );
						// stop at enemy pieces
						if ( this->getColor( f , r ) != EMPTY && this->getColor( f , r ) != color ) break;
					}
					// up right
					for ( int r = y+1 , f = x+1 ; r < 8 && f < 8 ; ++r , ++f )
					{
						// stop at pieces with the same color
						if ( this->getColor( f , r ) == color ) break;
						possibleMoves[numberOfPossibleMoves++].update( i , r*8+f , piece );
						// stop at enemy pieces
						if ( this->getColor( f , r ) != EMPTY && this->getColor( f , r ) != color ) break;
					}
				}
			}
			else if ( piece == WHITE_KING )
			{
				// straight moves
				if ( y-1 >= 0 && !this->isWhite( (y-1)*8+x ) && !this->threatMap.isBlackThreat( (y-1)*8+x ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+x , WHITE_KING );
				}
				if ( y+1 < 8 && !this->isWhite( (y+1)*8+x ) && !this->threatMap.isBlackThreat( (y+1)*8+x ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+x , WHITE_KING );
				}
				if ( x-1 >= 0 && !this->isWhite( y*8+(x-1) ) && !this->threatMap.isBlackThreat( y*8+(x-1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , y*8+(x-1) , WHITE_KING );
				}
				if ( x+1 < 8 && !this->isWhite( y*8+(x+1) ) && !this->threatMap.isBlackThreat( y*8+(x+1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , y*8+(x+1) , WHITE_KING );
				}

				// diagonal moves
				if ( y-1 >= 0 && x-1 >= 0 && !this->isWhite( (y-1)*8+(x-1) ) && !this->threatMap.isBlackThreat( (y-1)*8+(x-1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+(x-1) , WHITE_KING );
				}
				if ( y-1 >= 0 && x+1 < 8 && !this->isWhite( (y-1)*8+(x+1) ) && !this->threatMap.isBlackThreat( (y-1)*8+(x+1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+(x+1) , WHITE_KING );
				}
				if ( y+1 < 8 && x-1 >= 0 && !this->isWhite( (y+1)*8+(x-1) ) && !this->threatMap.isBlackThreat( (y+1)*8+(x-1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+(x-1) , WHITE_KING );
				}
				if ( y+1 < 8 && x+1 < 8 && !this->isWhite( (y+1)*8+(x+1) ) && !this->threatMap.isBlackThreat( (y+1)*8+(x+1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+(x+1) , WHITE_KING );
				}

				// castles
				if ( !this->b_whiteKingMoved )
				{
					// queen side castle
					if ( !this->b_a1RookMoved && this->getPiece(0) == WHITE_ROOK )
					{
						// are the squares empty
						if ( this->isEmpty( 1 ) && this->isEmpty( 2 ) && this->isEmpty( 3 ) )
						{
							// is none of the squares under attack by black
							if ( !this->threatMap.isBlackThreat( 2 ) && !this->threatMap.isBlackThreat( 3 ) && !this->threatMap.isBlackThreat( 4 ) )
							{
								possibleMoves[numberOfPossibleMoves++].update( i , 2 , WHITE_KING );
							}
						}
					}
					// king side castle
					if ( !this->b_h1RookMoved && this->getPiece(7) == WHITE_ROOK )
					{
						// are the squares empty
						if ( this->isEmpty( 5 ) && this->isEmpty( 6 ) )
						{
							// is none of the squares under attack by black
							if ( !this->threatMap.isBlackThreat( 4 ) && !this->threatMap.isBlackThreat( 5 ) && !this->threatMap.isBlackThreat( 6 ) )
							{
								possibleMoves[numberOfPossibleMoves++].update( i , 6 , WHITE_KING );
							}
						}
					}
				}
			}
			else if ( piece == BLACK_KING )
			{
				// straight moves
				if ( y-1 >= 0 && !this->isBlack( (y-1)*8+x ) && !this->threatMap.isWhiteThreat( (y-1)*8+x ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+x , BLACK_KING );
				}
				if ( y+1 < 8 && !this->isBlack( (y+1)*8+x ) && !this->threatMap.isWhiteThreat( (y+1)*8+x ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+x , BLACK_KING );
				}
				if ( x-1 >= 0 && !this->isBlack( y*8+(x-1) ) && !this->threatMap.isWhiteThreat( y*8+(x-1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , y*8+(x-1) , BLACK_KING );
				}
				if ( x+1 < 8 && !this->isBlack( y*8+(x+1) ) && !this->threatMap.isWhiteThreat( y*8+(x+1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , y*8+(x+1) , BLACK_KING );
				}

				// diagonal moves
				if ( y-1 >= 0 && x-1 >= 0 && !this->isBlack( (y-1)*8+(x-1) ) && !this->threatMap.isWhiteThreat( (y-1)*8+(x-1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+(x-1) , BLACK_KING );
				}
				if ( y-1 >= 0 && x+1 < 8 && !this->isBlack( (y-1)*8+(x+1) ) && !this->threatMap.isWhiteThreat( (y-1)*8+(x+1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y-1)*8+(x+1) , BLACK_KING );
				}
				if ( y+1 < 8 && x-1 >= 0 && !this->isBlack( (y+1)*8+(x-1) ) && !this->threatMap.isWhiteThreat( (y+1)*8+(x-1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+(x-1) , BLACK_KING );
				}
				if ( y+1 < 8 && x+1 < 8 && !this->isBlack( (y+1)*8+(x+1) ) && !this->threatMap.isWhiteThreat( (y+1)*8+(x+1) ) )
				{
					possibleMoves[numberOfPossibleMoves++].update( i , (y+1)*8+(x+1) , BLACK_KING );
				}

				// castles
				if ( !this->b_blackKingMoved )
				{
					// queen side castle
					if ( !this->b_a8RookMoved && this->getPiece(56) == BLACK_ROOK )
					{
						// are the squares empty
						if ( this->isEmpty( 57 ) && this->isEmpty( 58 ) && this->isEmpty( 59 ) )
						{
							// is none of the squares under attack by white
							if ( !this->threatMap.isWhiteThreat( 58 ) && !this->threatMap.isWhiteThreat( 59 ) && !this->threatMap.isWhiteThreat( 60 ) )
							{
								possibleMoves[numberOfPossibleMoves++].update( i , 58 , BLACK_KING );
							}
						}
					}
					// king side castle
					if ( !this->b_h8RookMoved && this->getPiece(63) == BLACK_ROOK )
					{
						// are the squares empty
						if ( this->isEmpty( 61 ) && this->isEmpty( 62 ) )
						{
							// is none of the squares under attack by white
							if ( !this->threatMap.isWhiteThreat( 60 ) && !this->threatMap.isWhiteThreat( 61 ) && !this->threatMap.isWhiteThreat( 62 ) )
							{
								possibleMoves[numberOfPossibleMoves++].update( i , 62 , BLACK_KING );
							}
						}
					}
				}
			}
		}
		else if ( this->getColor( x , y ) != EMPTY )
		{
			++numberOfColor2Pieces;
		}
	}

	// remove all illegal moves
	if ( color == WHITE )
	{
		numberOfMoves = 0;
		for ( uint i = 0 ; i < numberOfPossibleMoves ; ++i )
		{
			// castling moves already cannot happen if the king is in check or if the king would end up in check
			if ( possibleMoves[i].isCastle() )
			{
				moves[numberOfMoves++].update( possibleMoves[i] );	
			}
			else
			{
				// do the move temporarily
				BYTE oldToSquare = this->board[this->possibleMoves[i].to];
				if ( this->possibleMoves[i].piece == WHITE_PAWN && this->possibleMoves[i].enPassant )
				{
					this->board[this->possibleMoves[i].from] = EMPTY;
					this->board[this->possibleMoves[i].to] = WHITE_PAWN;
					// remove the black pawn
					this->board[this->possibleMoves[i].fromY()*8+this->possibleMoves[i].toX()] = EMPTY;
				}
				else
				{
					this->board[this->possibleMoves[i].from] = EMPTY;
					this->board[this->possibleMoves[i].to] = this->possibleMoves[i].piece;
				}

				// check whether the white king is in check
				if ( !lchessThreatMap::isWhiteInCheck( *this ) )
				{
					moves[numberOfMoves++].update( possibleMoves[i] );	
				}

				//// calculate the threat map of the new position
				//this->threatMap = lchessThreatMap::fromBoard( *this );

				//// if white is not in check it is a legal move
				//if ( !this->isWhiteInCheck() )
				//{
				//	moves[numberOfMoves++].update( possibleMoves[i] );	
				//}

				// undo the move
				if ( this->possibleMoves[i].piece == WHITE_PAWN && this->possibleMoves[i].enPassant )
				{
					this->board[this->possibleMoves[i].from] = this->possibleMoves[i].piece;
					this->board[this->possibleMoves[i].to] = oldToSquare;
					// restore the black pawn
					this->board[this->possibleMoves[i].fromY()*8+this->possibleMoves[i].toX()] = BLACK_PAWN;
				}
				else
				{
					this->board[this->possibleMoves[i].from] = this->possibleMoves[i].piece;
					this->board[this->possibleMoves[i].to] = oldToSquare;
				}
			}
		}
	}
	/*
	BLACK
	*/
	else if ( color == BLACK )
	{
		numberOfMoves = 0;
		for ( uint i = 0 ; i < numberOfPossibleMoves ; ++i )
		{
			// castling moves already cannot happen if the king is in check or if the king would end up in check
			if ( possibleMoves[i].isCastle() )
			{
				moves[numberOfMoves++].update( possibleMoves[i] );	
			}
			else
			{
				// do the move temporarily
				BYTE oldToSquare = this->board[this->possibleMoves[i].to];
				if ( this->possibleMoves[i].piece == BLACK_PAWN && this->possibleMoves[i].enPassant )
				{
					this->board[this->possibleMoves[i].from] = EMPTY;
					this->board[this->possibleMoves[i].to] = BLACK_PAWN;
					// remove the white pawn
					this->board[this->possibleMoves[i].fromY()*8+this->possibleMoves[i].toX()] = EMPTY;
				}
				else
				{
					this->board[this->possibleMoves[i].from] = EMPTY;
					this->board[this->possibleMoves[i].to] = this->possibleMoves[i].piece;
				}

				// check whether the black king is in check
				if ( !lchessThreatMap::isBlackInCheck( *this ) )
				{
					moves[numberOfMoves++].update( possibleMoves[i] );	
				}

				//// calculate the threat map of the new position
				//this->threatMap = lchessThreatMap::fromBoard( *this );

				//// if white is not in check it is a legal move
				//if ( !this->isBlackInCheck() )
				//{
				//	moves[numberOfMoves++].update( possibleMoves[i] );	
				//}

				// undo the move
				if ( this->possibleMoves[i].piece == BLACK_PAWN && this->possibleMoves[i].enPassant )
				{
					this->board[this->possibleMoves[i].from] = this->possibleMoves[i].piece;
					this->board[this->possibleMoves[i].to] = oldToSquare;
					// restore the white pawn
					this->board[this->possibleMoves[i].fromY()*8+this->possibleMoves[i].toX()] = WHITE_PAWN;
				}
				else
				{
					this->board[this->possibleMoves[i].from] = this->possibleMoves[i].piece;
					this->board[this->possibleMoves[i].to] = oldToSquare;
				}
			}
		}
	}

	//// reset the threat map to the current position again
	//this->threatMap = lchessThreatMap::fromBoard( *this );


	// check mate detection
	if ( color == WHITE )
	{
		if ( numberOfMoves == 0 )
		{
			if ( this->isWhiteInCheck() )
			{
				this->gameState = lchessGameState::BLACKWIN;
				std::cout << "WHITE IS CHECKMATE" << std::endl;

			}
			else
			{
				this->gameState = lchessGameState::DRAW;
				std::cout << "DRAW" << std::endl;
			}
		}
	}
	else if ( color == BLACK )
	{
		if ( numberOfMoves == 0 )
		{
			if ( this->isBlackInCheck() )
			{
				this->gameState = lchessGameState::WHITEWIN;
				std::cout << "BLACK IS CHECKMATE" << std::endl;
			}
			else
			{
				this->gameState = lchessGameState::DRAW;
				std::cout << "DRAW" << std::endl;
			}
		}
	}

	if ( numberOfColor1Pieces == 1 && numberOfColor2Pieces == 1 )
	{
		this->gameState = lchessGameState::DRAW;
		std::cout << "DRAW (king vs king)" << std::endl;
	}

	// timer.stop();
	std::cout << timer.getTime() << "ms" << std::endl;
}



int lchessBoard::evaluatePosition() const
{
	int whiteCounter = 0;
	int blackCounter = 0;
	for ( int i = 0 ; i < 64 ; ++i )
	{
		switch ( this->board[i] )
		{
		case WHITE_PAWN: whiteCounter += 1; break;
		case WHITE_ROOK: whiteCounter += 5; break;
		case WHITE_KNIGHT: whiteCounter += 3; break;
		case WHITE_BISHOP: whiteCounter += 3; break;
		case WHITE_QUEEN: whiteCounter += 9; break;
		case WHITE_KING: whiteCounter += 1000; break;
		case BLACK_PAWN: blackCounter += 1; break;
		case BLACK_ROOK: blackCounter += 5; break;
		case BLACK_KNIGHT: blackCounter += 3; break;
		case BLACK_BISHOP: blackCounter += 3; break;
		case BLACK_QUEEN: blackCounter += 9; break;
		case BLACK_KING: blackCounter += 1000; break;
		}
	}
	return whiteCounter - blackCounter;
}



void lchessBoard::move( const lchessMove& move )
{
	// castles
	if ( move.piece == WHITE_KING && move.from == 4 && move.to == 2 )
	{
		this->board[4] = EMPTY;
		this->board[0] = EMPTY;
		this->board[2] = WHITE_KING;
		this->board[3] = WHITE_ROOK;
	}
	else if ( move.piece == WHITE_KING && move.from == 4 && move.to == 6 )
	{
		this->board[4] = EMPTY;
		this->board[7] = EMPTY;
		this->board[6] = WHITE_KING;
		this->board[5] = WHITE_ROOK;
	}
	else if ( move.piece == BLACK_KING && move.from == 60 && move.to == 58 )
	{
		this->board[60] = EMPTY;
		this->board[56] = EMPTY;
		this->board[58] = BLACK_KING;
		this->board[59] = BLACK_ROOK;
	}
	else if ( move.piece == BLACK_KING && move.from == 60 && move.to == 62 )
	{
		this->board[60] = EMPTY;
		this->board[63] = EMPTY;
		this->board[62] = BLACK_KING;
		this->board[61] = BLACK_ROOK;
	}
	// en passant
	else if ( move.piece == WHITE_PAWN && move.enPassant )
	{
		this->board[move.from] = EMPTY;
		this->board[move.to] = WHITE_PAWN;
		// remove the black pawn
		this->board[move.fromY()*8+move.toX()] = EMPTY;
	}
	else if ( move.piece == BLACK_PAWN && move.enPassant )
	{
		this->board[move.from] = EMPTY;
		this->board[move.to] = BLACK_PAWN;
		// remove the white pawn
		this->board[move.fromY()*8+move.toX()] = EMPTY;
	}
	else
	{
		this->board[move.from] = EMPTY;
		this->board[move.to] = move.piece;

		// auto queen promotion
		if ( move.piece == WHITE_PAWN && move.toY() == 7 ) this->board[move.to] = WHITE_QUEEN;
		if ( move.piece == BLACK_PAWN && move.toY() == 0 ) this->board[move.to] = BLACK_QUEEN;
	}

	if ( move.from == 4 ) this->b_whiteKingMoved = true;
	if ( move.from == 0 ) this->b_a1RookMoved = true;
	if ( move.from == 7 ) this->b_h1RookMoved = true;

	if ( move.from == 60 ) this->b_blackKingMoved = true;
	if ( move.from == 56 ) this->b_a8RookMoved = true;
	if ( move.from == 63 ) this->b_h8RookMoved = true;

	// set en passant flags to true if the pawn has moved 2 squares
	if ( move.piece == WHITE_PAWN && move.fromY() == 1 && move.toY() == 3 )
	{
		this->b_whitePawnMoved[move.fromX()] = true;
	}
	if ( move.piece == BLACK_PAWN && move.fromY() == 6 && move.toY() == 4 )
	{
		this->b_blackPawnMoved[move.fromX()] = true;
	}

	// recalculate threat map
	this->threatMap = lchessThreatMap::fromBoard( *this );
}



bool lchessBoard::isWhiteInCheck() const
{
	for ( int i = 0 ; i < 64 ; ++i )
	{
		if ( this->board[i] == WHITE_KING )
		{
			return this->threatMap.isBlackThreat(i);
		}
	}
	std::cout << "lchessBoard > isWhiteInCheck > no white king found, something is wrong" << std::endl;
	return true;
}



bool lchessBoard::isBlackInCheck() const
{
	for ( int i = 0 ; i < 64 ; ++i )
	{
		if ( this->board[i] == BLACK_KING )
		{
			return this->threatMap.isWhiteThreat(i);
		}
	}
	std::cout << "lchessBoard > isBlackInCheck > no black king found, something is wrong" << std::endl;
	return true;
}



int lchessBoard::toX( const int index ) const
{
	return index%8;
}



int lchessBoard::toY( const int index ) const
{
	return index/8;
}



bool lchessBoard::isEmpty( const int index ) const
{
	return this->board[index] == EMPTY;
}



bool lchessBoard::isColor( const int index , const BYTE color ) const
{
	if ( this->board[index] & color ) return true;
	else return false;
}



bool lchessBoard::isWhite( const int index ) const
{
	if ( this->board[index] & WHITE ) return true;
	else return false;
}



bool lchessBoard::isBlack( const int index ) const
{
	if ( this->board[index] & BLACK ) return true;
	else return false;
}



BYTE lchessBoard::getPiece( const int index ) const
{
	return this->board[index];
}



BYTE lchessBoard::getPiece( const int x , const int y ) const
{
	return this->board[y*8+x];
}



BYTE lchessBoard::getColor( const int x , const int y ) const
{
	if ( this->isWhite( y*8+x ) ) return WHITE;
	if ( this->isBlack( y*8+x ) ) return BLACK;
	return EMPTY;
}



lchessGameState lchessBoard::getGameState() const
{
	return this->gameState;
}



std::string lchessBoard::toChessCoords( const int index )
{
	int file = (index%8);
	int rank = (index/8)+1;
	char fileChar = file+'a';
	return fileChar+std::to_string(rank);
}



std::string lchessBoard::toChessCoords( const int x , const int y )
{
	int rank = y+1;
	char fileChar = x+'a';
	return fileChar+std::to_string(rank);
}



void lchessBoard::print() const
{
#ifndef LINUX
	//std::cout << "+----+----+----+----+----+----+----+----+" << std::endl;
 //   for ( int i = 0 ; i < 64 ; ++i )
 //   {
 //       if ( i%8 == 0 && i != 0 ) std::cout << "|" << std::endl << "+----+----+----+----+----+----+----+----+" << std::endl;
 //       std::cout << "|";
 //       this->printPiece( this->board[i] );
 //   }
 //   std::cout << "|" << std::endl << "+----+----+----+----+----+----+----+----+" << std::endl;
	std::cout << "+----+----+----+----+----+----+----+----+" << std::endl;
    for ( int y = 7 ; y >= 0 ; --y )
    {
		for ( int x = 0 ; x < 8 ; ++x )
		{
			std::cout << "|";
			this->printPiece( this->board[y*8+x] );
		}
		std::cout << "|" << std::endl << "+----+----+----+----+----+----+----+----+" << std::endl;
    }
#else
    std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
    for ( int i = 0 ; i < 64 ; ++i )
    {
        if ( i%8 == 0 && i != 0 ) std::cout << "|" << std::endl << "+---+---+---+---+---+---+---+---+" << std::endl;
        std::cout << "|";
        this->printPiece( this->board[i] );
    }
    std::cout << "|" << std::endl << "+---+---+---+---+---+---+---+---+" << std::endl;
#endif
}



void lchessBoard::allocateMemory()
{
	possibleMoves.resize( 256 );
}



/*
private functions
*/



void lchessBoard::printPiece( const BYTE pieceType ) const
{
#ifndef LINUX
    if ( pieceType == BLACK_ROOK ) std::cout << " bR ";
    else if ( pieceType == BLACK_KNIGHT ) std::cout << " bN ";
    else if ( pieceType == BLACK_BISHOP ) std::cout << " bB ";
    else if ( pieceType == BLACK_QUEEN ) std::cout << " bQ ";
    else if ( pieceType == BLACK_KING ) std::cout << " bK ";
    else if ( pieceType == BLACK_PAWN ) std::cout << " bP ";

    else if ( pieceType == WHITE_ROOK ) std::cout << " wR ";
    else if ( pieceType == WHITE_KNIGHT ) std::cout << " wN ";
    else if ( pieceType == WHITE_BISHOP ) std::cout << " wB ";
    else if ( pieceType == WHITE_QUEEN ) std::cout << " wQ ";
    else if ( pieceType == WHITE_KING ) std::cout << " wK ";
    else if ( pieceType == WHITE_PAWN ) std::cout << " wP ";

    else std::cout << "    ";
#else
    std::string blackcolor = "\033[1;30m";
    std::string whitecolor = "\033[1;37m";
    std::string endcolor = "\033[0m";
    bool unicode = false;

    if ( !unicode )
    {
        if ( pieceType == BLACK_ROOK ) std::cout << blackcolor << " R " << endcolor;
        else if ( pieceType == BLACK_KNIGHT ) std::cout << blackcolor << " N " << endcolor;
        else if ( pieceType == BLACK_BISHOP ) std::cout << blackcolor << " B " << endcolor;
        else if ( pieceType == BLACK_QUEEN ) std::cout << blackcolor << " Q " << endcolor;
        else if ( pieceType == BLACK_KING ) std::cout << blackcolor << " K " << endcolor;
        else if ( pieceType == BLACK_PAWN ) std::cout << blackcolor << " P " << endcolor;

        else if ( pieceType == WHITE_ROOK ) std::cout << whitecolor << " R " << endcolor;
        else if ( pieceType == WHITE_KNIGHT ) std::cout << whitecolor << " N " << endcolor;
        else if ( pieceType == WHITE_BISHOP ) std::cout << whitecolor << " B " << endcolor;
        else if ( pieceType == WHITE_QUEEN ) std::cout << whitecolor << " Q " << endcolor;
        else if ( pieceType == WHITE_KING ) std::cout << whitecolor << " K " << endcolor;
        else if ( pieceType == WHITE_PAWN ) std::cout << whitecolor << " P " << endcolor;

        else std::cout << " . ";
    }
    else
    {
        if ( pieceType == BLACK_ROOK ) std::cout << blackcolor << " \u265C " << endcolor;
        else if ( pieceType == BLACK_KNIGHT ) std::cout << blackcolor << " \u265E " << endcolor;
        else if ( pieceType == BLACK_BISHOP ) std::cout << blackcolor << " \u265D " << endcolor;
        else if ( pieceType == BLACK_QUEEN ) std::cout << blackcolor << " \u265B " << endcolor;
        else if ( pieceType == BLACK_KING ) std::cout << blackcolor << " \u265A " << endcolor;
        else if ( pieceType == BLACK_PAWN ) std::cout << blackcolor << " \u265F " << endcolor;

        else if ( pieceType == WHITE_ROOK ) std::cout << whitecolor << " \u2656 " << endcolor;
        else if ( pieceType == WHITE_KNIGHT ) std::cout << whitecolor << " \u2658 " << endcolor;
        else if ( pieceType == WHITE_BISHOP ) std::cout << whitecolor << " \u2657 " << endcolor;
        else if ( pieceType == WHITE_QUEEN ) std::cout << whitecolor << " \u2655 " << endcolor;
        else if ( pieceType == WHITE_KING ) std::cout << whitecolor << " \u2654 " << endcolor;
        else if ( pieceType == WHITE_PAWN ) std::cout << whitecolor << " \u2659 " << endcolor;

        else std::cout << "   ";
    }
#endif
}



/*
lchessMove
*/



void lchessMove::print() const
{
	switch ( this->piece )
	{
	case WHITE_PAWN: std::cout << "White Pawn: "; break;
	case WHITE_ROOK: std::cout << "White Rook: "; break;
	case WHITE_KNIGHT: std::cout << "White Knight: "; break;
	case WHITE_BISHOP: std::cout << "White Bishop: "; break;
	case WHITE_QUEEN: std::cout << "White Queen: "; break;
	case WHITE_KING: std::cout << "White King: "; break;
	case BLACK_PAWN: std::cout << "Black Pawn: "; break;
	case BLACK_ROOK: std::cout << "Black Rook: "; break;
	case BLACK_KNIGHT: std::cout << "Black Knight: "; break;
	case BLACK_BISHOP: std::cout << "Black Bishop: "; break;
	case BLACK_QUEEN: std::cout << "Black Queen: "; break;
	case BLACK_KING: std::cout << "Black King: "; break;
	}
	std::cout << lchessBoard::toChessCoords(from) << " -> " << lchessBoard::toChessCoords(to) << std::endl;
}
