#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

//Esercizio 0
uniform float greenValue;

out vec3 ourColor;

void main()
{
    /*
    Esercizio 0:
    Fai variare il verde del triangolo in base al tempo con un uniform, ma usando le classi.
    Nota: mandare solo il verde al vertex shader e ricostruire il vettore del colore
    */

    gl_Position = vec4(aPos, 1.0);

    //versione dove cambia solo il verde gli altri colori stanno fermi
    //ourColor = vec3(aColor.x, greenValue, aColor.z);

    //versione dove si vede solo verde
    ourColor = vec3(0.0f, greenValue, 0.0f);
}