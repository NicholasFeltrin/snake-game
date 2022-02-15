#define OPENGL_H
#include "snake.h"

static u_int8_t *grid;

static enum DIRECTION direction[3] = {MOVE_RIGHT, DEFAULT, DEFAULT};

Textures textures;

void initGL(int glut_argc, char **glut_argv, u_int8_t *g){
	//set grid
	grid = g;

	//Initialize FreeGLUT
	glutInit(&glut_argc, glut_argv);

	//Create OpenGL 2,1 ocntext
	glutInitContextVersion(2,1);

	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Snake Game");

	//Initialize Projection Matrix
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    	//Initialize Modelview Matrix
    	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();

	//Initialize clear color
    	glClearColor( 0.f, 0.f, 0.f, 1.f );

	//Initialize viewprot
	glViewport(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT);	

	//initialize devIL
     	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION){
           	return -1;
     	}
     	ilInit(); 

	glEnable(GL_LINE_SMOOTH); 	//enable linux

    	//Check for error
    	GLenum error = glGetError();
    	if( error != GL_NO_ERROR ){
        	fprintf(stderr, "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		exit(EXIT_FAILURE);
    	}

	textures.snake_head = loadimage("asset/head.jpg");
	textures.snake_head = loadimage("asset/head.jpg");
	textures.snake_tail = loadimage("asset/head.jpg");

	//initializ functions
    	glutReshapeFunc(handleresize);       
	glutKeyboardFunc(handlekeys);
	glutDisplayFunc(render);
	glutTimerFunc(1000 / SCREEN_FPS, gl_runtime, 0);

       	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenTextures(1, &textures.snake_head);  // generate texture name
	glBindTexture(GL_TEXTURE_2D, textures.snake_head); // bind texture 
	glutMainLoop();
}

void handleresize(GLsizei newwidth, GLsizei newheight) {       
	int xdif = GRID_SCREEN_WIDTH - newwidth; int ydif = GRID_SCREEN_HEIGHT - newheight;
	int temp = xdif ^ ((xdif ^ ydif) & -(xdif < ydif));

	if(temp == 0){
		;
	}else if(temp > 0){
		GRID_SCREEN_WIDTH -= temp;
		GRID_SCREEN_HEIGHT -= temp;
	}else{
		GRID_SCREEN_WIDTH += -temp;
		GRID_SCREEN_HEIGHT += -temp;
	}

        glViewport(0, 0, SCREEN_WIDTH=newwidth, SCREEN_HEIGHT=newheight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
    	glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );
        glMatrixMode(GL_MODELVIEW);

        glutPostRedisplay();
}

void handlekeys(char key, int x, int y){

	if(game_state == GAME_START){
		switch(key){
			case 'q':
				glutDestroyWindow(glutGetWindow());
				break;
		
			case 'w':
				direction[0] = MOVE_UP;
				game_state = GAME_PLAYING;
				break;
			case 'a':
				direction[0] = MOVE_LEFT;
				game_state = GAME_PLAYING;
				break;
			case 's':
				direction[0] = MOVE_DOWN;
				game_state = GAME_PLAYING;
				break;
			case 'd':
				direction[0] = MOVE_RIGHT;
				game_state = GAME_PLAYING;
				break;

			//start game
			default:
				game_state = GAME_PLAYING;
				break;
		}
	}else if(game_state == GAME_PLAYING){
		switch(key){
			case 'q':
				glutDestroyWindow(glutGetWindow());
				break;

			case 'p':
				game_state = GAME_START;
				break;
		
			//movement keys
			case 'w':
				direction[1 + !(direction[1] == DEFAULT)] = MOVE_UP;
				break;
			case 'a':
				direction[1 + !(direction[1] == DEFAULT)] = MOVE_LEFT;
				break;
			case 's':
				direction[1 + !(direction[1] == DEFAULT)] = MOVE_DOWN;
				break;
			case 'd':
				direction[1 + !(direction[1] == DEFAULT)] = MOVE_RIGHT;
				break;
		
			default:
				break;
		}
	}
}

GLuint loadimage(char *filename){
	ILuint    image; 
	ILboolean temp; 
	
	ilGenImages(1, &image);
	ilBindImage(image);   
	temp = ilLoadImage(filename);
	temp = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	//if((temp = ilLoadImage(filename))){
	    	//if(!(temp = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))){
	    		//fprintf(stderr, "error bruh\n");
		//}
	//}

	return image;
}

void render(void){
	#define block_sizex (int)(GRID_SCREEN_WIDTH / GRIDX)
	#define block_sizey (int)(GRID_SCREEN_HEIGHT / GRIDY)

    	glClear(GL_COLOR_BUFFER_BIT);

	//reset modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//move to center of the screen
	glTranslatef( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );

	//draw background
    	glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );
	glBegin(GL_QUADS);
		glColor3f(RGB_NORMALIZE(87), RGB_NORMALIZE(138), RGB_NORMALIZE(52));
			glVertex2f(-SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f);	
			glVertex2f(-SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);	
			glVertex2f(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);	
			glVertex2f(SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f);	
	glEnd();

	//set viewport
    	glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT-50 );

	int topleftx;
	int toplefty;

	const float colors_empty[2][3] = {
		{RGB_NORMALIZE(170), RGB_NORMALIZE(215), RGB_NORMALIZE(81)},
		{RGB_NORMALIZE(162), RGB_NORMALIZE(209), RGB_NORMALIZE(73)}
	};


	int temp = 1;

	for(int j = 1;j <= GRIDY;++j){
		temp ^= 1;
		for(int i = 1;i <= GRIDX;++i){
			glBegin(GL_QUADS);
				glColor3f(colors_empty[(IX(i, j)&1) == temp][0], 
						colors_empty[(IX(i, j)&1) == temp][1], 
						colors_empty[(IX(i, j)&1) == temp][2] );
			
					topleftx = (block_sizex * (i - 1)) - GRID_SCREEN_WIDTH / 2.f;
					toplefty = (block_sizey * (j - 1)) - GRID_SCREEN_HEIGHT / 2.f;
			
					glVertex2f(topleftx + block_sizex, toplefty);
					glVertex2f(topleftx, toplefty);
					glVertex2f(topleftx, toplefty + block_sizey);
					glVertex2f(topleftx + block_sizex, toplefty + block_sizey);
			glEnd();
		}
	}    

	//color squares
	glColor3f(1,1,1);

	for(int j = 1;j <= GRIDY;++j){
		for(int i = 1;i <= GRIDX;++i){

			switch(grid[IX(i, j)]){
				case EMPTY:
					break;
				default:
					topleftx = (block_sizex * (i - 1)) - GRID_SCREEN_WIDTH / 2.f;
					toplefty = (block_sizey * (j - 1)) - GRID_SCREEN_HEIGHT / 2.f;

					glEnable(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexImage2D(GL_TEXTURE_2D, 0,
						       ilGetInteger(IL_IMAGE_BPP),
						       ilGetInteger(IL_IMAGE_WIDTH),
						       ilGetInteger(IL_IMAGE_HEIGHT), 0,
						       ilGetInteger(IL_IMAGE_FORMAT),
						       GL_UNSIGNED_BYTE, ilGetData()); // specify texture 

					glBegin(GL_QUADS);
						glTexCoord2f(0,0); glVertex2f(topleftx+block_sizex, toplefty);
						glTexCoord2f(0,1); glVertex2f(topleftx, toplefty);
						glTexCoord2f(1,1); glVertex2f(topleftx, toplefty+block_sizey);
						glTexCoord2f(1,0); glVertex2f(topleftx+block_sizex, toplefty+block_sizey);
					glEnd();

					glDisable(GL_TEXTURE_2D);
				break;
			}
		}
	}

    	//Update screen
    	glutSwapBuffers();
}

#undef block_sizex
#undef block_sizey

void update(void){
	switch(game_state){
		case GAME_START:

			break;

		case GAME_PLAYING:
			if(direction[1] == DEFAULT){
				;
			}else if(direction[2] == DEFAULT){
				direction[0] = direction[1];	
				direction[1] = DEFAULT;
			}else{
				direction[0] = direction[1];	
				direction[1] = direction[2];
				direction[2] = DEFAULT;
			}
			snake_move(direction[0]);
			break;
	}
}

void gl_runtime(int val){
	update();
	render();
	
	glutTimerFunc(1000 / SCREEN_FPS, gl_runtime, val);
}
