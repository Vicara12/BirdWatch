#version 330 core

in vec2 textureCoord;
out vec4 FragColor;

uniform sampler2D texture0;

void main ()
{
  vec4 color = texture(texture0, textureCoord);
  if (color.a > 0.8)
    FragColor = color;
  else
    discard;
}
