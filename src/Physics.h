#pragma once

#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Blokaroka.h"
#include <box2d/box2d.h>

// Define the comparison operator for b2BodyId
inline bool operator<(const b2BodyId &lhs, const b2BodyId &rhs)
{
    return lhs.index1 < rhs.index1;
}

#define PIXELS_TO_PHYSICS 0.0151515f
#define PHYSICS_TO_PIXELS 66.0f

class Physics
{
  public:
    Physics()
        : m_worldId(), m_mouseJoint(), m_debugDraw(), m_groundBody(), m_mouseTarget(), m_gravity(),
          m_bDebugRenderOn(false)
    {
        m_callbackBody = b2BodyId();
    };

    void Init();   // Create the physics world.
    void Update(); // Step the physics world.
    void Draw();   // Draw the physics debug visuals.

    b2BodyId GenerateBody();
    void DestroyBody(b2BodyId body);

    Blok *OnMouseDown(b2Vec2 p);
    void OnMouseMove(b2Vec2 p);
    void OnMouseUp();
    void UpdateMouse();

    void ChangeGravity(float yG);
    void ToggleGravity();
    bool IsGravityOn();

    void ToggleDebugRender();
    bool IsDebugRenderOn();

    // Debug Draw functions pased to m_debugDraw in Init()
    static void DrawTransformedPolygon(const b2Vec2 *vertices, int vertexCount, b2HexColor color, void *context,
                                       b2Transform t = b2Transform_identity);
    static void DrawPolygon(const b2Vec2 *vertices, int vertexCount, b2HexColor color, void *context);
    static void DrawSolidPolygon(b2Transform transform, const b2Vec2 *vertices, int vertexCount, float radius,
                                 b2HexColor color, void *context);
    static void DrawCircle(b2Vec2 center, float radius, b2HexColor color, void *context);
    static void DrawSolidCircle(b2Transform transform, float radius, b2HexColor color, void *context);
    static void DrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void *context);
    static void DrawPoint(const b2Vec2 &p, float size, const b2HexColor &color);

    static bool MouseTestCallback(b2ShapeId shape, void *context);

  private:
    b2WorldId m_worldId;
    b2JointId m_mouseJoint;
    b2DebugDraw m_debugDraw;
    b2BodyId m_groundBody;
    b2Vec2 m_mouseTarget;
    b2Vec2 m_gravity;

    bool m_bDebugRenderOn;

    static b2BodyId m_callbackBody;
};

extern Physics PHYSICS;

#endif // __PHYSICS_H__