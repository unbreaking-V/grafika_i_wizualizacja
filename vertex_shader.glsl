#version 330 core

layout (location = 0) in vec3 position;//Устанавливаем позиция переменной с координатами в 0
layout (location = 2) in vec2 texCoord;// Устанавливаем позицию переменной с текстурой в 2

out vec2 TexCoord;

uniform mat4 model;//Матрица модели 
uniform mat4 view;//Матрица вида
uniform mat4 projection;//Матрица проекции


void main()
{
    gl_Position = projection * view * model* vec4(position, 1.0f);
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
