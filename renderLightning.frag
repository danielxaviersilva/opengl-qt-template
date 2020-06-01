#version 400

#define PI 3.1415926535897932384626433832795

in float shading_model;

//Light Properties
in vec4 light_location;
in vec4 spot_direction;
in float spot_exponent;
in float spot_cutoff;


in vec4 Light_ambientColor;
in vec4 Light_diffuseColor;
in vec4 Light_specularColor;

//Material Properties
in vec4  Mat_ambientColor;
in vec4  Mat_diffuseColor;
in vec4  Mat_specularColor;
in float shineness;

//Expectator for Phong Model
in vec4 expectator_position;

//vertices characteristics
in vec4 normal;
in vec4 frag_pos;


in vec4 gouraud_color;
flat in vec4 flat_color;

//output color
out vec4 fColor;

void main (void) 
{
	//Ambient Component
	vec3 ambient_light =  Light_ambientColor.rgb*Mat_ambientColor.rgb;


	//Diffuse Component
	vec3 light_direction;
	vec3 norm_spot_direction = normalize(spot_direction.xyz);
	vec3 norm_normal = normalize(normal.xyz);
	

	float max_open = cos(PI*spot_cutoff/180);
	vec3 diffuse_light;

	vec3 R;
	vec3 V;
	vec3 phong_light;


	if(shading_model == 0.0)
	{
		if (light_location[3] == 0.0) //Is directional?
		{
			light_direction = normalize(light_location.xyz);
			diffuse_light = max( dot(light_direction, norm_normal), 0.0)*Light_diffuseColor.rgb*Mat_diffuseColor.rgb;
		}
		else if (spot_cutoff >= 0.0 && spot_cutoff <= 90.0) // is spot?
		{
			light_direction = normalize( light_location.xyz - frag_pos.xyz);

			if(abs(dot(light_direction, norm_normal)) > max_open)
				diffuse_light = pow(max(dot(light_direction, norm_normal), 0.0), spot_exponent)*Light_diffuseColor.rgb*Mat_diffuseColor.rgb;
			else  
				diffuse_light = vec3(0.0);
		}
		else if (spot_cutoff == 180.0) //is this punctual?
		{
			light_direction = normalize( light_location.xyz - frag_pos.xyz);
			diffuse_light = max(dot(light_direction, norm_normal), 0.0)*Light_diffuseColor.rgb*Mat_diffuseColor.rgb;
		}

		//Specular Component
		R = -light_direction + 2*(dot(light_direction, norm_normal))*norm_normal;
		V = -normalize(expectator_position.xyz - frag_pos.xyz);

		if(dot(light_direction, norm_normal) > 0.0)
			phong_light = pow(max(dot(R,V), 0.0), shineness)*Light_specularColor.rgb*Mat_specularColor.rgb;
		else
			phong_light = vec3(0.0);

		

		//phong_light = ks*pow(dot(normalize(light_direction + V), norm_normal), n)*color.rgb*light_color.rgb;
		
		fColor = vec4( ambient_light + diffuse_light + phong_light, 1.0);
	}
	else if(shading_model == 1.0)
	{
		fColor = gouraud_color;
	}
	else if(shading_model == 2.0)
	{
		fColor = flat_color;
	}
//        fColor = vec4(1.0,0.0,0.0,1.0);


}
