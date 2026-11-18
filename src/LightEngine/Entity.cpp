#include "Entity.h"

#include "GameManager.h"
#include "Debug.h"

#include <cassert>
#include <SDL3/SDL_render.h>
#include <algorithm>

void Entity::Initialize(float radius, const Color& color)
{
    assert(radius > 0.f);

    mCircle.position = { 0.f, 0.f };
    mCircle.radius = radius;
    mCircle.color = color;
    mCircle.filled = true;

    OnInitialize();
}

void Entity::Destroy()
{
    if (mToDestroy)
        return;

    mToDestroy = true;

    OnDestroy();
}

void Entity::SetPosition(Vector2f position, float ratioX, float ratioY)
{
    float size = mCircle.radius * 2.f;

    position.x -= size * ratioX;
    position.y -= size * ratioY;

    mCircle.position = position;
}

Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
    float size = mCircle.radius * 2.f;
    Vector2f position = mCircle.position;

    position.x += size * ratioX;
    position.y += size * ratioY;

    return position;
}

void Entity::Update()
{
    float dt = GetDeltaTime();

    OnUpdate();
}

void Entity::FixedUpdate()
{
    float fixedDt = GameManager::Get()->GetFixedDeltaTime();
    Vector2f gravity = { 0.f, 981.f };
    mVelocity += gravity * fixedDt;
    mCircle.position += mVelocity * fixedDt;
}

void Entity::Draw(SDL_Renderer* renderer) const
{
    mCircle.Draw(renderer);
}

Scene* Entity::GetScene() const
{
    return GameManager::Get()->GetScene();
}

float Entity::GetDeltaTime() const
{
    return GameManager::Get()->GetDeltaTime();
}

void Entity::TryCollide(Entity* other)
{
    Vector2f CenterA = GetPosition();
    Vector2f CenterB = other->GetPosition();

    Vector2f AB = CenterB - CenterA;

    float Overlap =  GetRadius() + other->GetRadius() - AB.Magnitude();

    if (Overlap < 0)
        return;

    Vector2f Normal = AB.Normalized();

    Vector2f Translation = Normal * Overlap * 0.5f;

    mCircle.position -= Translation;
    other->mCircle.position += Translation;

    Vector2f vnA = Normal * Vector2f::Dot(mVelocity, Normal);
    Vector2f vtA = mVelocity - vnA;

    Vector2f vnB = Normal * Vector2f::Dot(other->mVelocity, Normal);
    Vector2f vtB = other->mVelocity - vnB;

    mVelocity = vnB + vtA;
    other->mVelocity = vnA + vtB;

    OnCollision(other);
    other->OnCollision(this);
}

void Entity::TryCollide(const Rectangle& rect)
{
    //Detect
    Vector2f center = GetPosition();
    float radius = GetRadius();

    Vector2f rectPos = rect.position;

    Vector2f min = rectPos;
    Vector2f max = min + rect.size;
    float overlap = 0;
    Vector2f normal = {};
    //in rectangle
    if (center.x >= min.x && center.y >= min.y && center.x <= max.x && center.y <= max.y)
    {
        float left = center.x - min.x;
        float right = max.x - center.x;
        float top = center.y - min.y;
        float bottom = max.y - center.y;

        float distances[] =
        {
            left,
            right,
            top,
            bottom
        };

        Vector2f normales[] =
        {
            {1, 0},
            {-1, 0},
            {0, 1},
            {0, -1},
        };

        int minIndex = 0;
        for (int i = 1; i < 4; ++i)
        {
            if (distances[i] < distances[minIndex])
            {
                minIndex = i;
            }
        }

        overlap = distances[minIndex] + GetRadius();
        normal = normales[minIndex];
    }
    else
    {
        float closestX = std::clamp(center.x, min.x, max.x);
        float closestY = std::clamp(center.y, min.y, max.y);

        Vector2f closestPoint = { closestX,closestY };

        Vector2f AB = closestPoint - center;
        float dist = AB.Magnitude();
        normal = AB.Normalized();
        overlap = radius - dist;

        if (overlap < 0)
            return;
    }

    //Correct
    Vector2f overlapCorrection = -1 * normal * overlap;
    mCircle.position += overlapCorrection;

    //Bounce
    float dot = Vector2f::Dot(mVelocity, normal);
    Vector2f vn = dot * normal;
    Vector2f vt = mVelocity - vn;

    mVelocity = -1 * vn + vt;
}

void Entity::TryCollide(const Line& line)
{
    //Detect
    Vector2f AB = line.point - GetPosition();
    Vector2f normal = line.normal;

    float distance = Vector2f::Dot(AB, normal);
    float overlap = GetRadius() - distance;
    if (overlap < 0)
        return;

    //Correction
    Vector2f overlapCorrection = -1 * normal * overlap;
    mCircle.position += overlapCorrection;

    //Bounce
    float dot = Vector2f::Dot(mVelocity, normal);
    Vector2f vn = dot * normal;
    Vector2f vt = mVelocity - vn;

    mVelocity = -1 * vn + vt;
}
