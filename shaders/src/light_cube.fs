#version 330 core

out vec4 FragColor;

uniform vec3 lightDiffuse;

void main()
{
    FragColor = vec4(lightDiffuse, 1.0);
}