#include "Epicycle.hpp"

Epicycle::Epicycle(Vector2 position, float radius, float angle, float velocity)
{
    _position = position;
    _radius = radius;
    _angle = angle;
    _velocity = velocity;
}

void Epicycle::Update(float time)
{
}

Vector2 Epicycle::GetPosition()
{
    return _position;
}

float Epicycle::GetRadius()
{
    return _radius;
}

float Epicycle::GetAngle()
{
    return _angle;
}