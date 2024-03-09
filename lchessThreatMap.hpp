/*
use at own risk
*/
#pragma once

#include "lchess_includes.hpp"

class lchessBoard;



class lchessThreatMap
{
public:
	lchessThreatMap();
	virtual ~lchessThreatMap();

	static lchessThreatMap fromBoard( const lchessBoard& board );

	// a greedy check whether white is in check, it will return as soon as a check is found
	static bool isWhiteInCheck( const lchessBoard& board );
	// a greedy check whether black is in check, it will return as soon as a check is found
	static bool isBlackInCheck( const lchessBoard& board );

	bool isWhiteThreat( const int index ) const;
	bool isBlackThreat( const int index ) const;

	void print() const;

private:
	int64_t white;
	int64_t black;

	void init( const lchessBoard& board );

	static inline int toIndex( const int x , const int y ) { return y*8+x; }
};