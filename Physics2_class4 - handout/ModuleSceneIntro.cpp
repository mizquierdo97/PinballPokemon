#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "animation.h"
#include <math.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("Game/pinball/wheel.png");
	box = App->textures->Load("Game/pinball/crate.png");
	rick = App->textures->Load("Game/pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("Game/pinball/bonus.wav");
	background = App->textures->Load("Game/pinball/ruby.png");
	loop = App->textures->Load("Game/pinball/loopup.png");
	font = App->textures->Load("Game/pinball/font.png");
	take_font();

	sensor = App->physics->CreateRectangleSensor(47, 187, 5, 5);
	sensor2 = App->physics->CreateRectangleSensor(170, 197, 5, 5);
	sensor_square = App->physics->CreateRectangleSensor(218, 157, 5, 5);
	sensor_sout = App->physics->CreateRectangleSensor(190, 260, 5, 5);
	s_reset = App->physics->CreateRectangleSensor(100, 430, 200, 20);
	//Start Shape Map
	Shape_Map1();


	ball = App->physics->CreateCircle(250, 350, 6);
	

	////RIGHT

	right = App->physics->CreateRectangle(149, 376, 26, 7);



	point_right = App->physics->CreateCircle(149, 376, 2);
	point_right->body->SetType(b2_staticBody);


	revoluteJointDef_right.bodyA = right->body;
	revoluteJointDef_right.bodyB = point_right->body;
	revoluteJointDef_right.referenceAngle = 0 * DEGTORAD;
	revoluteJointDef_right.enableLimit = true;
	revoluteJointDef_right.lowerAngle = -30 * DEGTORAD;
	revoluteJointDef_right.upperAngle = 30 * DEGTORAD;
	revoluteJointDef_right.localAnchorA.Set(PIXEL_TO_METERS(13), 0);
	revoluteJointDef_right.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_right = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef_right);


	////LEFT
	left = App->physics->CreateRectangle(89, 376, 26, 7);


	point_left = App->physics->CreateCircle(87, 376, 2);
	point_left->body->SetType(b2_staticBody);



	revoluteJointDef_left.bodyA = left->body;
	revoluteJointDef_left.bodyB = point_left->body;
	revoluteJointDef_left.referenceAngle = 0 * DEGTORAD;
	revoluteJointDef_left.enableLimit = true;
	revoluteJointDef_left.lowerAngle = -30 * DEGTORAD;
	revoluteJointDef_left.upperAngle = 30 * DEGTORAD;
	revoluteJointDef_left.localAnchorA.Set(PIXEL_TO_METERS(-13), 0);
	revoluteJointDef_left.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_left = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef_left);


	//BUMPERS
	//1
	b2BodyDef bumper1;
	bumper1.type = b2_kinematicBody; //this will be a dynamic body
	bumper1.position.Set(PIXEL_TO_METERS(130), PIXEL_TO_METERS(127)); //a little to the left

	body_bumper1 = App->physics->world->CreateBody(&bumper1);
	b2CircleShape shape_bumper1;
	shape_bumper1.m_p.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //position, relative to body position
	shape_bumper1.m_radius = PIXEL_TO_METERS(5); //radius
	b2FixtureDef f_bumper1;
	f_bumper1.shape = &shape_bumper1; //this is a pointer to the shape above
	f_bumper1.restitution = 1;
	body_bumper1->CreateFixture(&f_bumper1); //add a fixture to the body
	
	//2
	b2BodyDef bumper2;
	bumper2.type = b2_kinematicBody; //this will be a dynamic body
	bumper2.position.Set(PIXEL_TO_METERS(160), PIXEL_TO_METERS(127)); //a little to the left

	body_bumper2 = App->physics->world->CreateBody(&bumper2);
	b2CircleShape shape_bumper2;
	shape_bumper2.m_p.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //position, relative to body position
	shape_bumper2.m_radius = PIXEL_TO_METERS(5); //radius
	b2FixtureDef f_bumper2;
	f_bumper2.shape = &shape_bumper2;
	f_bumper2.restitution = 1;//this is a pointer to the shape above
	body_bumper2->CreateFixture(&f_bumper2); //add a fixture to the body
											 

	 //3
	b2BodyDef bumper3;
	bumper3.type = b2_kinematicBody; //this will be a dynamic body
	bumper3.position.Set(PIXEL_TO_METERS(145), PIXEL_TO_METERS(152)); //a little to the left
	
	body_bumper3 = App->physics->world->CreateBody(&bumper3);
	b2CircleShape shape_bumper3;
	shape_bumper3.m_p.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //position, relative to body position
	shape_bumper3.m_radius = PIXEL_TO_METERS(5); //radius
	b2FixtureDef f_bumper3;
	f_bumper3.shape = &shape_bumper3;
	f_bumper3.restitution = 1;//this is a pointer to the shape above
	body_bumper3->CreateFixture(&f_bumper3); //add a fixture to the body



	//SLINGSHOT
	
	//1
	b2BodyDef slingshot1;
	slingshot1.type = b2_staticBody; //this will be a dynamic body
	slingshot1.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //a little to the left

	b2Body* body_slingshot1 = App->physics->world->CreateBody(&slingshot1);
	b2Vec2 vertices1[3];
	vertices1[0].Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	vertices1[1].Set(PIXEL_TO_METERS(-10), PIXEL_TO_METERS(10));
	vertices1[2].Set(PIXEL_TO_METERS(-20), PIXEL_TO_METERS(22));


	b2PolygonShape shape_slingshot1;
	shape_slingshot1.Set(vertices1, 3); //pass array to the shape
	b2FixtureDef fixture_slingshot1;
	fixture_slingshot1.shape = &shape_slingshot1; //change the shape of the fixture
	slingshot1.position.Set(PIXEL_TO_METERS(173), PIXEL_TO_METERS(318)); //in the middle
	b2Body* dynamicBody2_l = App->physics->world->CreateBody(&slingshot1);
	fixture_slingshot1.restitution = 2;
	dynamicBody2_l->CreateFixture(&fixture_slingshot1); //add a fixture to the body
	//////////Prismatic Joint
	
	//launcher->body->SetType(b2_kinematicBody);

	//2
	b2BodyDef slingshot2;
	slingshot2.type = b2_staticBody; //this will be a dynamic body
	slingshot2.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0)); //a little to the left

	b2Body* body_slingshot2 = App->physics->world->CreateBody(&slingshot2);
	b2Vec2 vertices2[3];
	vertices2[0].Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	vertices2[1].Set(PIXEL_TO_METERS(10), PIXEL_TO_METERS(10));
	vertices2[2].Set(PIXEL_TO_METERS(20), PIXEL_TO_METERS(22));


	b2PolygonShape shape_slingshot2;
	shape_slingshot2.Set(vertices2, 3); //pass array to the shape
	b2FixtureDef fixture_slingshot2;
	fixture_slingshot2.shape = &shape_slingshot2; //change the shape of the fixture
	slingshot2.position.Set(PIXEL_TO_METERS(66), PIXEL_TO_METERS(318)); //in the middle
	b2Body* dynamicBody2_r = App->physics->world->CreateBody(&slingshot2);
	fixture_slingshot2.restitution = 2;
	dynamicBody2_r->CreateFixture(&fixture_slingshot2); //add a fixture to the body





	launcher = App->physics->CreateRectangle(243, 390, 15, 5);
	staticLauncher = App->physics->CreateRectangle(243, 390, 15, 5);
	//launcher = App->physics->CreateRectangle(100, 200, 15, 5);
	//staticLauncher = App->physics->CreateRectangle(100, 200, 15, 5);
	staticLauncher->body->SetType(b2_staticBody);
	
	prismaticJoint_launcher.bodyB = launcher->body;
	prismaticJoint_launcher.bodyA = staticLauncher->body;


	prismaticJoint_launcher.collideConnected = false;
	prismaticJoint_launcher.enableLimit = true;
	//prismaticJoint_launcher.type
	
	prismaticJoint_launcher.lowerTranslation = PIXEL_TO_METERS(20);
	prismaticJoint_launcher.upperTranslation = PIXEL_TO_METERS(35);
	
	prismaticJoint_launcher.localAnchorA.Set(0, 0);
	prismaticJoint_launcher.localAnchorB.Set(0, 0);


	prismaticJoint_launcher.localAxisA.Set(0, -1);

	b2PrismaticJoint* joint_launcher = (b2PrismaticJoint*)App->physics->world->CreateJoint(&prismaticJoint_launcher);
	

	//////  -------------------SPRITES--------------------

	sprites = App->textures->Load("Game/pinball/pokemons.png");

	// - ANIMATIONS - pokemons

	sprite_ball.PushBack({106,119,16,16});


	pikachu.PushBack({ 46,21,25,24 });
	pikachu.PushBack({ 72,21,26,28 });
	pikachu.speed = 0.04f;


	makuhita.PushBack({ 169,15,33,34 });
	makuhita.PushBack({ 206,19,31,37 });
	makuhita.speed = 0.04f;

	cyndaquil.PushBack({ 17,145,31,39 });
	cyndaquil.PushBack({ 55,146,33,41 });
	cyndaquil.speed = 0.04f;

	spoink_relax.PushBack({ 5,57,22,42 });
	spoink_relax.PushBack({ 33,58,22,42 });
	spoink_relax.speed = 0.04f;

	spoink_fast.PushBack({ 58,64,21,36 });
	spoink_fast.PushBack({ 83,64,21,36 });
	spoink_fast.PushBack({ 108,64,21,36 });
	spoink_fast.PushBack({ 83,64,21,36 });
	spoink_fast.speed = 0.2f;

	chic.PushBack({ 148,83,21,42 });
	chic.PushBack({ 173,82,21,44 });
	chic.speed = 0.04f;

	shark.PushBack({ 1,2,42,45 });
	shark.PushBack({ 206,78,40,48 });
	shark.speed = 0.01f;

	ballena.PushBack({ 150,53,25,23 });
	ballena.PushBack({ 184,53,25,23 });
	ballena.speed = 0.04;

	square_pika.PushBack({ 118,147,17,18 });
	square_pika.PushBack({ 138,147,17,18 });
	square_pika.PushBack({ 162,147,17,18 });
	square_pika.PushBack({ 138,147,17,18 });
	square_pika.speed = 0.1;

	square_p2.PushBack({ 118,147,17,18 });


	//////  -------------------SPRITES--------------------

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	static int pikachu_x = 20;
	App->renderer->Blit(background, 0, 0);
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		b2Vec2 force = b2Vec2(0, -150);
		ball->body->ApplyForceToCenter(force, 1);
		
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		b2Vec2 force = b2Vec2(-40, 0);
		ball->body->ApplyForceToCenter(force, 1);
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		b2Vec2 force = b2Vec2(20, 0);
		ball->body->ApplyForceToCenter(force, 1);

	}

	static int pow = 0;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		pow += 3;
		//b2Vec2 force = b2Vec2(0, -150);
		//ball->body->ApplyForceToCenter(force, 1);
		if (pow > 300)
			pow = 300;
		
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		b2Vec2 force = b2Vec2(0, -pow);
		launcher->body->ApplyForceToCenter(force,1);

	}


	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		b2Vec2 force = b2Vec2(0, -200);
		right->body->ApplyForceToCenter(force, 1);
		revoluteJointDef_right.lowerAngle = 30 * DEGTORAD;
		pikachu_x = 197;
	
	}

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY())), 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		b2Vec2 force = b2Vec2(0, -200);
		left->body->ApplyForceToCenter(force, 1);
		revoluteJointDef_left.lowerAngle = 30 * DEGTORAD;
		pikachu_x = 20;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		static bool temp = false;
		if (temp == true)
			Shape_Map1();

		else
			Shape_Map2();
		temp = !temp;
	}

	if (change == true) {
		if (ball->body->GetLinearVelocity().y < -0.1 || ball->body->GetLinearVelocity().y > 1.9) {
			if (shape_map == true) {
				Shape_Map1();
				shape_map = !shape_map;
				change = !change;
				loop_blit = true;
			}
			else {
				Shape_Map2();
				shape_map = !shape_map;
				change = !change;
				loop_blit = false;
			}
		}
	}

	if (reset == true) {
		Reset_ball();
		reset = false;
	}

	static uint time = 0;
	time++;
	float posx = cos((time+ 0)/45.0)* 20;
	float posy = sin((time + 0) / 45.0) * 20;
	b2Vec2 pos = b2Vec2(PIXEL_TO_METERS(posx +PIXEL_TO_METERS(145)), PIXEL_TO_METERS(posy + PIXEL_TO_METERS(137)));

	body_bumper1->SetTransform(pos,0);

	posx = cos((time + 90) / 45.0) * 20;
	 posy = sin((time + 90) / 45.0) * 20;
	 pos = b2Vec2(PIXEL_TO_METERS(posx + PIXEL_TO_METERS(145)), PIXEL_TO_METERS(posy + PIXEL_TO_METERS(137)));

	body_bumper2->SetTransform(pos, 0);
	
	posx = cos((time + 180) / 45.0) * 20;
	posy = sin((time + 180) / 45.0) * 20;
	pos = b2Vec2(PIXEL_TO_METERS(posx + PIXEL_TO_METERS(145)), PIXEL_TO_METERS(posy + PIXEL_TO_METERS(137)));

	body_bumper3->SetTransform(pos, 0);




	//OnCollision(ball,sensor);
	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if (c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if (ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if (hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}



	// ray -----------------
	if (ray_on == true)
	{
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}


	// --POKEMONS--
	App->renderer->Blit(sprites,pikachu_x,357, &(pikachu.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(sprites, 193, 260, &(makuhita.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(sprites, 76, 128, &(cyndaquil.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(sprites, 55, 200, &(chic.GetCurrentFrame()), 0.01f);
	App->renderer->Blit(sprites, 15, 265, &(ballena.GetCurrentFrame()), 0.01f);

	if (App->input->GetKey(SDL_SCANCODE_Z) != KEY_REPEAT)
	{
		App->renderer->Blit(sprites, 232, 352, &(spoink_relax.GetCurrentFrame()), 0.01f);
	}
	else
	{
		App->renderer->Blit(sprites, 232, 358, &(spoink_fast.GetCurrentFrame()), 0.01f);
	}
	
	



	App->renderer->Blit(sprites, ball->body->GetPosition().x*25 - 7 , ball->body->GetPosition().y*25 -7, &(sprite_ball.GetCurrentFrame()));
	if (loop_blit == true) {
		App->renderer->Blit(loop, 0, 26);
	}
	App->renderer->Blit(sprites, 180, 148, &(shark.GetCurrentFrame()), 0.01f);

	//square pikachu
	static int temp = 0;
	if (temp < 170 && square == true)
	{
		App->renderer->Blit(sprites, 210, 145, &(square_pika.GetCurrentFrame()), 0.01f);
	}
	
	if (square == false)
	{
		App->renderer->Blit(sprites, 210, 145, &(square_p2.GetCurrentFrame()), 0.00f);
	}

	if (temp > 170)
	{
		square = false;
		temp = 0;
	}
	temp++;




	//BLITFONT
	//App->renderer->Blit(font, 1, 1, &numbers[4]);
	blit_font();
	return UPDATE_CONTINUE;

}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);
	static bool temp = true;

}

void ModuleSceneIntro::Shape_Map1()
{
	p2List_item<PhysBody*>* temp = ricks.getFirst();
	while (temp != NULL) {
		b2Body* body = temp->data->body;
		temp->data->body->GetWorld()->DestroyBody(body);
		temp = temp->next;
		//ricks.clear();
	}
	ricks.clear();


	int map[96] = {
		235, 394,
		252, 394,
		252, 174,
		247, 150,
		241, 130,
		228, 110,
		209, 91,
		194, 78,
		173, 68,
		147, 56,
		120, 56,
		88, 62,
		57, 75,
		34, 94,
		20, 117,
		13, 145,
		12, 182,
		13, 204,
		16, 218,
		22, 233,
		29, 248,
		38, 263,
		46, 272,
		46, 299,
		42, 305,
		29, 306,
		24, 313,
		24, 363,
		97, 403,
		97, 439,
		143, 439,
		143, 406,
		216, 361,
		216, 309,
		209, 301,
		193, 301,
		192, 270,
		205, 257,
		215, 240,
		222, 221,
		228, 196,
		228, 172,
		224, 147,
		220, 124,
		228, 135,
		234, 158,
		235, 206,
		235, 350
	};

	int shape1[26] = {
		57, 94,
		48, 101,
		41, 113,
		36, 128,
		33, 147,
		41, 165,
		53, 181,
		53, 175,
		50, 165,
		47, 146,
		50, 124,
		53, 118,
		57, 104
	};
	int shape2[58] = {
		109, 71,
		96, 78,
		84, 86,
		76, 99,
		70, 123,
		70, 138,
		72, 155,
		74, 170,
		76, 177,
		81, 178,
		81, 173,
		81, 166,
		76, 146,
		75, 130,
		76, 120,
		87, 115,
		93, 115,
		100, 116,
		103, 124,
		103, 133,
		103, 146,
		101, 159,
		100, 166,
		105, 169,
		120, 170,
		120, 160,
		110, 141,
		106, 101,
		110, 97
	};
	int shape3[20] = {
		32, 185,
		50,203,
		61, 210,
		66, 225,
		78, 243,
		78, 253,
		67, 263,
		49, 240,
		38, 216,
		34, 197

	};
	int shape4[12] = {
		131, 76,
		128, 72,
		124, 75,
		124, 91,
		124, 98,
		130, 98
	};
	int shape5[16] = {
		191, 322,
		191, 345,
		185, 351,
		152, 370,
		157, 380,
		193, 359,
		196, 353,
		196, 322
	};
	int shape6[14] = {
		49, 322,
		43, 322,
		43, 356,
		80, 378,
		86, 371,
		53, 350,
		49, 344
	};
	int shape7[10] = {
		145, 77,
		145, 99,
		152, 99,
		152, 77,
		149, 74
	};
/*
	int shape8[42] = {
		191, 227,
		191, 241,
		196, 232,
		203, 216,
		207, 200,
		210, 182,
		209, 161,
		208, 144,
		202, 124,
		192, 102,
		178, 89,
		166, 82,
		166, 99,
		180, 109,
		188, 119,
		195, 135,
		201, 157,
		204, 176,
		204, 188,
		200, 203,
		195, 218
	};



	int shape9[34] = {
		173, 210,
		173, 222,
		185, 204,
		191, 185,
		188, 164,
		177, 163,
		168, 167,
		160, 179,
		154, 191,
		154, 200,
		161, 200,
		169, 182,
		175, 172,
		184, 171,
		184, 184,
		181, 193,
		177, 200
	};*/

	int shape8[64] = {
		166, 101,
		166, 82,
		184, 94,
		199, 115,
		208, 143,
		211, 175,
		208, 204,
		200, 225,
		193, 241,
		189, 238,
		197, 220,
		204, 197,
		205, 170,
		200, 171,
		191, 186,
		180, 216,
		173, 223,
		172, 211,
		190, 176,
		203, 150,
		199, 137,
		187, 138,
		183, 151,
		176, 172,
		159, 199,
		154, 199,
		154, 188,
		166, 170,
		180, 142,
		184, 131,
		182, 109,
		174, 104
	};


	int rebote1[6] = {
		173, 318,
		173, 337,
		154, 340
	};

	int rebote2[6] = {
		66, 317,
		66, 338,
		85, 340
	};



	//ball = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5);

	ricks.add(App->physics->CreateChain(0, 0, map, 96));
	ricks.add(App->physics->CreateChain(0, 0, shape1, 26));
	ricks.add(App->physics->CreateChain(0, 0, shape2, 58));
	ricks.add(App->physics->CreateChain(0, 0, shape3, 20));
	ricks.add(App->physics->CreateChain(0, 0, shape4, 12));
	ricks.add(App->physics->CreateChain(0, 0, shape5, 16));
	ricks.add(App->physics->CreateChain(0, 0, shape6, 14));
	ricks.add(App->physics->CreateChain(0, 0, shape7, 10));
	ricks.add(App->physics->CreateChain(0, 0, shape8, 64));
	//ricks.add(App->physics->CreateChain(0, 0, shape9, 34));
	ricks.add(App->physics->CreateChain(0, 0, rebote1, 6));
	ricks.add(App->physics->CreateChain(0, 0, rebote2, 6));
}

void ModuleSceneIntro::Shape_Map2()
{
	p2List_item<PhysBody*>* asd = ricks.getFirst();
	while (asd != NULL) {
		b2Body* body = asd->data->body;
		asd->data->body->GetWorld()->DestroyBody(body);
		asd = asd->next;
		//ricks.clear();
	}
	ricks.clear();

	int map[74] = {
		143, 443,
		143, 403,
		215, 363,
		216, 314,
		209, 305,
		192, 305,
		192, 270,
		174, 214,
		196, 170,
		212, 139,
		224, 109,
		230, 83,
		223, 69,
		210, 82,
		207, 101,
		200, 116,
		195, 84,
		183, 63,
		165, 45,
		131, 30,
		82, 31,
		48, 45,
		20, 72,
		10, 103,
		11, 140,
		32, 185,
		50, 203,
		61, 210,
		71, 232,
		79, 245,
		47, 272,
		47, 303,
		30, 307,
		23, 318,
		23, 363,
		99, 403,
		99, 443
	};
	int shape1[36] = {
		154, 201,
		170, 173,
		184, 140,
		184, 116,
		174, 85,
		161, 68,
		150, 58,
		128, 50,
		97, 49,
		68, 56,
		49, 69,
		36, 85,
		29, 108,
		34, 150,
		49, 172,
		75, 190,
		98, 202,
		123, 210
	};


	ricks.add(App->physics->CreateChain(0, 0, map, 74));
	ricks.add(App->physics->CreateChain(0, 0, shape1, 36));



}

void ModuleSceneIntro::Reset_ball()
{
	ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS(243), PIXEL_TO_METERS(350)), 0);
	ball->body->SetAngularVelocity(0);
	ball->body->SetLinearVelocity(b2Vec2(0, 0));
}

void ModuleSceneIntro::take_font()
{
	
	for (int x = 0; x< 10; x++) {
		numbers[x] = { x*13 + 3, 3, 13,24 };
	}


}

void ModuleSceneIntro::blit_font() {
	int score_temp = score;
	for (int i = 8; i >= 0; i--) {
	
		int temp = score_temp % 10;
		App->renderer->Blit(font, i * 13 + 3, 1, &numbers[temp]);
		score_temp = score_temp / 10;
	}

}
