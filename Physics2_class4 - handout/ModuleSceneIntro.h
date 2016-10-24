#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePhysics.h"
#include "Application.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Shape_Map1();
	void Shape_Map2();
public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	PhysBody* ball;
	PhysBody* right;
	PhysBody* point_right;
	PhysBody* left;
	PhysBody* point_left;
	b2RevoluteJointDef revoluteJointDef_right;
	b2RevoluteJointDef revoluteJointDef_left;


	PhysBody* sensor;
	bool sensed;

	SDL_Texture* background;
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
