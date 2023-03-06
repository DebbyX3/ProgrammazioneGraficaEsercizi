#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float horOffset;

out vec3 ourColor;

void main()
{
    /*
    Esercizio 2:
    Specify a horizontal offset via a uniform and move the triangle to the right side of the screen 
    in the vertex shader using this offset value*/
    
    vec3 newPos = vec3(aPos.x + horOffset, aPos.y, aPos.z);
    gl_Position = vec4(newPos, 1.0);    
    ourColor = aColor;
}