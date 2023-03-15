#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tCord;

out vec2 textureCoord;

uniform mat4 TG;

void main()
{
    gl_Position = TG * vec4(aPos, 1.0);
    //gl_Position = TG * vec4(aPos, 1.0);
    textureCoord = tCord;
}
