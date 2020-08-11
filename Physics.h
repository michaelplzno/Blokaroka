#pragma once

#include "Blokaroka.h"

#define PIXELS_TO_PHYSICS 0.0151515f
#define PHYSICS_TO_PIXELS 66.0f

class Physics
{
public:



	class DebugDraw : public b2Draw
	{
	public:
		DebugDraw()
		{

		};

		~DebugDraw()
		{

		};

		void Create();
		void Destroy();

		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

		void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

		void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

		void DrawTransform(const b2Transform& xf) override;

		void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

		void DrawString(int x, int y, const char* string, ...);

		void DrawString(const b2Vec2& p, const char* string, ...);

		void DrawAABB(b2AABB* aabb, const b2Color& color);

		void Flush();
	};

	Physics() :
		m_world(NULL)
    {

    };

    void Init(); // Create the physics world.
    void Update(); // Step the physics world.
    void Draw(); // Draw the physics debug visuals.
	
	b2Body* GenerateBody();
	void DestroyBody(b2Body* body);
	
	Blok* OnMouseDown(b2Vec2 p);
	void OnMouseMove(b2Vec2 p);
	void OnMouseUp();
	void UpdateMouse();

private:
    b2World* m_world;
	b2MouseJoint* m_mouseJoint;
	DebugDraw* m_debugDraw;
	b2Body* m_groundBody;
	b2Vec2 m_mouseTarget;

};

extern Physics PHYSICS;