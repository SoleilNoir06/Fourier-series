#pragma once

#include <raylib.h>

class Epicycle
{

private:
    float _radius;
    float _angle;
    float _velocity;
    Vector2 _position;

public:
    Epicycle(Vector2 position, float radius, float angle, float velocity);

    Vector2 GetPosition();

    float GetRadius();

    float GetAngle();

    void Update(float time);
};