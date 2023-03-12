#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

//uniform sampler2D texture0;

void main ()
{
  //FragColor = texture(texture0, TexCoord);
  FragColor = vec3(0.f, 1.f, 0.f, 1.f);
}
