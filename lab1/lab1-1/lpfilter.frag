#version 150

uniform sampler2D texUnit;
uniform float texSize;
in vec2 texCoord;
out vec4 fragColor;

void main(void)
{
	float offset = 1.0 / 512.0;
	vec4 c = texture(texUnit, texCoord);
	vec4 l = texture(texUnit, texCoord +
	vec2(offset, 0.0));
	vec4 r = texture(texUnit, texCoord +
	vec2(-offset, 0.0));
	fragColor = (c + c + l + r) * 0.25;
}
