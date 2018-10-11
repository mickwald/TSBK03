// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"

// L�gg till egna globaler h�r efter behov.

float kMaxDistance = 100.0f;
float cohesionFactor = 0.02f;
float separationFactor = 0.16f;
float alignmentFactor = 0.1f;
float blacksheepFactor = -3.f;

void SpriteBehavior() // Din kod!
{
// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
// hastigheter och positioner, eller arbeta fr�n egna globaler.
	SpritePtr sprite_i = gSpriteRoot;
	int count = 0;
	while(sprite_i != NULL){
		if(sprite_i->face->texID != 4){ //not black sheep
		count = 0;
		sprite_i->avg_pos.h = 0.0f;
		sprite_i->avg_pos.v = 0.0f;
		sprite_i->avoid_vec.h = 0.0f;
		sprite_i->avoid_vec.v = 0.0f;
		/*if(sprite_i == gSpriteRoot){
			printf("pos.h: %f \n", sprite_i->position.h);
			printf("pos.v: %f \n", sprite_i->position.v);
		}*/
		SpritePtr sprite_j = gSpriteRoot;
		//printf("pos.h: %f \n", sprite_i->position.h);
		//printf("pos.v: %f \n", sprite_i->position.v);
		/*if(sprite_j == sprite_i){
			sprite_j = sprite_j->next;
			printf("skipping sprite\n");
		}*/
		while(sprite_j != NULL){
			if(sprite_j != sprite_i){
				GLfloat dist =  sqrt(pow(sprite_i->position.h - 
							 sprite_j->position.h,2) +
					    	     pow(sprite_i->position.v - 
							 sprite_j->position.v,2));
				//printf("dist: %f \n", dist);
				if(dist <= kMaxDistance){
					//printf("near!! \n");
					count++;
					sprite_i->avg_pos.h += sprite_j->position.h;
					sprite_i->avg_pos.v += sprite_j->position.v;
					if(sprite_j->face->texID == 4){
						blacksheepFactor = 5.f;
					} else {
						blacksheepFactor = 1.f;
					}
					sprite_i->avoid_vec.h += blacksheepFactor*(sprite_i->position.h - sprite_j->position.h)* (1.f - dist / kMaxDistance);
					sprite_i->avoid_vec.v += blacksheepFactor*(sprite_i->position.v - sprite_j->position.v) * (1.f - dist / kMaxDistance);
					sprite_i->speed_diff.h += sprite_j->speed.h - sprite_i->speed.h;
					sprite_i->speed_diff.v += sprite_j->speed.v - sprite_i->speed.v;
				}
				
			
			}
		sprite_j = sprite_j->next;
		}
		if(count > 0){
			//printf("Count: %d\n", count);
			sprite_i->avg_pos.h = sprite_i->avg_pos.h/(GLfloat) count;
			sprite_i->avg_pos.v = sprite_i->avg_pos.v/(GLfloat) count;
			sprite_i->avoid_vec.h = sprite_i->avoid_vec.h/(GLfloat) count;
			sprite_i->avoid_vec.v = sprite_i->avoid_vec.v/(GLfloat) count;
			sprite_i->speed_diff.h = sprite_i->speed_diff.h/(GLfloat) count;
			sprite_i->speed_diff.v = sprite_i->speed_diff.v/(GLfloat) count;
			//printf("avg_pos.h: %f \n", sprite_i->avg_pos.h);
			//printf("avg_pos.v: %f \n\n", sprite_i->avg_pos.v);
		}
		count = 0;
		}
		sprite_i = sprite_i->next;
	}
	sprite_i = gSpriteRoot;
	while(sprite_i != NULL){
		if(sprite_i->face->texID != 4){
		if(sprite_i->avg_pos.h > 0.0f && sprite_i->avg_pos.v > 0.0f){
			FPoint cohesion;
			FPoint separation;
			FPoint alignment;
			cohesion.h = (sprite_i->avg_pos.h - sprite_i->position.h) * cohesionFactor;
			cohesion.v = (sprite_i->avg_pos.v - sprite_i->position.v) * cohesionFactor;
			separation.h = (sprite_i->avoid_vec.h  * separationFactor);
			separation.v = (sprite_i->avoid_vec.v * separationFactor);
			alignment.h = sprite_i->speed_diff.h * alignmentFactor;
			alignment.v = sprite_i->speed_diff.v * alignmentFactor;
			sprite_i->speed.h += cohesion.h + separation.h + alignment.h;
			sprite_i->speed.v += cohesion.v + separation.v + alignment.v;
			float speedVal = sqrt(pow(sprite_i->speed.h,2) + pow(sprite_i->speed.v,2));
			sprite_i->speed.h = 5.0f*sprite_i->speed.h/speedVal + ((((float)(rand() % 1000000)) - 500000.f)/1000000.f);
			sprite_i->speed.v = 5.0f*sprite_i->speed.v/speedVal + ((((float)(rand() % 1000000)) - 500000.f)/1000000.f);
			if(sprite_i->face->texID == 3){
				float chanceh = (((float)(rand() % 1000000))/1000000.f);
				float chancev = (((float)(rand() % 1000000))/1000000.f);
				if(chanceh < .01f){
					sprite_i->speed.h = -sprite_i->speed.h;
				}
				if(chancev < .01f){
					sprite_i->speed.v = -sprite_i->speed.v;
				}
			}
		}
		} else {
			//sprite_i->speed.h += ((((float)(rand() % 1000000)) - 500000.f)/1000000.f);
			//sprite_i->speed.v += ((((float)(rand() % 1000000)) - 500000.f)/1000000.f);
			float speedVal = sqrt(pow(sprite_i->speed.h,2) + pow(sprite_i->speed.v,2));
			sprite_i->speed.h = 5.0f*sprite_i->speed.h/speedVal;
			sprite_i->speed.v = 5.0f*sprite_i->speed.v/speedVal;
		}
		sprite_i = sprite_i->next;
	}

}

// Drawing routine
void Display()
{
	SpritePtr sp;

	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawBackground();

	SpriteBehavior(); // Din kod!

// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);

	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
	SpritePtr dog = gSpriteRoot;
  while(dog->face->texID != 4){
	dog = dog->next;
	}
  switch (key)
  {
    case 'q':
    	cohesionFactor += 0.01;
    	printf("cohesionFactor = %f\n", cohesionFactor);
    	break;
    case 'a':
    	cohesionFactor -= 0.01;
    	printf("cohesionFactore = %f\n", cohesionFactor);
    	break;
    case 'w':
    	separationFactor += 0.001;
    	printf("separationFactor = %f\n", separationFactor);
    	break;
    case 's':
    	separationFactor -= 0.001;
    	printf("separationFactor = %f\n", separationFactor);
    	break;
    case 'e':
    	alignmentFactor += 0.1;
    	printf("alignmentFactor = %f\n", alignmentFactor);
    	break;
    case 'd':
    	alignmentFactor -= 0.1;
    	printf("alignmentFactor = %f\n", alignmentFactor);
    	break;
    case 'r':
    	kMaxDistance += 10.0f;
    	printf("kMaxDistance = %f\n", kMaxDistance);
    	break;
    case 'f':
    	kMaxDistance -= 10.0f;
    	printf("kMaxDistance = %f\n", kMaxDistance);
    	break;
    case GLUT_KEY_UP:
    	dog->speed.v += 1.f;
    	printf("dog->speed.v = %f\n", dog->speed.v);
    	break;
    case GLUT_KEY_DOWN:
    	dog->speed.v -= 1.f;
    	printf("dog->speed.v = %f\n", dog->speed.v);
    	break;
    case GLUT_KEY_LEFT:
    	dog->speed.h -= 1.f;
    	printf("dog->speed.h = %f\n", dog->speed.h);
    	break;
    case GLUT_KEY_RIGHT:
    	dog->speed.h += 1.f;
    	printf("dog->speed.h = %f\n", dog->speed.h);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;

	srand(time(NULL));

	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund

	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat

	NewSprite(sheepFace, 100, 200, 1, 1);
	NewSprite(sheepFace, 200, 100, 1.5, -1);
	NewSprite(sheepFace, 250, 200, -1, 1.5);
	NewSprite(sheepFace, 500, 600, 1, 1);
	NewSprite(sheepFace, 400, 700, -1, -1);
	NewSprite(sheepFace, 450, 600, 1, 1);
	NewSprite(blackFace, 450, 600, -1, 1);
	NewSprite(blackFace, 450, 600, 1, -1);
	NewSprite(dogFace, 300, 400, 1, 1.5);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");

	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);

	InitSpriteLight();
	Init();

	glutMainLoop();
	return 0;
}
