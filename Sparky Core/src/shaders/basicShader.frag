#version 330 core
out vec4 FragColor;

in vec3 FColor;

void main()
{
    FragColor = vec4(FColor, 1.0);   
}  