#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tCord;

out vec2 textureCoord;

uniform mat4 TG, PreTG;

void main()
{
    gl_Position = TG * PreTG * vec4(aPos.x, aPos.yz, 1.0);
    textureCoord = tCord;
}
