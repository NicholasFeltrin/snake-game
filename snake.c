#define SNAKE_H
#include "snake.h"

Cell *grid;

int grid_length = GRIDX*GRIDY-3;

int snake_head_index;
int snake_tail_index;

enum GAME_STATE game_state = GAME_START;

u_int8_t *init_snake(void){	

	grid = malloc(sizeof(Cell));
	NULLEXIT(grid, "failed allocating grid");

	memset(grid, 0, sizeof(Cell));
	//set top border to blank space
	memset(grid->is_node, BARIER, (GRIDX+2)*sizeof(u_int8_t));
	//set botom border to blank space
	memset((&(grid->is_node[(GRIDY+1)*(GRIDX+2)])), BARIER, (GRIDX+2)*sizeof(u_int8_t));
	
	for(int j = 0;j <= GRIDY;++j){
		grid->is_node[(GRIDX+2)*j] = BARIER;
		grid->is_node[(GRIDX+2)*j+GRIDX+1] = BARIER; 
	}


	//puts head in the middle of the grid
	snake_head_index =  (GRIDX+2)*((int)(ceil((float)(GRIDY+2)/2))) + 
		((int)(floor((float)(GRIDX+2)/2)));
	//puts tail in the middle of the grid behind the head
	snake_tail_index =  (GRIDX+2)*((int)(ceil((float)(GRIDY+2)/2))) + 
		((int)(floor((float)(GRIDX+2)/2))-2);

	grid->is_node[snake_head_index] = HEAD;
	grid->is_node[snake_tail_index+1] = BODY;
	grid->is_node[snake_tail_index] = TAIL;

	grid->next_node[snake_tail_index+1] = snake_head_index;
	grid->next_node[snake_tail_index] = snake_tail_index+1;

	//init random number generator
	srand(time(0));

	spawn_food();

	//srand(0);

	//for(int i = 0;i < ((GRIDX+2)*(GRIDY+2));++i){
		//printf("%d\t%d\n", (u_int8_t)(grid->is_node[i]), i);
	//}
	return grid->is_node;
}


void snake_move(enum DIRECTION direc){
	static enum DIRECTION direction = MOVE_RIGHT;
	unsigned int next_head_index;

	if((direc&1) != (direction&1)){
		direction = direc;
	}

	switch(direction){
		case MOVE_UP:
			next_head_index = snake_head_index-(GRIDX+2);
			break;

		case MOVE_DOWN:
			next_head_index = snake_head_index+(GRIDX+2);
			break;

		case MOVE_LEFT:
			next_head_index = snake_head_index-1;
			break;

		case MOVE_RIGHT:
			next_head_index = snake_head_index+1;
			break;
	}

	if(grid->is_node[next_head_index] == EMPTY || grid->is_node[next_head_index] == TAIL){
		//setup old tail to be emtpy
		grid->is_node[snake_tail_index] = EMPTY;
		//setup new head
		grid->is_node[next_head_index] = HEAD;
		//setup new tail
		grid->is_node[grid->next_node[snake_tail_index]] = TAIL;

		//setup last head to be body
		grid->is_node[snake_head_index] = BODY;
		grid->next_node[snake_head_index] = next_head_index;

		snake_tail_index = grid->next_node[snake_tail_index];

		//don't know if leaving this out will cause bugs :/
		//grid->next_node[snake_tail_index] = 0;

		snake_head_index = next_head_index;		

	}else if(grid->is_node[next_head_index] == FOOD){
		//setup new head
		grid->is_node[next_head_index] = HEAD;

		//setup last head to be body
		grid->is_node[snake_head_index] = BODY;
		grid->next_node[snake_head_index] = next_head_index;

		snake_head_index = next_head_index;		
		--grid_length;

		//deal with food
		spawn_food();	

		
	}else{
		game_state = GAME_OVER;
		free(grid);
		exit(EXIT_SUCCESS);
	}
}

void spawn_food(void){
	int i = 0;
	int j = 0;
	int index = rand()%grid_length;

	for(i = GRIDX+3;j <= index;++i){
		j += (grid->is_node[i] == EMPTY || grid->is_node[i] == FOOD);	
	}

	grid->is_node[i-1] = FOOD;
}

