#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
uniform mat4 matrix;
uniform mat4 bonePos0;
uniform mat4 bonePos1;
uniform mat4 boneRot0;
uniform mat4 boneRot1;
uniform float weight;

out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

// Uppgift 3: Soft-skinning på GPU
//
// Flytta över din implementation av soft skinning från CPU-sidan
// till vertexshadern. Mer info finns på hemsidan.

void main(void)
{
	mat4 invmBone0 = inverse(bonePos0);
	mat4 invmBone1 = inverse(bonePos1);
	mat4 boneMat0 = bonePos0 * boneRot0;
	mat4 boneMat1 = bonePos1 * boneRot1;
	vec3 position = vec3(((boneMat0 * invmBone0) * vec4(in_Position, 1.0)) * in_TexCoord.x + ((boneMat1 * invmBone1) * vec4(in_Position, 1.0)) * in_TexCoord.y); 
	
	// transformera resultatet med ModelView- och Projection-matriserna
	gl_Position = matrix * vec4(position,1.0);
	
	// sätt röd+grön färgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);

	// Lägg på en enkel ljussättning på vertexarna 	
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}

