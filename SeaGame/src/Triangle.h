#pragma once
#include <iostream>

struct Triangle
{
	// The points in the triangle
	std::vector<sf::Vector2f> points;
	// Used to draw colliding triangles as red instead of green
	Triangle(std::vector<sf::Vector2f> p)
	{
		points = p;
	}
	bool collidesWith(Triangle other)
	{
		for (auto otherP = other.points.begin(); otherP != other.points.end(); otherP++) {
			// Starts as true, will be set to false if the point is found to be outside the triangle
			bool isInside = true;
			for (size_t i = 0; i < this->points.size(); i++) {
				// If not inside, move on to next point
				if (!isInside)
					break;
				for (size_t j = i + 1; j < this->points.size(); j++) {
					// Get slope
					sf::Vector2f diff = (this->points[i]) - (this->points[j]);
					float m;
					if (diff.x == 0) {
						// Note that here, infinity is not actually infinite, just really big
						// But the slope will be large enough to work as pretty much a vertical line
						m = INFINITY;
					}
					else {
						m = diff.y / diff.x;
					}
					// Get y offset
					// y = mx + b
					// y - mx = b
					float b = (this->points[i]).y - m * (this->points[i]).x;
					// Get the one other point not being used
					size_t k = 3 - (j + i);
					// Get whether the point is above or below the line created by the other 2 points
					float sOne = std::copysign(1.0f, this->points[k].y - (m * this->points[k].x + b));
					// Get whether the other triangle's above or below the line
					float sTwo = std::copysign(1.0f, (*otherP).y - (m * (*otherP).x + b));
					// Check if they are not on the same side
					if (sOne != sTwo) {
						isInside = false;
						break;
					} // if
				} // for j
			} // for i
			// if the point has not been found to be outside the triangle by now, it must be inside the triangle
			if (isInside) {
				return true;
			}
		} // for otherP
		return false;
	}
};