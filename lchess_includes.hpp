#ifndef LCHESS_INCLUDES_HPP
#define LCHESS_INCLUDES_HPP

// Include necessary headers
#include <iostream>
#include <vector>
#include <string>

// Define macros or constants if needed
#define MAX_MOVES 100

// Define BYTE 

#ifndef BYTE
#define BYTE uint8_t
#endif

// Define lchessGameState which could be lchessGameState::BLACKWIN, lchessGameState::WHITEWIN, lchessGameState::DRAW, lchessGameState::ONGOING
enum lchessGameState
{
    BLACKWIN,
    WHITEWIN,
    DRAW,
    ONGOING
};

// Include Timer class
#include "Timer.hpp"




#endif // LCHESS_INCLUDES_HPP