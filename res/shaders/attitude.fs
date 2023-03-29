#version 330 core

in vec2 texCord;
out vec4 FragColor;

uniform mat4 texTransform;
uniform sampler2D texture0;
uniform sampler2D border;
uniform sampler2D statics;

void main ()
{
  vec4 statics_color = texture(statics, texCord);
  vec4 realTexCoord = texTransform * vec4(texCord.x, texCord.y, 0, 1);
  vec4 bgn_color;
  // in the upper part of the attitude indicator there is
  // a semi-circle that does not have numbers, which displays
  // a ttriangle with the angle, this makes the numberless
  // strip. If the point is north of the non number display
  // area, take the side color (blue, brown or white)
  if (texCord.y > 0.7 && length(texCord-vec2(0.5, 0.5)) > 0.40)
    bgn_color = texture(texture0, vec2(0,realTexCoord.y));
  else
    bgn_color = texture(texture0, realTexCoord.xy);
  vec4 mixed_color = mix(bgn_color, statics_color, statics_color.a);
  // the border acts as a kind of sencil buffer whose alpha channel
  // indicates where to draw
  vec4 border_color = texture(border, texCord);
  FragColor = vec4(mixed_color.rgb, 1-border_color.a);
}
