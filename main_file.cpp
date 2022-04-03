#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shaderprogram.h"
#include <cmath> 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGl", nullptr, nullptr);
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

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLfloat vertices[] = {
        0.5f,-0.5f, 0.0f,   1.0f,0.0f,0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,
        0.0f, 0.5f, 0.0f,   0.0f,0.0f,1.0f
        };

    GLuint VBO,VAO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    //glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    
    //Копируем наши вершини в буфер для OpenGL
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

     //Копируем наши индексы в буфер для OpenGL
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   //glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(vertices),indices,GL_STATIC_DRAW);

    //Атрибут с координатами  
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Атрибут с цветом
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window))
    {
        // Get callback actions
        glfwSetKeyCallback(window, key_callback);

        // Обработка событий 
        glfwPollEvents();
        
        // Очищаем буфер цвета 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
       
        //Aктивируем шейдерную программу
        basicShader->use();

        //Обновляем цвет формы 
       /* GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue)/2 )+0.5;
        GLint vertexColorLocation = glGetUniformLocation(basicShader->shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
        */

        //Рисуем 
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);

        //Режим Wireframe 

        //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glBindVertexArray(0);
    

        glfwSwapBuffers(window);
    }


    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);

    // Remove shaders
    freeShaders();
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
