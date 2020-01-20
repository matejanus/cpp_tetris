#pragma once
#include <array>
#include <iostream>

class Tetermino{

public:
Tetermino();
~Tetermino() = default;
Tetermino(Tetermino& t) = delete;
std::string getData();

private:
const std::array<std::string, 7> pieceDefinition;

};
