#pragma once

#include <raylib.h>

class Epicycle
{

private:
    Vector2 _center; // Center of circle
    float _radius;   // Radius of circle, length of vector
    float _angle;    // Current angle in radians
    float _speed;    // Rotation speed (frequency)

public:
    /// @brief Constructor
    /// @param center Center of circle
    /// @param radius Radius of circle, length of vector
    /// @param angle Current angle in radians
    /// @param speed Rotation speed (frequency)
    Epicycle(Vector2 center, float radius, float angle, float speed);

    /// @brief Updates angle (vector) in time
    /// @param deltaTime Delta time
    void Update(float deltaTime);

    /// @brief Compute X and Y coordinates of radius tip
    /// @return Returns X and Y coordinates of radius tip
    Vector2 GetTipPosition();

    /// @brief Draw circle and vector
    void Draw(bool showCircles, bool darkMode);

    void SetCenter(Vector2 newCenter);

    float GetRadius() const;
};