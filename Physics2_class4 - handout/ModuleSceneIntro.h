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
	void Reset_ball();
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
	bool reset = false;

	bool square = false;
	bool door = false;

	bool bs_pokeball = false;
	bool bs_flipper = false;
	bool bs_bumper = false;
	bool bs_slingshot = false;
	bool bs_pokemon = false;
	bool bs_points = false;
	bool s_cyndaquil = false;
	bool s_pikachu = false;
	bool s_shark = false;

	PhysBody* sensor;
	PhysBody* sensor2;
	PhysBody* s_reset;

	PhysBody* sen_pokeball;
	PhysBody* sen_shark;
	PhysBody* sen_cyndaquil;
	PhysBody* sen_pikachu;
	PhysBody* sen_points1;
	PhysBody* sen_points2;
	PhysBody* sen_points3;
	PhysBody* sen_points4;
	PhysBody* sen_points5;
	PhysBody* sen_points6;

	PhysBody* sensor_square;
	PhysBody* sensor_door;

	//PhysBody* b_bumper1;
	//PhysBody* b_bumper2;
	//PhysBody* b_bumper3;
	


	bool sensed;

	SDL_Texture* background;
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* loop;
	SDL_Texture* font;
	uint bonus_fx;
	uint s_flipper;
	uint s_bumper;
	uint s_slingshot;
	uint s_pokeball;
	uint s_pokemon;
	uint s_points;
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
	Animation a_right;
	Animation a_left;
	Animation bumpers1;
	Animation bumpers2;
	Animation bumpers3;

	Animation door_closed;
	Animation door_open;

	Animation square_pika;
	Animation square_p2;

};
