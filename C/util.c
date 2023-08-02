#include "util.h"
#include "main.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <sys/types.h>

extern int graphicsRuntime(GLFWwindow *window);
extern void graphicsTerminate();
extern void resizeCallback(GLFWwindow *window, int width, int height);
extern void inputCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
extern void render();


int graphicsInit(){

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Snake Game", NULL, NULL);
  if(window == NULL){
    graphicsTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    fprintf(stderr, "Failed to initialize GLAD");
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
	glfwSetKeyCallback(window, inputCallback);


  #ifdef DEBUG
  fprintf(stdout, "Window Initialized\n");
  #endif

  if(graphicsRuntime(window)){
    return -1;
  }
  return 0;
}


void graphicsTerminate(){
  glfwTerminate();
  #ifdef DEBUG
  fprintf(stdout, "Window terminated\n");
  #endif
}


int graphicsRuntime(GLFWwindow *window){

  float vertices[] = {
    -0.5f, -0.5f, 0.0f,  //bottom left
    0.5f, -0.5f, 0.0f,  //bottom right
    -0.5f, 0.5f, 0.0f,  //top left 
    0.5f, 0.5f, 0.0f    //top right
  };

  float colors[] = {
    1.0, 0.0f, 0.0f,  //bottom left
    0.0, 1.0f, 0.0f,//bottom right
    0.0, 0.0f, 1.0f,//top left 
    1.0, 1.0f, 1.0f //top right
  };

  u_int indices[] = {
    2, 0, 1,
    2, 3, 1
  };

  u_int VAO;
  glGenVertexArrays(1, &VAO);

  u_int VBO[2];
  glGenBuffers(2, VBO);

  u_int EBO;
  glGenBuffers(1, &EBO);

  const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 color;\n"
    "void main(){\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   color = aColor;\n"
    "}\n";

  const char *fragmentShaderSource = 
    "#version 330 core\n"
    "in vec3 color;\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(color, 1.0);\n"
    "}\n";

  u_int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  u_int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  int success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success){
    char err[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, err);
    fprintf(stderr, "Vertex shader error: %s\n", err);
    return -1;
  }

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success){
    char err[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, err);
    fprintf(stderr, "fragment shader error: %s\n", err);
    return -1;
  }

  u_int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success){
    char err[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, err);
    fprintf(stderr, "shader program link error: %s\n", err);
    return -1;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);


  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);

  while(!glfwWindowShouldClose(window)){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  graphicsTerminate();
  return 0;
}

void resizeCallback(GLFWwindow *window, int width, int height){
  #ifdef DEBUG
  fprintf(stdout, "resizeCallback called\n");
  #endif
  glViewport(0, 0, width, height);
}

void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
  #ifdef DEBUG
  fprintf(stdout, "inputCallback called\n");
  #endif
  switch(key){
    case GLFW_KEY_Q:
      glfwSetWindowShouldClose(window, 1);
      break;
  }
}
