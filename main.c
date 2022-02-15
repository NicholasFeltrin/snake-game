//#define OPENGL_H
#include "snake.h"

int main(int argc, char **argv){
	u_int8_t *temp;
	temp = init_snake();
	initGL(argc, argv, temp);

	exit(EXIT_SUCCESS);
}
