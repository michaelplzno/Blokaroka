#include "Physics.h"

Physics PHYSICS;

//
void Physics::DebugDraw::Create()
{

}

//
void Physics::DebugDraw::Destroy()
{

}

//
void Physics::DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	for (int i = 0; i < vertexCount; i++)
	{
		Renderer::Pixel start(vertices[i]);
		Renderer::Pixel end(vertices[(i + 1) % vertexCount]);
		RENDER.Line(start, end, RGB(color.r * 255, color.g * 255, color.b * 255), 0, color.a);
	}
	//OutputDebugString("DrawPolygon\n");
}

//
void Physics::DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	DrawPolygon(vertices, vertexCount, color);

	//OutputDebugString("DrawPolygon2\n");
}

//
void Physics::DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	Renderer::Pixel pix(center);
	int r = (int)(radius * PHYSICS_TO_PIXELS);

	RENDER.Circle(pix.m_iX, pix.m_iY, r, RGB(color.r * 255, color.g * 255, color.b * 255), 0, color.a);
}

//
void Physics::DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	Renderer::Pixel pix(center);
	int r = (int)(radius * PHYSICS_TO_PIXELS);

	RENDER.Circle(pix.m_iX, pix.m_iY, r, RGB(color.r * 255, color.g * 255, color.b * 255), 0, color.a);
}

//
void Physics::DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	Renderer::Pixel start(p1);
	Renderer::Pixel end(p2);
	RENDER.Line(start, end, RGB(color.r * 255, color.g * 255, color.b * 255), 0, color.a);
}

//
void Physics::DebugDraw::DrawTransform(const b2Transform& xf)
{

}

//
void Physics::DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{

	Renderer::Pixel pix(p);
	int s = (int)(size);// *PHYSICS_TO_PIXELS);

	RENDER.Rectangle(pix.m_iX - s, pix.m_iY - s, pix.m_iX + s, pix.m_iY + s, RGB(color.r * 255, color.g * 255, color.b * 255), 0, color.a);
}

//
void Physics::DebugDraw::DrawString(int x, int y, const char* string, ...)
{

}

//
void Physics::DebugDraw::DrawString(const b2Vec2& pw, const char* string, ...)
{

}

//
void Physics::DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	//OutputDebugString("DrawAABB\n");
}

void Physics::Init()
{	

	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = new b2World(gravity);

	m_debugDraw = new DebugDraw();
	m_debugDraw->Create();
	m_world->SetDebugDraw(m_debugDraw);

	{
		b2BodyDef bd;
		m_groundBody = m_world->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(20, 2);// (b2Vec2(200.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		m_groundBody->CreateFixture(&shape, 0.0f);
		Renderer::Pixel pix(RENDER.GetWidth() / 2, RENDER.GetHeight() + 2 * PHYSICS_TO_PIXELS);
		m_groundBody->SetTransform(pix.ToPhysics(), 0);
	}

	{
		b2BodyDef bd;
		m_groundBody = m_world->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(20, 2);// (b2Vec2(200.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		m_groundBody->CreateFixture(&shape, 0.0f);
		Renderer::Pixel pix(RENDER.GetWidth() / 2, RENDER.GetHeight() + 2 * PHYSICS_TO_PIXELS);
		m_groundBody->SetTransform(pix.ToPhysics(), 0);
	}

	{
		b2BodyDef bd;
		b2Body* ground = m_world->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(20, 2);// (b2Vec2(200.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		ground->CreateFixture(&shape, 0.0f);
		Renderer::Pixel pix(RENDER.GetWidth() / 2,  -2 * PHYSICS_TO_PIXELS);
		ground->SetTransform(pix.ToPhysics(), 0);
	}

	{
		b2BodyDef bd;
		b2Body* ground = m_world->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(2, RENDER.GetHeight() * PIXELS_TO_PHYSICS);// (b2Vec2(200.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		ground->CreateFixture(&shape, 0.0f);
		Renderer::Pixel pix(-2 * PHYSICS_TO_PIXELS, RENDER.GetHeight() * .5f);
		ground->SetTransform(pix.ToPhysics(), 0);
	}

	{
		b2BodyDef bd;
		b2Body* ground = m_world->CreateBody(&bd);

		b2PolygonShape shape;
		shape.SetAsBox(2, RENDER.GetHeight() * PIXELS_TO_PHYSICS);// (b2Vec2(200.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		ground->CreateFixture(&shape, 0.0f);
		Renderer::Pixel pix(RENDER.GetWidth() + 2 * PHYSICS_TO_PIXELS, RENDER.GetHeight() * .5f);
		ground->SetTransform(pix.ToPhysics(), 0);
	}

	/*b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(10.0f, 10.0f);
	b2Body* body = m_world->CreateBody(&bd);

	b2CircleShape shape;
	shape.m_radius = .5f;
	body->CreateFixture(&shape, 1.0f);

	bd.position.Set(10.0f, 15.0f);
	body = m_world->CreateBody(&bd);
	shape.m_radius = 2.5f;
	body->CreateFixture(&shape, 1.0f);*/

}

void Physics::Update()
{
	
	m_world->SetAllowSleeping(false);
	m_world->SetWarmStarting(true);
	m_world->SetContinuousPhysics(true);

	b2Vec2 gravity;
	if (gravity_OnOff)
		gravity.Set(0.0f, -10.0f);
	else
		gravity.Set(0.0f, 0.0f);
	m_world->SetGravity(gravity);

	m_world->SetSubStepping(true);
	m_world->Step(g_fDeltaT, 10, 5);

	
}


void Physics::Draw()
{
	return;

	//OutputDebugString("Here\n");
	if (m_world != NULL && m_debugDraw != NULL)
	{
		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		//flags += b2Draw::e_aabbBit;
		flags += b2Draw::e_centerOfMassBit;
		
		m_debugDraw->SetFlags(flags);

		//OutputDebugString("Here inner\n");
		m_world->DebugDraw();
	}
}


b2Body* Physics::GenerateBody()
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.fixedRotation = true;
	//bd.position.Set(blok->GetX() * PIXELS_TO_PHYSICS, (RENDER.GetHeight() - blok->GetY()) * PIXELS_TO_PHYSICS);
	b2Body* body = m_world->CreateBody(&bd);

	return body;
}


void Physics::DestroyBody(b2Body* body)
{
	m_world->DestroyBody(body);
}

class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture) override
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

Blok* Physics::OnMouseDown(b2Vec2 p)
{
	if (m_mouseJoint != NULL)
	{
		return NULL;
	}

	// Make a small box.
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = p - d;
	aabb.upperBound = p + d;

	// Query the world for overlapping shapes.
	QueryCallback callback(p);
	m_world->QueryAABB(&callback, aabb);

	if (callback.m_fixture)
	{
		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef md;
		md.bodyA = m_groundBody;
		md.bodyB = body;
		md.target = p;
		md.maxForce = 1000.0f * body->GetMass();
		//md.dampingRatio = 1;
		//md.frequencyHz = 60;
		m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
		body->SetAwake(true);
		return (Blok*)body->GetUserData();
	}

	return NULL;
}

void Physics::UpdateMouse()
{
	if (m_mouseJoint != NULL)
	{
		b2Vec2 target = m_mouseTarget;
		b2Vec2 orgin = m_mouseJoint->GetAnchorB();
		b2Vec2 delta = m_mouseTarget - orgin;
		if (delta.Length() > 1.25f)
		{
			delta.Normalize();
			delta *= 1.25f;
			target = orgin + delta;
		}
		m_mouseJoint->SetTarget(target);
	}
}
void Physics::OnMouseMove(b2Vec2 p)
{
	m_mouseTarget = p;
}
void Physics::OnMouseUp()
{
	if (m_mouseJoint)
	{
		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}
}


