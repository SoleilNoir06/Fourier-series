#pragma once
#include <vector>
#include <string>

struct Point2D
{
    float x;
    float y;
};

std::vector<Point2D> GeneratePathFromFormulas(const std::string &formulaX, const std::string &formulaY, int numSamples);