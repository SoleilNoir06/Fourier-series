#include "Epicycle.hpp"
#include <cmath>

Epicycle::Epicycle(Vector2 center, float radius, float angle, float speed)
{
    _center = center;
    _radius = radius;
    _angle = angle;
    _speed = speed;
}

/// @brief Updates angle (vector) in time
/// @param deltaTime Delta time
void Epicycle::Update(float deltaTime)
{
    _angle += _speed * deltaTime;
}

/// @brief Compute X and Y coordinates of radius tip
/// @return Returns X and Y coordinates of radius tip
Vector2 Epicycle::GetTipPosition()
{
    // Compute tip position
    float TipPosX = _center.x + _radius * std::cos(_angle);
    float TipPosY = _center.y + _radius * std::sin(_angle);

    return {TipPosX, TipPosY};
}

/// @brief Draw circle and vector
void Epicycle::Draw()
{
    // Draw circle
    DrawCircleLines(_center.x, _center.y, _radius, RED);

    // Get tip position
    Vector2 tip = GetTipPosition();

    // Draw radius
    DrawLineEx(_center, tip, 2.0f, BLACK);

    // Draw small circle at the tip
    DrawCircleV(tip, 3.0f, BLUE);
}

void Epicycle::SetCenter(Vector2 newCenter)
{
    _center = newCenter;
}
