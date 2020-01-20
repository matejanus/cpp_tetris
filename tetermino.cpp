#include "includes/tetermino.hpp"

Tetermino::Tetermino() : pieceDefinition{"..X."
                                         "..X."
                                         "..X."
                                         "..X.",

                                         "..X."
                                         ".XX."
                                         ".X.."
                                         "....",

                                         ".X.."
                                         ".XX."
                                         "..X."
                                         "....",

                                         "...."
                                         ".XX."
                                         ".XX."
                                         "....",

                                         "..X."
                                         ".XX."
                                         "..X."
                                         "....",

                                         "...."
                                         ".XX."
                                         "..X."
                                         "..X.",

                                         "...."
                                         "..XX"
                                         "..X."
                                         "..X."}
{
    std::cout<<"tetermino";
}

std::string Tetermino::getData()
{
    return std::string("data");
}