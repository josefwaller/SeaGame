#pragma once

struct Triangle
{
	// The points in the triangle
	sf::Vector2f points[3];
	Triangle(std::vector<sf::Vector2f> p)
	{
		for (size_t i = 0; i < 3; i++) {
			points[i] = p[i];
		}
	}
	bool collidesWith(Triangle other)
	{
		// TBA
		return false;
	}
};