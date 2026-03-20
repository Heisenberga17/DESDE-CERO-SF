#version 300 es
precision highp float;

in vec3 fragPos;
in vec3 fragNor;
out vec4 color;

void main()
{
        color = vec4(fragNor, 1.0);
}
