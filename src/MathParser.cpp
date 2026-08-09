#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
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

    // Automatically scale the path
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

        if (maxAmplitude > 0.001f)
        {
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

        if (radius > 0.1f)
            results.push_back({radius, angle, speed});
    }

    std::sort(results.begin(), results.end(), [](const EpicycleData &a, const EpicycleData &b)
              { return a.radius > b.radius; });

    return results;
}

std::vector<Point2D> GeneratePathFromSVG(const char *filename)
{
    std::vector<Point2D> path;

    // NanoSVG reads the file
    // 96.0f = DPI (standard resolution)
    NSVGimage *image = nsvgParseFromFile(filename, "px", 96.0f);

    if (image == nullptr)
    {
        std::cerr << "Impossible to open SVG file !" << std::endl;
        return path;
    }

    // Going through every shape of image
    for (NSVGshape *shape = image->shapes; shape != nullptr; shape = shape->next)
    {
        // Running trough every paths of the shape
        for (NSVGpath *p = shape->paths; p != nullptr; p = p->next)
        {
            // Calculating every point
            for (int i = 0; i < p->npts - 1; i += 3)
            {
                // Creating pointers tothe 4 control points of the curve
                // X and Y in succession so multiplying index by 2
                float *p0 = &p->pts[(i + 0) * 2]; // Starting point
                float *p1 = &p->pts[(i + 1) * 2]; // Control point 1
                float *p2 = &p->pts[(i + 2) * 2]; // Control point 2
                float *p3 = &p->pts[(i + 3) * 2]; // Ending point

                // Choosing how many points we want extract from this curve
                int samplesPerCurve = 20;

                // Loop to sample
                for (int j = 0; j < samplesPerCurve; j++)
                {
                    // Calculating t cursor which goes from 0.0 to 1.0
                    float t = (float)j / (float)samplesPerCurve;

                    // Pre calculating powers
                    float u = 1.0f - t;
                    float uu = u * u;
                    float uuu = uu * u;
                    float tt = t * t;
                    float ttt = tt * t;

                    float pointX = uuu * p0[0] + 3 * t * uu * p1[0] + 3 * tt * u * p2[0] + ttt * p3[0];
                    float pointY = uuu * p0[1] + 3 * t * uu * p1[1] + 3 * tt * u * p2[1] + ttt * p3[1];

                    // Filling points vector
                    path.push_back({pointX, pointY});
                }
            }
        }
    }

    nsvgDelete(image);

    return path;
}