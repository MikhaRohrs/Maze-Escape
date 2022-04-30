#include "ecm.h"
#include "cmp_projectile.h"

#include "cmp_timer.h"
#include "../scenes/scene_leaderboard.h"
#include "Box2D/Dynamics/b2Fixture.h"
using namespace std;
using namespace sf;

// Player projectiles, instantiated when the player uses his weapon.
void ProjectileComponent::update(double dt) {
	_lifetime -= dt;
    // If the projectile's lifespan reaches 0, delete it.
	if (_lifetime <= 0.f) 
	{
		_parent->setForDelete();
	}
	// If the projectile makes contact with the player, the player loses 2 seconds from hurting himself, and destroys the projectile.
	// Also checks if the projectile lifetime is less than 1.2 seconds remaining (It has been active for 0.3 seconds) to prevent fringe cases
	// of the player accidentally running into their projectile just after firing it.
	else if(length(_player->getPosition() - _parent->getPosition()) < 15.f && _lifetime < 1.2f)
	{
  		_parent->setForDelete();
		_timer->get_components<TimerComponent>()[0]->ChangeTime(-2.f);
	}
	// Final check would be to see if it hits an enemy, in which case the enemy and the bullet are both destroyed,
	// then the player gains time.
}

ProjectileComponent::ProjectileComponent(Entity* p)
    : PhysicsComponent(p, true, {6.f, 6.f}), _lifetime(1.5f),
	_player(_parent->scene->ents.find("player")[0]), _timer(_parent->scene->ents.find("timer")[0])
{}