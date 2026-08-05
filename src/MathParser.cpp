#include "MathParser.hpp"
#include "exprtk.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

const double MATH_PI = 3.14159265358979323846;

std::vector<Point2D> GeneratePathFromFormulas(const std::string &formulaX, const std::string &formulaY, int numSamples)
{
    std::vector<Point2D> path;
    double t_var = 0.0;

    exprtk::symbol_table<double> symbolTable;
    symbolTable.add_variable("t", t_var);
    symbolTable.add_constants();

    exprtk::expression<double> expressionX;
    exprtk::expression<double> expressionY;
    expressionX.register_symbol_table(symbolTable);
    expressionY.register_symbol_table(symbolTable);

    exprtk::parser<double> parser;

    if (parser.compile(formulaX, expressionX) && parser.compile(formulaY, expressionY))
    {
        for (int i = 0; i < numSamples; i++)
        {
            t_var = (double)i / (double)numSamples * 2.0 * MATH_PI;

            double resultX = expressionX.value();
            double resultY = expressionY.value();

            path.push_back({(float)resultX, (float)resultY});
        }
    }
    else
    {
        std::cerr << "Invalid syntax in formula" << std::endl;
    }

    //Automatically scale the path
    if (!path.empty())
    {
        // Searching for the limits of the drawing
        float minX = path[0].x;
        float maxX = path[0].x;
        float minY = path[0].y;
        float maxY = path[0].y;

        for (size_t i = 0; i < path.size(); i++)
        {
            if (path[i].x < minX)
                minX = path[i].x;
            if (path[i].x > maxX)
                maxX = path[i].x;
            if (path[i].y < minY)
                minY = path[i].y;
            if (path[i].y > maxY)
                maxY = path[i].y;
        }

        // Computing biggest dimension (width and height)
        float width = std::abs(maxX - minX);
        float height = std::abs(maxY - minY);
        float maxAmplitude = std::max(width, height);

        if (maxAmplitude > 0.001f){
            float targetSize = 800.0f;
            float autoZoom = targetSize / maxAmplitude;

            for (size_t i = 0; i < path.size(); i++)
            {
                path[i].x *= autoZoom;
                path[i].y *= autoZoom;
            }
        }

    }

    return path;
}

std::vector<EpicycleData> ComputeDFT(const std::vector<Point2D> &points)
{
    std::vector<EpicycleData> results;
    int N = (int)points.size();

    for (int k = 0; k < N; k++)
    {
        float re = 0.0f;
        float im = 0.0f;

        for (int n = 0; n < N; n++)
        {
            float phi = (2.0f * (float)MATH_PI * (float)k * (float)n) / (float)N;
            re += points[n].x * std::cos(phi) + points[n].y * std::sin(phi);
            im += points[n].y * std::cos(phi) - points[n].x * std::sin(phi);
        }

        float radius = std::sqrt(re * re + im * im) / (float)N;
        float angle = std::atan2(im, re);

        // Adjusting frequency
        int freq = k;
        if (k > N / 2)
        {
            freq = k - N;
        }
        float speed = (float)freq;

        results.push_back({radius, angle, speed});
    }

    std::sort(results.begin(), results.end(), [](const EpicycleData &a, const EpicycleData &b)
              { return a.radius > b.radius; });

    return results;
}