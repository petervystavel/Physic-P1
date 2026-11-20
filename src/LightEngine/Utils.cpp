#pragma once

#include "Utils.h"
#include "Rendering.h"
#include "Debug.h"

IntersectInfo Intersect(const Circle& c, const Segment& s)
{
    IntersectInfo info = { false };

    Vector2f AE = s.b - s.a;
    Vector2f direction = AE.Normalized();
    Vector2f AB = c.position - s.a;

    float distanceAC = Vector2f::Dot(AB, direction);
    Vector2f C = s.a + (distanceAC * direction);
    Vector2f BC = C - c.position;

    float radiusSqr = c.radius * c.radius;
    float lenghtSqrBC = BC.MagnitudeSqr();

    if (lenghtSqrBC > radiusSqr)
        return info;

    float decalage = sqrt(radiusSqr - lenghtSqrBC);
    float distance = distanceAC - decalage;
    float distanceSqr = distance * distance;

    if (distance < 0.f || distanceSqr > AE.MagnitudeSqr())
        return info;

    info.point = s.a + direction * distance;
    info.found = true;

    return info;
}
