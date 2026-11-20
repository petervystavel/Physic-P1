#include "SampleScene.h"

#include "DummyEntity.h"
#include "InputManager.h"
#include "Debug.h"

void SampleScene::OnInitialize()
{
    mpEntity1 = CreateEntity<DummyEntity>(40.f, Color::Cyan);
    mpEntity1->SetPosition({ 300.f, 360.f });
    mpEntity1->SetMass(1);

    mpEntity2 = CreateEntity<DummyEntity>(40.f, Color::Red);
    mpEntity2->SetPosition({ 600.f, 360.f });
    mpEntity2->SetMass(1);

    //mpEntity2 = CreateEntity<DummyEntity>(40.f*2, Color::Cyan);
    //mpEntity2->SetPosition({ 600.f, 360.f });
    //mpEntity2->SetMass(10);

    //GameManager::Get()->AddLine({ 0, 0 }, { 0, -1 }); //TOP
    //GameManager::Get()->AddLine({ 0, 0 }, { -1, 0 }); //LEFT
    //GameManager::Get()->AddLine({ 0, GetWindowHeight()}, {0, 1}); //BOTTOM
    //GameManager::Get()->AddLine({ GetWindowWidth(), 0}, {1, 0}); //RIGHT

}

void SampleScene::OnUpdate()
{
    //if (InputManager::Get()->IsMouseDown(SDL_BUTTON_LEFT))
    //{
    //    Vector2f mousePos = InputManager::Get()->GetMousePosition();
    //    Vector2f pos = mpEntity1->GetPosition();

    //    mpEntity1->SetVelocity(mousePos - pos);

    //    //Do something
    //}

    Vector2f mousePos = InputManager::Get()->GetMousePosition();
    mpEntity1->SetVelocity(10*(mousePos - mpEntity1->GetPosition()));

    Segment s;
    s.a = { 300, 200 };
    s.b = { 500, 400 };

    Debug::DrawLine(s.a, s.b, Color::White);

    Circle c;
    c.position = mpEntity1->GetPosition();
    c.radius = mpEntity1->GetRadius();

    IntersectInfo info = Intersect(c, s);

    if (info.found)
        Debug::DrawCircle(info.point, 10.f, Color::Red, true);
}
