#pragma once
#include <vector>
#include <string>

struct Point2D
{
    float x;
    float y;
};

struct EpicycleData
{
    float radius;
    float angle;
    float speed;
};

std::vector<Point2D> GeneratePathFromFormulas(const std::string &formulaX, const std::string &formulaY, int numSamples);
std::vector<EpicycleData> ComputeDFT(const std::vector<Point2D> &points);