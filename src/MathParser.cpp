#include "MathParser.hpp"
#include "exprtk.hpp"
#include <cmath>
#include <iostream>

const double PI = 3.14159265358979323846;

std::vector<Point2D> GeneratePathFromFormulas(const std::string &formulaX, const std::string &formulaY, int numSamples)
{
    std::vector<Point2D> path;

    // Var that will evolve in time
    double t_var = 0.0;

    // Symbol table configuration
    exprtk::symbol_table<double> symbolTable;
    symbolTable.add_variable("t", t_var);
    symbolTable.add_constants();

    // String formula configuration
    exprtk::expression<double> expressionX;
    exprtk::expression<double> expressionY;
    expressionX.register_symbol_table(symbolTable);
    expressionY.register_symbol_table(symbolTable);

    // String compilation X
    exprtk::parser<double> parser;

    if (parser.compile(formulaX, expressionX) && parser.compile(formulaY, expressionY))
    {
        for (int i = 0; i < numSamples; i++)
        {
            t_var = (double)i / numSamples * 2.0 * PI;

            double resultX = expressionX.value();
            double resultY = expressionY.value();

            // Adding point to path
            path.push_back({(float)resultX, (float)resultY});
        }
    }
    else
        std::cerr << "Invalide syntax in formula" << std::endl;

    // Return path
    return path;
}