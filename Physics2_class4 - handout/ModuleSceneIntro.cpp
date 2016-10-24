#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "animation.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	background = App->textures->Load("pinball/ruby.png");


	sensor = App->physics->CreateRectangleSensor(47, 187, 5, 5);
	sensor2 = App->physics->CreateRectangleSensor(170, 197, 5, 5);
	//Start Shape Map
	Shape_Map1();


	ball = App->physics->CreateCircle(250,380, 6);
	ball->body->IsBullet();

	//////////Revolution Joint
	////RIGHT
	right = App->physics->CreateRectangle(149,376, 28,7);
	
	
	
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
	left = App->physics->CreateRectangle(89, 376, 28, 7);


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
	
	//////////Prismatic Joint
	launcher = App->physics->CreateRectangle(200, 200, 10, 3);
	staticLauncher = App->physics->CreateRectangle(200, 204, 10, 3);
	staticLauncher->body->SetType(b2_staticBody);

	prismaticJoint_launcher.bodyA = launcher->body;
	prismaticJoint_launcher.bodyB = staticLauncher->body;
	prismaticJoint_launcher.localAnchorA.Set(0, 0);
	prismaticJoint_launcher.localAnchorB.Set(0, 0);
	prismaticJoint_launcher.collideConnected = true; 
	prismaticJoint_launcher.upperTranslation = 5;
	b2PrismaticJoint* joint_launcher = (b2PrismaticJoint*)App->physics->world->CreateJoint(&prismaticJoint_launcher);


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
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		b2Vec2 force = b2Vec2(0, -400);
		right->body->ApplyForceToCenter(force, 1);
		revoluteJointDef_right.lowerAngle = 30 * DEGTORAD;
	}

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		ball->body->SetTransform(b2Vec2(PIXEL_TO_METERS( App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY())), 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		b2Vec2 force = b2Vec2(0, -400);
		left->body->ApplyForceToCenter(force, 1);
		revoluteJointDef_left.lowerAngle = 30 * DEGTORAD;
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
		if (shape_map == true) {
			Shape_Map1();
			shape_map = !shape_map;
			change = !change;
		}
		else {
			Shape_Map2();
			shape_map = !shape_map;
			change = !change;
		}
	}
	

	launcher->body->ApplyForceToCenter(b2Vec2(0, 1), 1);

	//OnCollision(ball,sensor);
	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}
	
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

	int rebote1[6] = {
		173, 318,
		173, 337,
		155, 348
	};

	int rebote2[6] = {
		66, 317,
		66, 338,
		84, 348
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