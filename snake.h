#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define GRIDX 16
#define GRIDY 16
#define IX(x, y) (y * (GRIDX+2)) + x		//project 2D to 1D
#define NULLEXIT(ptr, err) if(ptr == NULL){				\
				fprintf(stderr, "NULL error: %s", err);	\
				exit(EXIT_FAILURE);			\
			}

//types
typedef struct{
	u_int8_t is_node[(GRIDY+2)*(GRIDX+2)];
	int next_node[(GRIDY+2)*(GRIDX+2)];
}Cell;


//enum
enum DIRECTION{
	MOVE_UP 	= 1,
	MOVE_DOWN 	= 3,
	MOVE_LEFT  	= 2,
	MOVE_RIGHT 	= 4,
	DEFAULT 	= 0,
};

enum SQUARE_TYPES{
	BARIER 	= 255,	//if there is color thats bad
	FOOD 	= 4,	//red
	HEAD 	= 3,	//blue
	BODY 	= 2,	//green
	TAIL 	= 1,	//pink
	EMPTY 	= 0,	//black
};

enum GAME_STATE{
	GAME_START,
	GAME_PLAYING,
	GAME_OVER,
};

extern void gl_runtime(int val);
extern void initGL(int glut_argc, char **glut_argv, u_int8_t *g);
extern u_int8_t *init_snake(void);

extern enum GAME_STATE game_state;


//
//snake header file
//
#ifdef SNAKE_H
#undef SNAKE_H


#include <string.h>
#include <time.h>
//#include <unistd.h>  		//linux dependent


extern void spawn_food(void);

#endif


//
//opengl header file
//
#ifdef OPENGL_H
#undef OPENGL_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <IL/il.h>

typedef struct{
	GLuint snake_head;
	GLuint snake_body;
	GLuint snake_tail;
}Textures;

//screen 
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 690;
#define SCREEN_FPS 7

int GRID_SCREEN_WIDTH = 640;
int GRID_SCREEN_HEIGHT  = 640;

#define RGB_NORMALIZE(v) (float)v/255

//functions
extern void update(void);
extern void render(void);
extern void handlekeys(char key, int x, int y);
extern void handleresize(GLsizei newwidth, GLsizei newheight);
extern GLuint loadimage(char *filename);

extern void snake_move(enum DIRECTION direc);

#endif
