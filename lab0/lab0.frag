/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in float shade;
in vec2 TexCoord;

out vec4 out_Color;

uniform float t;

void main(void)
{
float a = sin(TexCoord.s * 30.0 + t)/2.0 + 0.5;
float b = sin(TexCoord.t * 30.0 * (1.0+sin(t/4.0)))/2.0 + 0.5;
out_Color = vec4(a*shade, b*shade, 0.8*shade, 1.0);
	//out_Color=vec4(shade,shade,shade,1.0);
}
