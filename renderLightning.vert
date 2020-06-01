#version 400

#define PI 3.1415926535897932384626433832795

uniform float u_shading_model;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;

//Light Properties
uniform vec4  u_light_location;
uniform vec4  u_spot_direction;
uniform float u_spot_exponent;
uniform float u_spot_cutoff;


uniform vec4  u_Light_ambientColor;
uniform vec4  u_Light_diffuseColor;
uniform vec4  u_Light_specularColor;


//Material Properties
uniform vec4  u_Mat_ambientColor;
uniform vec4  u_Mat_diffuseColor;
uniform vec4  u_Mat_specularColor;
uniform float u_shineness;


//Expectator for Phong Model
uniform vec4  u_expectator_position;


//outs
out float shading_model;

out vec4 light_location;
out vec4 spot_direction;
out float spot_exponent;
out float spot_cutoff;


out vec4 Light_ambientColor;
out vec4 Light_diffuseColor;
out vec4 Light_specularColor;

out vec4  Mat_ambientColor;
out vec4  Mat_diffuseColor;
out vec4  Mat_specularColor;
out float shineness;

out vec4 expectator_position;

//vertices characteristics
out vec4 normal;
out vec4 frag_pos;
out vec4 gouraud_color;
flat out vec4 flat_color;


in vec4 v_normal;
in vec4 vertex;



void main(void) 
{

	//In outs to frag shader
	light_location = u_projectionMatrix*u_modelViewMatrix *u_light_location;
	spot_direction = u_projectionMatrix*u_modelViewMatrix *u_spot_direction;
	expectator_position = u_projectionMatrix*u_modelViewMatrix *u_expectator_position;

	/*light_location = u_light_location;
	spot_direction = u_spot_direction;
	expectator_position = u_expectator_position;*/

	spot_exponent = u_spot_exponent;
	spot_cutoff = u_spot_cutoff;
	Light_ambientColor = u_Light_ambientColor;
	Light_diffuseColor = u_Light_diffuseColor;
	Light_specularColor = u_Light_specularColor;
	Mat_ambientColor = u_Mat_ambientColor;
	Mat_diffuseColor = u_Mat_diffuseColor;
	Mat_specularColor = u_Mat_specularColor;
	shineness = u_shineness;


    
	shading_model = u_shading_model;

	vec3 ambient_light;


	//Diffuse Component
	vec3 light_direction;
	vec3 norm_spot_direction = normalize(spot_direction.xyz);
	vec3 norm_normal;// = normalize(normal.xyz);

	float max_open = cos(PI*spot_cutoff/180);
	vec3 diffuse_light;

	//phong specular component
	vec3 R;
	vec3 V;
	vec3 phong_light;

	//vertex related variables
	vec4 xvertex = u_modelViewMatrix *vertex;
	//frag_pos = //vertex;//u_projectionMatrix *xvertex;
	frag_pos = u_projectionMatrix *xvertex;


	//normal = transpose(inverse(u_projectionMatrix*u_modelViewMatrix))*v_normal;
    normal = u_projectionMatrix*u_modelViewMatrix*v_normal;
    //normal = v_normal;
	norm_normal = normalize(normal.xyz);
	if(shading_model == 1.0 || shading_model == 2.0)
	{
		ambient_light =  Light_ambientColor.rgb*Mat_ambientColor.rgb;
		if (light_location[3] == 0.0) //Is directional?
		{
			light_direction = normalize(-light_location.xyz);
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
			light_direction = normalize(light_location.xyz - frag_pos.xyz);
			diffuse_light = max(dot(light_direction, norm_normal), 0.0)*Light_diffuseColor.rgb*Mat_diffuseColor.rgb;
		}

		//Specular Component
			R = -light_direction + 2*(dot(light_direction, norm_normal))*norm_normal;
			V = normalize(expectator_position.xyz - frag_pos.xyz);

			if(dot(light_direction, norm_normal) > 0)
				phong_light = pow(max(dot(R,V), 0.0), shineness)*Light_specularColor.rgb*Mat_specularColor.rgb;
			else
				phong_light = vec3(0.0);

		if (shading_model == 1.0)
		{
			//gouraud_color = vec4(0,1,0,1);
			gouraud_color = vec4(ambient_light + diffuse_light + phong_light, 1.0);
		}
		else if(shading_model == 2.0)
		{
			flat_color = vec4( ambient_light + diffuse_light + phong_light, 1.0);
			//flat_color = vec4(0,0,1,1);
		}
	}

	
	//gl_Position = u_projectionMatrix * xvertex;
	gl_Position = frag_pos;



}
