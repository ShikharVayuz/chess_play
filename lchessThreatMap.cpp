/*
use at own risk
*/
#include "lchessThreatMap.hpp"
#include "lchessBoard.hpp"



lchessThreatMap::lchessThreatMap()
{
	this->white = 0;
	this->black = 0;
}



lchessThreatMap::~lchessThreatMap()
{

}



lchessThreatMap lchessThreatMap::fromBoard( const lchessBoard& board )
{
	lchessThreatMap threatMap;
	threatMap.init( board );
	return threatMap;
}



bool lchessThreatMap::isWhiteInCheck( const lchessBoard& board )
{
	for ( int i = 0 ; i < 64 ; ++i )
	{
		if ( board.isBlack( i ) )
		{
			BYTE piece = board.getPiece( i );
			int x = i%8;
			int y = i/8;
			if ( piece == BLACK_PAWN )
			{
				if ( y > 0 && x-1 >= 0 && board.getPiece(toIndex(x-1,y-1)) == WHITE_KING ) return true;
				if ( y > 0 && x+1 < 8 && board.getPiece(toIndex(x+1,y-1)) == WHITE_KING ) return true;
			}
			else if ( piece == BLACK_KNIGHT )
			{
				// up
				if ( x-1 >= 0 && y+2 < 8 && board.getPiece(toIndex(x-1,y+2)) == WHITE_KING ) return true;
				if ( x+1 < 8 && y+2 < 8 && board.getPiece(toIndex(x+1,y+2)) == WHITE_KING ) return true;

				// down
				if ( x-1 >= 0 && y-2 >= 0 && board.getPiece(toIndex(x-1,y-2)) == WHITE_KING ) return true;
				if ( x+1 < 8 && y-2 >= 0 && board.getPiece(toIndex(x+1,y-2)) == WHITE_KING ) return true;

				// left
				if ( x-2 >= 0 && y-1 >= 0 && board.getPiece(toIndex(x-2,y-1)) == WHITE_KING ) return true;
				if ( x-2 >= 0 && y+1 < 8 && board.getPiece(toIndex(x-2,y+1)) == WHITE_KING ) return true;

				// right
				if ( x+2 < 8 && y-1 >= 0 && board.getPiece(toIndex(x+2,y-1)) == WHITE_KING ) return true;
				if ( x+2 < 8 && y+1 < 8 && board.getPiece(toIndex(x+2,y+1)) == WHITE_KING ) return true;
			}
			else if ( piece == BLACK_ROOK || piece == BLACK_BISHOP || piece == BLACK_QUEEN )
			{
				// rook moves
				if ( piece == BLACK_ROOK || piece == BLACK_QUEEN )
				{
					// up
					for ( int r = y+1 ; r < 8 ; ++r )
					{
						if ( board.getPiece(r*8+x) == WHITE_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+x) ) break;
					}

					// down
					for ( int r = y-1 ; r >= 0 ; --r )
					{
						if ( board.getPiece(r*8+x) == WHITE_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+x) ) break;
					}

					// left
					for ( int f = x-1 ; f >= 0 ; --f )
					{
						if ( board.getPiece(y*8+f) == WHITE_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(y*8+f) ) break;
					}

					// right
					for ( int f = x+1 ; f < 8 ; ++f )
					{
						if ( board.getPiece(y*8+f) == WHITE_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(y*8+f) ) break;
					}
				}

				// bishop moves
				if ( piece == BLACK_BISHOP || piece == BLACK_QUEEN )
				{
					// down left
					for ( int r = y-1 , f = x-1 ; r >= 0 && f >= 0 ; --r , --f )
					{
						if ( board.getPiece(r*8+f) == WHITE_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// down right
					for ( int r = y-1 , f = x+1 ; r >= 0 && f < 8 ; --r , ++f )
					{
						if ( board.getPiece(r*8+f) == WHITE_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// up left
					for ( int r = y+1 , f = x-1 ; r < 8 && f >= 0 ; ++r , --f )
					{
						if ( board.getPiece(r*8+f) == WHITE_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// up right
					for ( int r = y+1 , f = x+1 ; r < 8 && f < 8 ; ++r , ++f )
					{
						if ( board.getPiece(r*8+f) == WHITE_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}
				}
			}
		}
	}
	return false;
}



bool lchessThreatMap::isBlackInCheck( const lchessBoard& board )
{
	for ( int i = 0 ; i < 64 ; ++i )
	{
		if ( board.isWhite( i ) )
		{
			BYTE piece = board.getPiece( i );
			int x = i%8;
			int y = i/8;
			if ( piece == WHITE_PAWN )
			{
				if ( y < 7 && x-1 >= 0 && board.getPiece(toIndex(x-1,y+1)) == BLACK_KING ) return true;
				if ( y < 7 && x+1 < 8 && board.getPiece(toIndex(x+1,y+1)) == BLACK_KING ) return true;
			}
			else if ( piece == WHITE_KNIGHT )
			{
				// up
				if ( x-1 >= 0 && y+2 < 8 && board.getPiece(toIndex(x-1,y+2)) == BLACK_KING ) return true;
				if ( x+1 < 8 && y+2 < 8 && board.getPiece(toIndex(x+1,y+2)) == BLACK_KING ) return true;

				// down
				if ( x-1 >= 0 && y-2 >= 0 && board.getPiece(toIndex(x-1,y-2)) == BLACK_KING ) return true;
				if ( x+1 < 8 && y-2 >= 0 && board.getPiece(toIndex(x+1,y-2)) == BLACK_KING ) return true;

				// left
				if ( x-2 >= 0 && y-1 >= 0 && board.getPiece(toIndex(x-2,y-1)) == BLACK_KING ) return true;
				if ( x-2 >= 0 && y+1 < 8 && board.getPiece(toIndex(x-2,y+1)) == BLACK_KING ) return true;

				// right
				if ( x+2 < 8 && y-1 >= 0 && board.getPiece(toIndex(x+2,y-1)) == BLACK_KING ) return true;
				if ( x+2 < 8 && y+1 < 8 && board.getPiece(toIndex(x+2,y+1)) == BLACK_KING ) return true;
			}
			else if ( piece == WHITE_ROOK || piece == WHITE_BISHOP || piece == WHITE_QUEEN )
			{
				// rook moves
				if ( piece == WHITE_ROOK || piece == WHITE_QUEEN )
				{
					// up
					for ( int r = y+1 ; r < 8 ; ++r )
					{
						if ( board.getPiece(r*8+x) == BLACK_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+x) ) break;
					}

					// down
					for ( int r = y-1 ; r >= 0 ; --r )
					{
						if ( board.getPiece(r*8+x) == BLACK_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+x) ) break;
					}

					// left
					for ( int f = x-1 ; f >= 0 ; --f )
					{
						if ( board.getPiece(y*8+f) == BLACK_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(y*8+f) ) break;
					}

					// right
					for ( int f = x+1 ; f < 8 ; ++f )
					{
						if ( board.getPiece(y*8+f) == BLACK_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(y*8+f) ) break;
					}
				}

				// bishop moves
				if ( piece == WHITE_BISHOP || piece == WHITE_QUEEN )
				{
					// down left
					for ( int r = y-1 , f = x-1 ; r >= 0 && f >= 0 ; --r , --f )
					{
						if ( board.getPiece(r*8+f) == BLACK_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// down right
					for ( int r = y-1 , f = x+1 ; r >= 0 && f < 8 ; --r , ++f )
					{
						if ( board.getPiece(r*8+f) == BLACK_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// up left
					for ( int r = y+1 , f = x-1 ; r < 8 && f >= 0 ; ++r , --f )
					{
						if ( board.getPiece(r*8+f) == BLACK_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// up right
					for ( int r = y+1 , f = x+1 ; r < 8 && f < 8 ; ++r , ++f )
					{
						if ( board.getPiece(r*8+f) == BLACK_KING ) return true;
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}
				}
			}
		}
	}
	return false;
}



bool lchessThreatMap::isWhiteThreat( const int index ) const
{
	return ( this->white >> int64_t(index) ) & int64_t(1);
}



bool lchessThreatMap::isBlackThreat( const int index ) const
{
	return ( this->black >> int64_t(index) ) & int64_t(1);
}



void lchessThreatMap::print() const
{
	std::cout << "White Threat Map:" << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	for ( int y = 7 ; y >= 0 ; --y )
	{
		for ( int x = 0 ; x < 8 ; ++x )
		{
			std::cout << "|";
			if ( ( this->white >> int64_t(y*8+x) ) & int64_t(1) ) std::cout << " X ";
			else std::cout << "   ";
		}
		std::cout << "|" << std::endl << "+---+---+---+---+---+---+---+---+" << std::endl;
	}
  //  for ( int i = 0 ; i < 64 ; ++i )
  //  {
  //      if ( i%8 == 0 && i != 0 ) std::cout << "|" << std::endl << "+---+---+---+---+---+---+---+---+" << std::endl;
  //      std::cout << "|";
		//if ( ( this->white >> int64_t(i) ) & int64_t(1) ) std::cout << " X ";
		//else std::cout << "   ";
  //  }
  //  std::cout << "|" << std::endl << "+---+---+---+---+---+---+---+---+" << std::endl;
}



/*
private functions
*/



void lchessThreatMap::init( const lchessBoard& board )
{
	for ( int i = 0 ; i < 64 ; ++i )
	{
		if ( !board.isEmpty( i ) )
		{
			BYTE piece = board.getPiece( i );
			int x = i%8;
			int y = i/8;
			if ( piece == WHITE_PAWN )
			{
				if ( y < 7 && x-1 >= 0 ) this->white |= ( int64_t(1) << int64_t(toIndex(x-1,y+1)) );
				if ( y < 7 && x+1 < 8 ) this->white |= ( int64_t(1) << int64_t(toIndex(x+1,y+1)) );
			}
			else if ( piece == WHITE_KNIGHT )
			{
				// up
				if ( x-1 >= 0 && y+2 < 8 ) this->white |= ( int64_t(1) << int64_t(toIndex(x-1,y+2)) );
				if ( x+1 < 8 && y+2 < 8 ) this->white |= ( int64_t(1) << int64_t(toIndex(x+1,y+2)) );

				// down
				if ( x-1 >= 0 && y-2 >= 0 ) this->white |= ( int64_t(1) << int64_t(toIndex(x-1,y-2)) );
				if ( x+1 < 8 && y-2 >= 0 ) this->white |= ( int64_t(1) << int64_t(toIndex(x+1,y-2)) );

				// left
				if ( x-2 >= 0 && y-1 >= 0 ) this->white |= ( int64_t(1) << int64_t(toIndex(x-2,y-1)) );
				if ( x-2 >= 0 && y+1 < 8 ) this->white |= ( int64_t(1) << int64_t(toIndex(x-2,y+1)) );

				// right
				if ( x+2 < 8 && y-1 >= 0 ) this->white |= ( int64_t(1) << int64_t(toIndex(x+2,y-1)) );
				if ( x+2 < 8 && y+1 < 8 ) this->white |= ( int64_t(1) << int64_t(toIndex(x+2,y+1)) );
			}
			else if ( piece == WHITE_KING )
			{
				// straight moves
				if ( y-1 >= 0 ) this->white |= ( int64_t(1) << int64_t(toIndex(x,y-1)) );
				if ( y+1 < 8 ) this->white |= ( int64_t(1) << int64_t(toIndex(x,y+1)) );
				if ( x-1 >= 0 ) this->white |= ( int64_t(1) << int64_t(toIndex(x-1,y)) );
				if ( x+1 < 8 ) this->white |= ( int64_t(1) << int64_t(toIndex(x+1,y)) );

				// diagonal moves
				if ( y-1 >= 0 && x-1 >= 0 ) this->white |= ( int64_t(1) << int64_t(toIndex(x-1,y-1)) );
				if ( y-1 >= 0 && x+1 < 8 ) this->white |= ( int64_t(1) << int64_t(toIndex(x+1,y-1)) );
				if ( y+1 < 8 && x-1 >= 0 ) this->white |= ( int64_t(1) << int64_t(toIndex(x-1,y+1)) );
				if ( y+1 < 8 && x+1 < 8 ) this->white |= ( int64_t(1) << int64_t(toIndex(x+1,y+1)) );
			}
			else if ( piece == WHITE_ROOK || piece == WHITE_BISHOP || piece == WHITE_QUEEN )
			{
				// rook moves
				if ( piece == WHITE_ROOK || piece == WHITE_QUEEN )
				{
					// up
					for ( int r = y+1 ; r < 8 ; ++r )
					{
						this->white |= ( int64_t(1) << int64_t(r*8+x) );
						// stop at any piece
						if ( !board.isEmpty(r*8+x) ) break;
					}

					// down
					for ( int r = y-1 ; r >= 0 ; --r )
					{
						this->white |= ( int64_t(1) << int64_t(r*8+x) );
						// stop at any piece
						if ( !board.isEmpty(r*8+x) ) break;
					}

					// left
					for ( int f = x-1 ; f >= 0 ; --f )
					{
						this->white |= ( int64_t(1) << int64_t(y*8+f) );
						// stop at any piece
						if ( !board.isEmpty(y*8+f) ) break;
					}

					// right
					for ( int f = x+1 ; f < 8 ; ++f )
					{
						this->white |= ( int64_t(1) << int64_t(y*8+f) );
						// stop at any piece
						if ( !board.isEmpty(y*8+f) ) break;
					}
				}

				// bishop moves
				if ( piece == WHITE_BISHOP || piece == WHITE_QUEEN )
				{
					// down left
					for ( int r = y-1 , f = x-1 ; r >= 0 && f >= 0 ; --r , --f )
					{
						this->white |= ( int64_t(1) << int64_t(r*8+f) );
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// down right
					for ( int r = y-1 , f = x+1 ; r >= 0 && f < 8 ; --r , ++f )
					{
						this->white |= ( int64_t(1) << int64_t(r*8+f) );
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// up left
					for ( int r = y+1 , f = x-1 ; r < 8 && f >= 0 ; ++r , --f )
					{
						this->white |= ( int64_t(1) << int64_t(r*8+f) );
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// up right
					for ( int r = y+1 , f = x+1 ; r < 8 && f < 8 ; ++r , ++f )
					{
						this->white |= ( int64_t(1) << int64_t(r*8+f) );
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}
				}
			}
			/*
			BLACK
			*/
			else if ( piece == BLACK_PAWN )
			{
				if ( y > 0 && x-1 >= 0 ) this->black |= ( int64_t(1) << int64_t(toIndex(x-1,y-1)) );
				if ( y > 0 && x+1 < 8 ) this->black |= ( int64_t(1) << int64_t(toIndex(x+1,y-1)) );
			}
			else if ( piece == BLACK_KNIGHT )
			{
				// up
				if ( x-1 >= 0 && y+2 < 8 ) this->black |= ( int64_t(1) << int64_t(toIndex(x-1,y+2)) );
				if ( x+1 < 8 && y+2 < 8 ) this->black |= ( int64_t(1) << int64_t(toIndex(x+1,y+2)) );

				// down
				if ( x-1 >= 0 && y-2 >= 0 ) this->black |= ( int64_t(1) << int64_t(toIndex(x-1,y-2)) );
				if ( x+1 < 8 && y-2 >= 0 ) this->black |= ( int64_t(1) << int64_t(toIndex(x+1,y-2)) );

				// left
				if ( x-2 >= 0 && y-1 >= 0 ) this->black |= ( int64_t(1) << int64_t(toIndex(x-2,y-1)) );
				if ( x-2 >= 0 && y+1 < 8 ) this->black |= ( int64_t(1) << int64_t(toIndex(x-2,y+1)) );

				// right
				if ( x+2 < 8 && y-1 >= 0 ) this->black |= ( int64_t(1) << int64_t(toIndex(x+2,y-1)) );
				if ( x+2 < 8 && y+1 < 8 ) this->black |= ( int64_t(1) << int64_t(toIndex(x+2,y+1)) );
			}
			else if ( piece == BLACK_KING )
			{
				// straight moves
				if ( y-1 >= 0 ) this->black |= ( int64_t(1) << int64_t(toIndex(x,y-1)) );
				if ( y+1 < 8 ) this->black |= ( int64_t(1) << int64_t(toIndex(x,y+1)) );
				if ( x-1 >= 0 ) this->black |= ( int64_t(1) << int64_t(toIndex(x-1,y)) );
				if ( x+1 < 8 ) this->black |= ( int64_t(1) << int64_t(toIndex(x+1,y)) );

				// diagonal moves
				if ( y-1 >= 0 && x-1 >= 0 ) this->black |= ( int64_t(1) << int64_t(toIndex(x-1,y-1)) );
				if ( y-1 >= 0 && x+1 < 8 ) this->black |= ( int64_t(1) << int64_t(toIndex(x+1,y-1)) );
				if ( y+1 < 8 && x-1 >= 0 ) this->black |= ( int64_t(1) << int64_t(toIndex(x-1,y+1)) );
				if ( y+1 < 8 && x+1 < 8 ) this->black |= ( int64_t(1) << int64_t(toIndex(x+1,y+1)) );
			}
			else if ( piece == BLACK_ROOK || piece == BLACK_BISHOP || piece == BLACK_QUEEN )
			{
				// rook moves
				if ( piece == BLACK_ROOK || piece == BLACK_QUEEN )
				{
					// up
					for ( int r = y+1 ; r < 8 ; ++r )
					{
						this->black |= ( int64_t(1) << int64_t(r*8+x) );
						// stop at any piece
						if ( !board.isEmpty(r*8+x) ) break;
					}

					// down
					for ( int r = y-1 ; r >= 0 ; --r )
					{
						this->black |= ( int64_t(1) << int64_t(r*8+x) );
						// stop at any piece
						if ( !board.isEmpty(r*8+x) ) break;
					}

					// left
					for ( int f = x-1 ; f >= 0 ; --f )
					{
						this->black |= ( int64_t(1) << int64_t(y*8+f) );
						// stop at any piece
						if ( !board.isEmpty(y*8+f) ) break;
					}

					// right
					for ( int f = x+1 ; f < 8 ; ++f )
					{
						this->black |= ( int64_t(1) << int64_t(y*8+f) );
						// stop at any piece
						if ( !board.isEmpty(y*8+f) ) break;
					}
				}

				// bishop moves
				if ( piece == BLACK_BISHOP || piece == BLACK_QUEEN )
				{
					// down left
					for ( int r = y-1 , f = x-1 ; r >= 0 && f >= 0 ; --r , --f )
					{
						this->black |= ( int64_t(1) << int64_t(r*8+f) );
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// down right
					for ( int r = y-1 , f = x+1 ; r >= 0 && f < 8 ; --r , ++f )
					{
						this->black |= ( int64_t(1) << int64_t(r*8+f) );
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// up left
					for ( int r = y+1 , f = x-1 ; r < 8 && f >= 0 ; ++r , --f )
					{
						this->black |= ( int64_t(1) << int64_t(r*8+f) );
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}

					// up right
					for ( int r = y+1 , f = x+1 ; r < 8 && f < 8 ; ++r , ++f )
					{
						this->black |= ( int64_t(1) << int64_t(r*8+f) );
						// stop at any piece
						if ( !board.isEmpty(r*8+f) ) break;
					}
				}
			}
		}
	}
}