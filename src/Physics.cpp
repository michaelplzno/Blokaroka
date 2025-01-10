#include "Physics.h"

Physics PHYSICS;
b2BodyId Physics::m_callbackBody;

void Physics::DrawTransformedPolygon(const b2Vec2 *vertices, int vertexCount,
                                     b2HexColor color, void *context,
                                     b2Transform t)
{
    for (int i = 0; i < vertexCount; i++)
    {
        Renderer::Pixel start(b2TransformPoint(t, vertices[i]));
        Renderer::Pixel end(
            b2TransformPoint(t, vertices[(i + 1) % vertexCount]));

        RENDER.Line(start, end, color, -10000, 1);
    }
    // OutputDebugString("DrawPolygon\n");
}

void Physics::DrawPolygon(const b2Vec2 *vertices, int vertexCount,
                          b2HexColor color, void *context)
{
    for (int i = 0; i < vertexCount; i++)
    {
        Renderer::Pixel start(vertices[i]);
        Renderer::Pixel end(vertices[(i + 1) % vertexCount]);

        RENDER.Line(start, end, color, 0, 1);
    }
    // OutputDebugString("DrawPolygon\n");
}

void Physics::DrawSolidPolygon(b2Transform transform, const b2Vec2 *vertices,
                               int vertexCount, float radius, b2HexColor color,
                               void *context)
{
    DrawTransformedPolygon(vertices, vertexCount, color, context, transform);

    // OutputDebugString("DrawPolygon2\n");
}

void Physics::DrawCircle(b2Vec2 center, float radius, b2HexColor color,
                         void *context)
{
    Renderer::Pixel pix(center);
    int r = (int)(radius * PHYSICS_TO_PIXELS);

    RENDER.Circle(pix.m_iX, pix.m_iY, r, color, 0, 1);
}

void Physics::DrawSolidCircle(b2Transform transform, float radius,
                              b2HexColor color, void *context)
{
    Renderer::Pixel pix(transform.p);
    int r = (int)(radius * PHYSICS_TO_PIXELS);

    RENDER.Circle(pix.m_iX, pix.m_iY, r, color, 0, 1);
}

void Physics::DrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void *context)
{

    Renderer::Pixel start(p1);
    Renderer::Pixel end(p2);
    RENDER.Line(start, end, color, 0, 1);
}

void Physics::DrawPoint(const b2Vec2 &p, float size, const b2HexColor &color)
{
    MessageBoxW(NULL, L"DrawPoint is called", L"Info", MB_OK);

    Renderer::Pixel pix(p);
    int s = (int)(size); // *PHYSICS_TO_PIXELS);

    RENDER.Rectangle(pix.m_iX - s, pix.m_iY - s, pix.m_iX + s, pix.m_iY + s,
                     color, 0, 1);
}

void Physics::Init()
{
    b2Vec2 gravity = {0.0f, 0.0f};
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = gravity;

    m_worldId = b2CreateWorld(&worldDef);

    m_debugDraw = b2DefaultDebugDraw();
    m_debugDraw.DrawCircle = DrawCircle;
    m_debugDraw.DrawSolidCircle = DrawSolidCircle;
    m_debugDraw.DrawPolygon = DrawPolygon;
    m_debugDraw.DrawSolidPolygon = DrawSolidPolygon;
    m_debugDraw.DrawSegment = DrawSegment;

    // Bottom Edge. Move this edge up by the number of pixels the start menu is.
    {
        // Get the height of the taskbar
        RECT taskbarRect;
        HWND taskbar = FindWindowW(L"Shell_TrayWnd", NULL);
        int taskbarHeight = 0;
        if (taskbar && GetWindowRect(taskbar, &taskbarRect))
        {
            taskbarHeight = taskbarRect.bottom - taskbarRect.top;
        }
        float taskbarHeightPhysics = taskbarHeight * PIXELS_TO_PHYSICS;

        b2BodyDef bd = b2DefaultBodyDef();
        bd.type = b2_staticBody;

        b2Polygon shape =
            b2MakeBox((RENDER.GetWidth() * .55f) * PIXELS_TO_PHYSICS, 2);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.friction = 0.3f;

        // Set the ground body for the mouse joint.
        b2BodyId bodyId = m_groundBody = b2CreateBody(m_worldId, &bd);
        b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &shape);
        b2Vec2 pos = {RENDER.GetWidth() * .5f * PIXELS_TO_PHYSICS,
                      -2 + taskbarHeightPhysics};
        b2Body_SetTransform(bodyId, pos, b2Rot_identity);
    }

    // Top Edge.
    {
        b2BodyDef bd = b2DefaultBodyDef();
        bd.type = b2_staticBody;

        b2Polygon shape =
            b2MakeBox((RENDER.GetWidth() * .55f) * PIXELS_TO_PHYSICS, 2);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.friction = 0.3f;

        b2BodyId bodyId = b2CreateBody(m_worldId, &bd);
        b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &shape);
        b2Vec2 pos = {RENDER.GetWidth() * .5f * PIXELS_TO_PHYSICS,
                      RENDER.GetHeight() * PIXELS_TO_PHYSICS + 2};
        b2Body_SetTransform(bodyId, pos, b2Rot_identity);
    }

    // Left Edge.
    {
        b2BodyDef bd = b2DefaultBodyDef();
        bd.type = b2_staticBody;

        b2Polygon shape =
            b2MakeBox(2, (RENDER.GetHeight() * .55f) * PIXELS_TO_PHYSICS);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.friction = 0.3f;

        b2BodyId bodyId = b2CreateBody(m_worldId, &bd);
        b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &shape);
        b2Vec2 pos = {-2, RENDER.GetHeight() * .5f * PIXELS_TO_PHYSICS};
        b2Body_SetTransform(bodyId, pos, b2Rot_identity);
    }

    // Right Edge.
    {
        b2BodyDef bd = b2DefaultBodyDef();
        bd.type = b2_staticBody;

        b2Polygon shape =
            b2MakeBox(2, (RENDER.GetHeight() * .55f) * PIXELS_TO_PHYSICS);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.friction = 0.3f;

        b2BodyId bodyId = b2CreateBody(m_worldId, &bd);
        b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &shape);
        b2Vec2 pos = {RENDER.GetWidth() * PIXELS_TO_PHYSICS + 2,
                      RENDER.GetHeight() * .5f * PIXELS_TO_PHYSICS};
        b2Body_SetTransform(bodyId, pos, b2Rot_identity);
    }
}

void Physics::Update()
{
    b2World_EnableSleeping(m_worldId, true);
    b2World_EnableWarmStarting(m_worldId, true);
    b2World_EnableContinuous(m_worldId, true);
    float cappedT = g_fDeltaT;
    if (cappedT > 1.0f / 30.0f)
    {
        cappedT = 1.0f / 30.0f;
    }
    b2World_Step(m_worldId, cappedT, 4);
}

void Physics::Draw()
{
    if (m_worldId.index1 != 0 && m_bDebugRenderOn)
    {
        m_debugDraw.drawShapes = true;
        m_debugDraw.drawMass = true;

        b2World_Draw(m_worldId, &m_debugDraw);
    }
}

b2BodyId Physics::GenerateBody()
{
    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = b2_dynamicBody;
    bd.fixedRotation = true;
    // bd.position.Set(blok->GetX() * PIXELS_TO_PHYSICS, (RENDER.GetHeight() -
    // blok->GetY()) * PIXELS_TO_PHYSICS);

    b2BodyId body = b2CreateBody(m_worldId, &bd);

    return body;
}

void Physics::DestroyBody(b2BodyId body)
{
    b2DestroyBody(body);
}

bool Physics::MouseTestCallback(b2ShapeId shape, void *context)
{
    m_callbackBody = b2Shape_GetBody(shape);
    return true;
}

Blok *Physics::OnMouseDown(b2Vec2 p)
{
    if (m_mouseJoint.index1 != 0)
    {
        return NULL;
    }

    b2Transform xf = b2Transform_identity;
    b2QueryFilter qf = b2DefaultQueryFilter();
    qf.categoryBits = 0x0001;
    qf.maskBits = 0xFFFF;

    m_callbackBody.index1 = 0;
    // Query the world for shapes that touch this point.
    b2World_OverlapPoint(m_worldId, p, xf, qf, MouseTestCallback, NULL);

    if (m_callbackBody.index1 != 0 && b2Body_GetUserData(m_callbackBody))
    {

        b2MouseJointDef md = b2DefaultMouseJointDef();
        md.bodyIdA = m_groundBody;
        md.bodyIdB = m_callbackBody;
        md.target = p;
        md.maxForce = 1000.0f * b2Body_GetMass(m_callbackBody);
        md.dampingRatio = 0.7f;
        md.hertz = 5.0f;
        m_mouseJoint = b2CreateMouseJoint(m_worldId, &md);
        b2Joint_SetCollideConnected(m_mouseJoint, true);
        b2Body_SetAwake(m_callbackBody, true);

        return (Blok *)b2Body_GetUserData(m_callbackBody);
    }

    return NULL;
}

void Physics::UpdateMouse()
{
    if (m_mouseJoint.index1 != 0)
    {
        b2Vec2 target = m_mouseTarget;
        b2MouseJoint_SetTarget(m_mouseJoint, target);
    }
}
void Physics::OnMouseMove(b2Vec2 p)
{
    m_mouseTarget = p;
}

void Physics::OnMouseUp()
{
    if (m_mouseJoint.index1 != 0)
    {
        b2DestroyJoint(m_mouseJoint);
        m_mouseJoint.index1 = 0;
    }
}

void Physics::ChangeGravity(float y_G)
{
    m_gravity = {0.00f, y_G};
    b2World_SetGravity(m_worldId, m_gravity);
}

void Physics::ToggleGravity()
{
    if (m_gravity.y == 0.0f)
    {
        m_gravity.y = -10.0f;
    }
    else
    {
        m_gravity.y = 0.0f;
    }
    b2World_SetGravity(m_worldId, m_gravity);
}

bool Physics::IsGravityOn()
{
    return m_gravity.y != 0.0f;
}

void Physics::SetDebugRender(bool bDebugRenderOn)
{
    m_bDebugRenderOn = bDebugRenderOn;
}

void Physics::ToggleDebugRender()
{
    m_bDebugRenderOn = !m_bDebugRenderOn;
}

bool Physics::IsDebugRenderOn()
{
    return m_bDebugRenderOn;
}