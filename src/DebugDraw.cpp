#include "stdafx.h"
#include "DebugDraw.h"
#include "VectorAlgebra2D.h"
#include "InputManager.h"
#include "PhysicsManager.h"


DebugDraw::DebugDraw() :
	m_enabled(true)
{
	if (!m_font.loadFromFile("../assets/consolab.ttf"))
	{
		err() << "Could not load font\n";
	}
};

void DebugDraw::draw_line(const Vector2f& p1, const Vector2f& p2, const Color& color, const float thickness)
{
	if (!m_enabled)
	{
		return;
	}

	auto dir = p2 - p1;
	const auto length = MathUtil::length(dir);

	if (length >= 1.0f)
	{
		dir = (dir / length) * thickness * 0.5f;
		const Vector2f right(dir.y, -dir.x);

		std::shared_ptr<ConvexShape> shape(new ConvexShape());
		shape->setPointCount(4);
		shape->setPoint(0, p1 - right);
		shape->setPoint(1, p1 + right);
		shape->setPoint(2, p2 + right);
		shape->setPoint(3, p2 - right);
		shape->setFillColor(color);

		m_shapes.push_back(shape);
	}
}

void DebugDraw::draw_arrow(const Vector2f& origin, const Vector2f& direction, const Color& color, const float thickness)
{
	if (!m_enabled)
	{
		return;
	}

	const auto length = MathUtil::length(direction);

	if (length >= 1.0f)
	{
		const auto unit_direction = direction / length;
		const Vector2f right(unit_direction.y, -unit_direction.x);

		const auto head_size = 5.0f;
		const auto head = length > (head_size * 2) ? length - head_size : length * 0.5f;

		const auto p1 = origin;
		const auto p2 = origin + unit_direction * head;
		const auto p3 = origin + direction;

		std::shared_ptr<ConvexShape> shape(new ConvexShape());
		shape->setPointCount(4);

		shape->setPoint(0, p1 - right * thickness * 0.5f);
		shape->setPoint(1, p1 + right * thickness * 0.5f);
		shape->setPoint(2, p2 + right * thickness * 0.5f);
		shape->setPoint(3, p2 - right * thickness * 0.5f);
		shape->setFillColor(color);

		m_shapes.push_back(shape);

		std::shared_ptr<ConvexShape> head_shape(new ConvexShape());
		head_shape->setPointCount(3);

		head_shape->setPoint(0, p2 - right * head_size);
		head_shape->setPoint(1, p2 + right * head_size);
		head_shape->setPoint(2, p3);
		head_shape->setFillColor(color);

		m_shapes.push_back(head_shape);
	}
}

void DebugDraw::draw_circle(const Vector2f& center, const float radius, const Color& outline_color,
                            const Color& fill_color)
{
	if (!m_enabled)
	{
		return;
	}

	auto shape = std::make_shared<CircleShape>(radius);
	shape->setOutlineThickness(1.0f);
	shape->setOutlineColor(outline_color);
	shape->setFillColor(fill_color);
	shape->setPosition(center);
	shape->setOrigin(Vector2f(radius, radius));

	m_shapes.push_back(shape);
}

void DebugDraw::draw_rectangle(const Vector2f& position, const Vector2f& size, const Color& outline_color,
                               const Color& fill_color)
{
	if (!m_enabled)
	{
		return;
	}

	std::shared_ptr<RectangleShape> shape(new RectangleShape(size));
	shape->setOutlineThickness(1.0f);
	shape->setOutlineColor(outline_color);
	shape->setFillColor(fill_color);
	shape->setPosition(position);
	shape->setOrigin(size * 0.5f);

	m_shapes.push_back(shape);
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, const int32 vertex_count, const b2Color& color)
{
	auto polygon = std::make_shared<ConvexShape>();
	for (auto i = 0; i < vertex_count; i++)
	{
		polygon->setPoint(i, PhysicsManager::b2s(vertices[i]));
	}
	polygon->setOutlineColor(PhysicsManager::b2s(color));
	polygon->setOutlineThickness(1.0f);
	m_shapes.push_back(polygon);
}


void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, const int32 vertex_count, const b2Color& color)
{
	auto polygon = std::make_shared<ConvexShape>();
	for (auto i = 0; i < vertex_count; i++)
	{
		polygon->setPoint(i, PhysicsManager::b2s(vertices[i]));
	}
	polygon->setFillColor(PhysicsManager::b2s(color, 50));
	polygon->setOutlineColor(PhysicsManager::b2s(color));
	polygon->setOutlineThickness(1.0f);
	m_shapes.push_back(polygon);
}

void DebugDraw::DrawCircle(const b2Vec2& center, const float32 radius, const b2Color& color)
{
	auto circle = std::make_shared<CircleShape>((radius * PhysicsManager::RATIO));
	circle->setPosition(Vector2f(center.x * PhysicsManager::RATIO, center.y * PhysicsManager::RATIO));
	circle->setOutlineColor(PhysicsManager::b2s(color));
	m_shapes.push_back(circle);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, const float32 radius, const b2Vec2& axis, const b2Color& color)
{
	//no converion in cordinates of center and upper left corner, Circle in sfml is managed by default with the center
	auto circle = std::make_shared<CircleShape>((radius * PhysicsManager::RATIO));
	circle->setFillColor(PhysicsManager::b2s(color, 50));
	circle->setOutlineColor(PhysicsManager::b2s(color));
	circle->setPosition(Vector2f(center.x * PhysicsManager::RATIO, center.y * PhysicsManager::RATIO));

	// line of the circle wich shows the angle
	const auto p = center + (radius * axis);
	const auto line = std::make_shared<VertexArray>(Lines, 2);
	(*line)[0].position = Vector2f(center.x * PhysicsManager::RATIO, center.y * PhysicsManager::RATIO);
	(*line)[1].position = Vector2f(p.x * PhysicsManager::RATIO, p.y * PhysicsManager::RATIO);
	(*line)[0].color = (*line)[1].color = PhysicsManager::b2s(color);

	m_shapes.push_back(circle);
	m_shapes.push_back(line);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	const auto line = std::make_shared<VertexArray>(LinesStrip, 2);
	(*line)[0].position = PhysicsManager::b2s(p1);
	(*line)[1].position = PhysicsManager::b2s(p2);
	(*line)[0].color = (*line)[1].color = PhysicsManager::b2s(color);
	m_shapes.push_back(line);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	float line_size;
	auto x = xf.p.x * PhysicsManager::RATIO;
	auto y = xf.p.y * PhysicsManager::RATIO;
	const float line_proportion = 0.15; // 0.15 ~ 10 pixels
	const auto p1 = xf.p;

	//red (X axis)
	auto p2 = p1 + (line_proportion * xf.q.GetXAxis());
	const auto red_line = std::make_shared<VertexArray>(LinesStrip, 2);
	(*red_line)[0].position = Vector2f(p1.x * PhysicsManager::RATIO, p1.y * PhysicsManager::RATIO);
	(*red_line)[1].position = Vector2f(p2.x * PhysicsManager::RATIO, p2.y * PhysicsManager::RATIO);
	(*red_line)[0].color = (*red_line)[1].color = Color::Red;

	//green (Y axis)
	p2 = p1 - (line_proportion * xf.q.GetYAxis());
	const auto green_line = std::make_shared<VertexArray>(LinesStrip, 2);
	(*green_line)[0].position = Vector2f(p1.x * PhysicsManager::RATIO, p1.y * PhysicsManager::RATIO);
	(*green_line)[1].position = Vector2f(p2.x * PhysicsManager::RATIO, p2.y * PhysicsManager::RATIO);
	(*green_line)[0].color = (*green_line)[1].color = Color::Green;

	m_shapes.push_back(red_line);
	m_shapes.push_back(green_line);
}

void DebugDraw::drawPolygonShape(const b2PolygonShape& b2_shape, GameObject& trans, const b2Color& color)
{
	auto convex = std::make_shared<ConvexShape>();
	convex->setPointCount(b2_shape.m_count);

	for (int i = 0; i < b2_shape.m_count; i++)
	{
		convex->setPoint(i, PhysicsManager::b2s(b2_shape.m_vertices[i]));
	}
	convex->setPosition(trans.getPosition());
	convex->setRotation(trans.getRotation());
	convex->setOutlineColor(PhysicsManager::b2s(color));
	convex->setOutlineThickness(1);
	convex->setFillColor(PhysicsManager::b2s(color, 80));
	m_shapes.push_back(convex);
}

void DebugDraw::DrawPoint(const b2Vec2& p, const float32 size, const b2Color& color)
{
	DrawCircle(p, size, color);
}

void DebugDraw::drawAABB(b2AABB* aabb, const Color& color)
{
	auto polygon = std::make_shared<ConvexShape>();
	polygon->setPointCount(4);
	polygon->setPoint(0, sf::Vector2<float>(aabb->lowerBound.x * PhysicsManager::RATIO,
	                                        aabb->lowerBound.y * PhysicsManager::RATIO));
	polygon->setPoint(1, sf::Vector2<float>(aabb->upperBound.x * PhysicsManager::RATIO,
	                                        aabb->lowerBound.y * PhysicsManager::RATIO));
	polygon->setPoint(2, sf::Vector2<float>(aabb->upperBound.x * PhysicsManager::RATIO,
	                                        aabb->upperBound.y * PhysicsManager::RATIO));
	polygon->setPoint(3, sf::Vector2<float>(aabb->lowerBound.x * PhysicsManager::RATIO,
	                                        aabb->upperBound.y * PhysicsManager::RATIO));

	polygon->setOutlineThickness(1.0f);
	polygon->setOutlineColor(color);
	polygon->setFillColor(Color::Transparent);
	m_shapes.push_back(polygon);
}

void DebugDraw::draw_text(const std::string& str, const Vector2f& center, const Color& color)
{
	std::shared_ptr<Text> text(new Text());
	text->setPosition(center);
	text->setString(str);
	text->setFillColor(color);
	text->setFont(m_font);
	text->setCharacterSize(14);
	const auto bounds = text->getLocalBounds();
	text->setOrigin(
		bounds.width * 0.5f,
		bounds.height * 0.5f + bounds.top);

	m_shapes.push_back(text);
}

void DebugDraw::update(float deltaTime)
{
	if (InputManager::getInstance().isKeyReleased("debugdraw"))
	{
		set_enabled(!is_enabled());
	}
}

void DebugDraw::draw(RenderWindow& renderWindow)
{
	if (m_enabled)
	{
		for (const auto& shape : m_shapes)
		{
			renderWindow.draw(*shape);
		}
	}

	m_shapes.clear();
}
