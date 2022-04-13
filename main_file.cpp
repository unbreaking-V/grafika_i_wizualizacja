#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
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
    // Позиции          // Цвета             // Текстурные координаты
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
};


     GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    GLuint VBO,VAO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    

    glBindVertexArray(VAO);
    
    //Копируем наши вершини в буфер для OpenGL
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

     //Копируем наши индексы в буфер для OpenGL
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    GLuint texture1, texture2;

    glGenTextures(1,&texture1); 
    glBindTexture(GL_TEXTURE_2D,texture1);

    //Подключаем текстуры  
    unsigned char* image = SOIL_load_image("texture/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);

     image = SOIL_load_image("texture/awesomeface.png",&width,&height,0,SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Атрибут с координатами  
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //Атрибут с цветом
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    //Атрибут с текстурой
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);


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
       
        //Добовляем текстуры 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(basicShader->shaderProgram,"ourTexture2"),0);    
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2);
        glUniform1i(glGetUniformLocation(basicShader->shaderProgram,"ourTexture2"),1);
        
      
        //Aктивируем шейдерную программу
        basicShader->use();

        //Обновляем цвет формы 
       /* GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue)/2 )+0.5;
        GLint vertexColorLocation = glGetUniformLocation(basicShader->shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
        */

        //Рисуем
        //Cоздаем трансформацию  
        glm::mat4 trans;
        trans = glm::translate(trans,glm::vec3(0.4f,0.0f,-0.5f));
        trans = glm::rotate(trans,(GLfloat)glfwGetTime() * 1.0f , glm::vec3(0.0f,0.0f, 1.0f));
       // trans = glm::scale(trans,glm::vec3(0.5,0.5,0.5));

        // Получаем юниформ-позицию матрицы и устанавливаем матрицу
         GLuint transformLoc = glGetUniformLocation(basicShader -> shaderProgram,"transform");
         glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));
     
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
      
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans,glm::vec3(0.1f,0.0f,-0.6f));
        GLfloat scaleAmount = sin(glfwGetTime());
        trans = glm::scale(trans,glm::vec3(scaleAmount,scaleAmount,scaleAmount));
        glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));

       

        
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
