#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main()
{
    //nota: passiamo al view quindi lo aggiungiamo alla moltiplicazione di fragpos e anche normal
    FragPos = vec3(model * view * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model * view))) * aNormal;  

    //nota: visto che dobbiamo portare la luce dal sistema mondo al sistema view, 
    //me la faccio passare, la trasformo e poi la do al fragment shader
    LightPos = vec3(vec4(lightPos, 1.0) * view); //è già nel mondo
    
    gl_Position = projection * vec4(FragPos, 1.0); //attenzione, tolgo la moltiplicazione per la view perchè l'ho già fatta!!
}