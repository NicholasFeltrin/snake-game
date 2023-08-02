#include "main.h"
#include "util.h"

int main(){
  if(graphicsInit()){
    fprintf(stderr, "Exited with error\n");
    return -1;
  }


  return 0;
}



