#include <iostream>
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include <cmath> 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window , double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

//Размер окна 
const GLuint WIDTH = 1920 , HEIGHT = 1080 ;
bool keys[1024];

//Camera , function LookAt 
Camera camera(glm::vec3(0.0f,0.0f,3.0f));
//координаты центра экрана 
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool firstMouse = true;

//Переменые deltaTime, для оптимизации скорости на разном железе
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGl", nullptr, nullptr);
    
    if (window == nullptr) 
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initalize GLEW" << std::endl;
        return -1;
    }

    // Initialize shaders
    initShaders();

    glViewport(0, 0, WIDTH, HEIGHT);

  float vertices[] = {
    -10.0f, -0.5f, -0.1f,  0.0f, 0.0f,
     10.0f, -0.5f, -0.1f,  1.0f, 0.0f,
     10.0f,  0.5f, -0.1f,  1.0f, 1.0f,
     10.0f,  0.5f, -0.1f,  1.0f, 1.0f,
    -10.0f,  0.5f, -0.1f,  0.0f, 1.0f,
    -10.0f, -0.5f, -0.1f,  0.0f, 0.0f,

    -10.0f, -0.5f,  0.1f,  0.0f, 0.0f,
     10.0f, -0.5f,  0.1f,  1.0f, 0.0f,
     10.0f,  0.5f,  0.1f,  1.0f, 1.0f,
     10.0f,  0.5f,  0.1f,  1.0f, 1.0f,
    -10.0f,  0.5f,  0.1f,  0.0f, 1.0f,
    -10.0f, -0.5f,  0.1f,  0.0f, 0.0f,

    -10.0f,  0.5f,  0.1f,  1.0f, 0.0f,
    -10.0f,  0.5f, -0.1f,  1.0f, 1.0f,
    -10.0f, -0.5f, -0.1f,  0.0f, 1.0f,
    -10.0f, -0.5f, -0.1f,  0.0f, 1.0f,
    -10.0f, -0.5f,  0.1f,  0.0f, 0.0f,
    -10.0f,  0.5f,  0.1f,  1.0f, 0.0f,

     10.0f,  0.5f,  0.1f,  1.0f, 0.0f,
     10.0f,  0.5f, -0.1f,  1.0f, 1.0f,
     10.0f, -0.5f, -0.1f,  0.0f, 1.0f,
     10.0f, -0.5f, -0.1f,  0.0f, 1.0f,
     10.0f, -0.5f,  0.1f,  0.0f, 0.0f,
     10.0f,  0.5f,  0.1f,  1.0f, 0.0f,

    -10.0f, -0.5f, -0.1f,  0.0f, 1.0f,
     10.0f, -0.5f, -0.1f,  1.0f, 1.0f,
     10.0f, -0.5f,  0.1f,  1.0f, 0.0f,
     10.0f, -0.5f,  0.1f,  1.0f, 0.0f,
    -10.0f, -0.5f,  0.1f,  0.0f, 0.0f,
    -10.0f, -0.5f, -0.1f,  0.0f, 1.0f,

    -10.0f,  0.5f, -0.1f,  0.0f, 1.0f,
     10.0f,  0.5f, -0.1f,  1.0f, 1.0f,
     10.0f,  0.5f,  0.1f,  1.0f, 0.0f,
     10.0f,  0.5f,  0.1f,  1.0f, 0.0f,
    -10.0f,  0.5f,  0.1f,  0.0f, 0.0f,
    -10.0f,  0.5f, -0.1f,  0.0f, 1.0f
    };

  glm::vec3 cubePositions[] =
  {
   glm::vec3( -0.2f,  0.0f,  -5.0f),
   glm::vec3( 0.0f,  0.0f, 15.0f),
   glm::vec3( -9.8f,0.0f,5.0f ),
   glm::vec3 (9.8f,0.0,5.0f)
   };

    

    GLuint VBO,VAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    
    //Копируем наши вершини в буфер для OpenGL
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);


    //Атрибут с координатами  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Атрибут с текстурами 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    GLuint texture1;


    // ==============
    // Texture 1
    // ==============

    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);

    //Задаем параметры нашей текстуры 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);

    //Устанавливаем филтрацию текстур 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    // Загружаем, создаем текстуру и генерируем MIP-карты
    int width, height;
    unsigned char* image = SOIL_load_image("texture/stone_1.jpg", &width, &height, 0 , SOIL_LOAD_RGB );
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
   

    while (!glfwWindowShouldClose(window))
    { 

     // Рассчитать дельта-время текущего кадра
         GLfloat currentFrame = glfwGetTime();
         deltaTime = currentFrame - lastFrame;
         lastFrame = currentFrame;

        // Get callback actions
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // Обработка событий 
        glfwPollEvents();
        
        //Включаем функцию LookAt
        do_movement();

        // Очищаем буфер цвета 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Буфер глубины 
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Связываем текстуры с помощью текстурных блоков
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(basicShader->shaderProgram,"ourTexture1"),0);
        glActiveTexture(GL_TEXTURE1);
        
        //Aктивируем шейдерную программу
        basicShader->use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();

        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, 0.0f, glm::vec3(0.5f,1.0f,0.0f));

        view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
        projection = glm::perspective(camera.Zoom,(GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        
        GLint modelLoc = glGetUniformLocation(basicShader->shaderProgram,"model");
        GLint viewLoc = glGetUniformLocation(basicShader->shaderProgram,"view");
        GLint projLoc = glGetUniformLocation(basicShader->shaderProgram,"projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        //Рисуем 
        glBindVertexArray(VAO);
         
        for(GLuint i = 0; i < 4; i++)
         {
         glm::mat4 model = glm::mat4(1.0f);
         model = glm::translate(model,cubePositions[i]);
         GLfloat angle = 1.57f;
         if ( i > 1 ){
             model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
         }
         glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
         glDrawArrays(GL_TRIANGLES,0,36);
         }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }


    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);


    // Remove shaders
    freeShaders();
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement()
{
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD,deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD,deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT,deltaTime);

}


void mouse_callback(GLFWwindow* window, double xpos , double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    //Вычисляем смещение мыши с момента последнего кадры 
    GLfloat xoffset = xpos - lastX;
    //Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
