#version 330 core

in vec2 textureCoord;
out vec4 FragColor;

//uniform mat4 texTransf;
uniform sampler2D texture0;

void main ()
{
  //vec4 realTexCoord = texTransf * vec4(textureCoord.x, textureCoord.y, 0, 1);
  //FragColor = texture(texture0, realTexCoord.xy);
  FragColor = texture(texture0, textureCoord);
}
