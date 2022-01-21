#version 430

// Entrées
in vec4 vColor;
in vec2 vTexCoord;

// Sorties
out vec4 fragColor;

// Variable d'échantillonage de l'unité de texture 0
layout(binding=0) uniform sampler2D samp;

// Main
void main()
{
	fragColor=texture(samp,vTexCoord)*vColor;
}
