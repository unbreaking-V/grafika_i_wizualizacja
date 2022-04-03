#version 330 core

layout (location = 0) in vec3 position;//Устанавливаем позиция переменной с координатами в 0
layout (location = 1) in vec3 color;//Уставливаем позицию переменной с цветом в 1
layout (location = 2) in vec2 texCoord;// Устанавливаем позицию переменной с текстурой в 2

out vec3 ourColor;//Передаем цвет во фрагментный шейдер 
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position, 1.0);
    ourColor = color;//Устанавливаем значение цвета, полученной от вершинных данных 
    TexCoord = texCoord;
}
