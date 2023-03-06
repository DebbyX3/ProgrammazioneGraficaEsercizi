#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
    /*
    Esercizio 3:
    Output the vertex position to the fragment shader using the out keyword and set the fragment’s
    color equal to this vertex position (see how even the vertex position values are
    interpolated across the triangle). Once you managed to do this; try to answer the following
    question : why is the bottom - left side of our triangle black ?

    l'angolo in basso a sx è nero perchè la posizione passata come colore è negativa, quindi è renderizzata con il colore nero:
    0.5f, -0.5f, 0.0f bottom right = 0.5, 0, 0 = metà rosso
    -0.5f, -0.5f, 0.0f bottom left = 0, 0, 0 = nero
    0.0f,  0.5f, 0.0f top = 0, 0.5, 0 = metà verde*/

    gl_Position = vec4(aPos, 1.0);
    ourColor = aPos;
    
}