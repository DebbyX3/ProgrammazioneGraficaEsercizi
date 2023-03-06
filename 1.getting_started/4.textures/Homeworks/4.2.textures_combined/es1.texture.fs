
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float opacityValue;

void main()
{	
	//Esercizio 1
	//make the happy face looking in the reverse direction and increase its opacity
	vec2 newTexCoord = vec2(-TexCoord.x, TexCoord.y);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, newTexCoord), 1.0);	
}