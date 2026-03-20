#version 300 es
layout(location = 0) in vec4 vertPos;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec3 fragPos;

void main()
{
	gl_Position = P * V * M * vertPos;
	fragPos = gl_Position.xyz;
	gl_PointSize = 50.0;
}
