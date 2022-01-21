#version 430

// Entrées
layout(location=0) in vec3 aVertexPosition;
layout(location=1) in vec4 aVertexColor;
layout(location=2) in vec2 aVertexTexCoord;

// Variables uniformes
uniform mat4 uMVP;

// Sorties
out vec4 vColor;
out vec2 vTexCoord;

// Main
void main()
{
    vColor=aVertexColor;
    gl_Position=uMVP*vec4(aVertexPosition,1);
	vTexCoord=aVertexTexCoord;
}
