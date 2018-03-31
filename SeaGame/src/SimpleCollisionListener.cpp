#include "SimpleCollisionListener.h"
#include "Entity.h"
#include "PhysicsComponent.h"

void SimpleCollisionListener::BeginContact(b2Contact* contact)
{
	// Get both the entities
	std::weak_ptr<Entity> one;
	std::weak_ptr<Entity> two;
	void* oneVoid = contact->GetFixtureA()->GetBody()->GetUserData();
	if (oneVoid) {
		// Dear future Josef, if there is an error here, it is probably because an 
		// entity was destroyed without properly destroying its box 2d collision body
		// and then the body hit something and called the collision method on an entity which did not
		// exist anymore
		one = (static_cast<PhysicsComponent*>(oneVoid))->getParent();
	}
	else {
		throw std::runtime_error("UserData on a Box2D body was not a PhysicsComponent");
	}
	void* twoVoid = contact->GetFixtureB()->GetBody()->GetUserData();
	if (twoVoid) {
		// see above comment
		two = static_cast<PhysicsComponent*>(twoVoid)->getParent();
	}
	else {
		throw std::runtime_error("UserData on a Box2D body was not a PhysicsComponent");
	}
	// Call the controller's collision methods
	if (one.lock()->controller != nullptr) {
		one.lock()->controller->onCollision(two);
	}
	if (two.lock()->controller != nullptr) {
		two.lock()->controller->onCollision(one);
	}
}