#version 330 core

in vec3 fragCol;
out vec4 color;

uniform float alpha;

void main()
{
        color = vec4(fragCol, alpha);
}
