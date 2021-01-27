#pragma once

#include <Box2D/Box2D.h>
#include "GameObject.h"

/// Simple debug draw functionality.
/// Use the relevant Draw* methode to render debug stuff in the current frame
class DebugDraw : public b2Draw
{
public:
	static DebugDraw& getInstance()
	{
		static DebugDraw instance;
		return instance;
	}

	void draw_line(const Vector2f& p1, const Vector2f& p2, const Color& color, float thickness = 1.0f);
	void draw_arrow(const Vector2f& origin, const Vector2f& direction, const Color& color, float thickness = 1.0f);
	void draw_circle(const Vector2f& center, float radius, const Color& outline_color,
	                 const Color& fill_color = Color::Transparent);
	void draw_text(const std::string& str, const Vector2f& center, const Color& color);
	void draw_rectangle(const Vector2f& position, const Vector2f& size, const Color& outline_color,
	                    const Color& fill_color = Color::Transparent);
	void DrawRectangle(const Vector2f& p1, const Vector2f& p2, const Color& outlineColor,
	                   const Color& fillColor = Color::Transparent);

	// Box2D Debug Draw
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;
	void drawPolygonShape(const b2PolygonShape& b2shape, GameObject& trans, const b2Color& color);
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;
	void drawAABB(b2AABB* aabb, const Color& color);
	// end Box2D Debug Draw

	bool is_enabled() const
	{
		return m_enabled;
	}

	void set_enabled(bool enabled)
	{
		m_enabled = enabled;
	}

	void update(float deltaTime);

	void draw(RenderWindow& renderWindow);

private:
	DebugDraw();

	DebugDraw(DebugDraw const&)
	{
	};

	void operator=(DebugDraw const&) const
	{
	};

	~DebugDraw()
	= default;

	bool m_enabled;
	std::vector<std::shared_ptr<Drawable>>	m_shapes;
	Font m_font;
};
