#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
	vec4 bs = -(texture(texUnit,outTexCoord) - texture(texUnit,vec2(outTexCoord.s+(1.0/256.0), outTexCoord.t)));
	vec4 bt = -(texture(texUnit,outTexCoord) - texture(texUnit,vec2(outTexCoord.s, outTexCoord.t+(1.0/256.0))));

	mat3 Mvt = mat3(Ps,Pt,out_Normal);	

	vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates

	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions

	light = Mvt* light;	

	vec3 normal = vec3(bs.r*10.0,bt.r*10.0,1.0);
	// Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.
	out_Color = vec4( dot(normal, light));// * texture(texUnit, outTexCoord);*/

/* //1-2A!!!!!
	vec4 bs = -(texture(texUnit,outTexCoord) - texture(texUnit,vec2(outTexCoord.s+(1.0/256.0), outTexCoord.t)));
	vec4 bt = -(texture(texUnit,outTexCoord) - texture(texUnit,vec2(outTexCoord.s, outTexCoord.t+(1.0/256.0))));

	vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates

	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions


	vec3 normal = normalize(out_Normal + (vec3(5.0*bs)*Ps) + (vec3(5.0*bt)*Pt));
	//vec3 normal = normalize(out_Normal);
	// Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.
	out_Color = vec4( dot(normal, light));// * texture(texUnit, outTexCoord);*/
}
