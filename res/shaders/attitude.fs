#version 330 core

in vec2 texCord;
out vec4 FragColor;

uniform mat4 texTransform;
uniform sampler2D texture0;
uniform sampler2D border;
uniform sampler2D statics;

void main ()
{
  // the border acts as a kind of sencil buffer whose alpha channel
  // indicates where to draw
  vec4 border_color = texture(border, texCord);
  bool in_border = (border_color.a < 0.1);
  if (in_border) {
    vec4 statics_color = texture(statics, texCord);
    vec4 realTexCoord = texTransform * vec4(texCord.x, texCord.y, 0, 1);
    vec4 bgn_color = texture(texture0, realTexCoord.xy);
    vec4 mixed_color = mix(bgn_color, statics_color, statics_color.w);
    mixed_color.a = (1-border_color.a);
    FragColor = mixed_color;
  } else {
    discard;
  }
}
