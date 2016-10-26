#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePhysics.h"
#include "Application.h"
#include "animation.h"

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
	void take_font();
	void blit_font();
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

	b2Body* body_bumper1;
	b2Body* body_bumper2;
	b2Body* body_bumper3;

	SDL_Rect numbers[10];
	uint score = 00000000;

	PhysBody* launcher;
	PhysBody* staticLauncher;
	b2PrismaticJointDef prismaticJoint_launcher;

	bool shape_map = false;
	bool change = false;
	bool loop_blit = true;

	bool square = false;
	PhysBody* sensor;
	PhysBody* sensor2;
	

	PhysBody* sensor_square;
	PhysBody* sensor_sout;


	bool sensed;

	SDL_Texture* background;
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* loop;
	SDL_Texture* font;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;


	SDL_Texture* sprites = nullptr;
	Animation pikachu;
	Animation makuhita;
	Animation cyndaquil;
	Animation spoink_relax;
	Animation spoink_fast;
	Animation chic;
	Animation shark;
	Animation ballena;
	Animation sprite_ball;


	Animation square_pika;
	Animation square_p2;

};
