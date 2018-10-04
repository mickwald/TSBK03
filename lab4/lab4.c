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

#include <math.h>
#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"

// L�gg till egna globaler h�r efter behov.

#define kMaxDistance 50.0f

void SpriteBehavior() // Din kod!
{
// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
// hastigheter och positioner, eller arbeta fr�n egna globaler.
	SpritePtr sprite_i = gSpriteRoot;
	int count = 0;
	while(sprite_i != NULL){
		if(sprite_i == gSpriteRoot){
			printf("pos.h: %f \n", sprite_i->position.h);
			printf("pos.v: %f \n", sprite_i->position.v);
		}
		SpritePtr sprite_j = gSpriteRoot;
		//printf("pos.h: %f \n", sprite_i->position.h);
		//printf("pos.v: %f \n", sprite_i->position.v);
		if(sprite_j == sprite_i)
			sprite_j = sprite_j->next;
		while(sprite_j != NULL){
			GLfloat dist =  sqrt(pow(sprite_i->position.h - sprite_j->position.h,2) +
					    pow(sprite_i->position.v - sprite_j->position.v,2));
			//printf("dist: %f \n", dist);
			if(dist <= kMaxDistance){
				//printf("near!! \n");
				count++;
				sprite_i->avg_pos.h += sprite_j->position.h;
				sprite_i->avg_pos.v += sprite_j->position.v;
			}
			//printf("avg_pos.h: %f \n", avg_pos.h);
			//printf("avg_pos.v: %f \n\n\n", avg_pos.v);
			sprite_j = sprite_j->next;
		}
		if(count > 0){
			sprite_i->avg_pos.h = sprite_i->avg_pos.h/(GLfloat) count;
			sprite_i->avg_pos.v = sprite_i->avg_pos.v/(GLfloat) count;
		}
		count = 0;
		sprite_i = sprite_i->next;
	}
	sprite_i = gSpriteRoot;
	while(sprite_i != NULL){
		sprite_i->speed.h += sprite_i->avg_pos.h * 0.001f;
		sprite_i->speed.v += sprite_i->avg_pos.v * 0.001f;
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
  switch (key)
  {
    case '+':
    	someValue += 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;

	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund

	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat

	NewSprite(sheepFace, 100, 200, 1, 1);
	NewSprite(sheepFace, 200, 100, 1.5, -1);
	NewSprite(sheepFace, 250, 200, -1, 1.5);
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
