#version 400

#define PI 3.1415926535897932384626433832795
#define DIRECTIONAL 0.0

#define PHONG 0.0
#define GOURAUD 1.0
#define FLAT 2.0

in vec4 normal;
in vec4 vertex;

struct LightSource{
	vec3 ambient, diffuse, specular;
	vec4 position; //if position[3] = 0 => light is directional
	vec3 spotDirection;
	float spotExponent, spotCutoff;
};

struct MaterialLighting{
	vec3 ambient, diffuse, specular;
	float shineness;
};

uniform LightSource u_lightSource;
uniform MaterialLighting u_materialLighting;



in vec3 instance_displacement;
in float instance_radius;
in mat4 instance_reorientationMatrix;


uniform float u_shadingModel;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;

//Expectator for Phong Model
uniform vec4  u_expectatorPosition;

out vec4 o_expectatorPosition;
out vec4 o_transformedNormal;
out vec4 fragPos;
out vec4 o_gouraudColor;
flat out vec4 o_flatColor;
out LightSource o_transformedLightSource;

void setLighting(in float shadingModel, in LightSource source, in MaterialLighting material, 
				 in vec3 specularExpectator, in vec3 normal, in vec3 vertice,
				 out vec4 gouraudColor, out vec4 flatColor);




void main(void) 
{
	
	o_expectatorPosition = u_projectionMatrix*u_modelViewMatrix *u_expectatorPosition;

	vec4 instanceVertex = vec4(instance_radius*(instance_reorientationMatrix*vertex).xyz + instance_displacement, 1.0);
	vec4 instanceNormal = transpose(inverse(instance_reorientationMatrix))*normal;

	fragPos = u_projectionMatrix*u_modelViewMatrix*instanceVertex;
	o_transformedNormal = transpose(inverse(u_projectionMatrix*u_modelViewMatrix))*instanceNormal;


	o_transformedLightSource = u_lightSource;
	o_transformedLightSource.position = vec4(vec3(u_projectionMatrix*u_modelViewMatrix*vec4(u_lightSource.position.xyz, 1.0)), u_lightSource.position.w);
	o_transformedLightSource.spotDirection = vec3(u_projectionMatrix*u_modelViewMatrix *vec4(u_lightSource.spotDirection, 1.0));




	setLighting(u_shadingModel, o_transformedLightSource, u_materialLighting,
				o_expectatorPosition.xyz, o_transformedNormal.xyz, fragPos.xyz,
				o_gouraudColor, o_flatColor);
	
	gl_Position = fragPos;
}

void setAmbientLight(in vec3 materialAmbientColor, in vec3 sourceAmbientColor, out vec3 resultAmbientColor);

void setDiffuseLight(in vec3 materialDiffuseColor, in vec3 sourceDiffuseColor, in float lightCategory, 
				     in vec3 lightLocation, in float spotExponent, in float spotCutoff, in vec3 normal, in vec3 vertice,
				     out vec3 resultDiffuseColor, out vec3 lightDirection);

void setSpecularLight(in vec3 materialSpecularColor, in vec3 sourceSpecularColor, in vec3 lightDirection, in vec3 vertice,
					  in vec3 expectatorPosition, in vec3 normal, in float shineness, out vec3 resultSpecularColor);

void setLighting(in float shadingModel, in LightSource source, in MaterialLighting material, 
				 in vec3 specularExpectator, in vec3 normal, in vec3 vertice,
				 out vec4 gouraudColor, out vec4 flatColor)
{
	vec3 ambient, diffuse, specular;
	vec3 lightDirection; //variable calculated in diffuse color computation and needed in specular
	setAmbientLight(material.ambient, source.ambient, ambient);
	setDiffuseLight(material.diffuse, source.diffuse, source.position[3], source.position.xyz, source.spotExponent, source.spotCutoff, normal, vertice,
		diffuse, lightDirection);
	setSpecularLight(material.specular, source.specular, lightDirection, vertice, specularExpectator, normal, material.shineness, specular);

	gouraudColor = vec4(ambient + diffuse + specular, 1.0);
	flatColor = vec4(ambient + diffuse + specular, 1.0);
}


void setAmbientLight(in vec3 materialAmbientColor, in vec3 sourceAmbientColor, out vec3 resultAmbientColor)
{
	resultAmbientColor = materialAmbientColor*sourceAmbientColor;
}

void setDiffuseLight(in vec3 materialDiffuseColor, in vec3 sourceDiffuseColor, in float lightCategory, 
				     in vec3 lightLocation, in float spotExponent, in float spotCutoff, in vec3 normal, in vec3 vertice,
				     out vec3 resultDiffuseColor, out vec3 lightDirection)
{
	float maxOpen = cos(PI*spotCutoff/180);
	lightDirection = vec3(0.0);
	if (lightCategory == DIRECTIONAL) //Is directional?
	{
		lightDirection = normalize(-lightLocation);
		resultDiffuseColor = max(dot(lightDirection, normal), 0.0)*materialDiffuseColor*sourceDiffuseColor;
	}
	else if (spotCutoff >= 0.0 && spotCutoff <= 90.0) // is spot?
	{
		lightDirection = normalize( lightLocation-vertice);

		if((dot(lightDirection, normal)) > maxOpen){
			resultDiffuseColor = pow(max(dot(lightDirection, normal), 0.0), spotExponent)*materialDiffuseColor*sourceDiffuseColor;
		}
		else{
			resultDiffuseColor = vec3(0.0);
			lightDirection = vec3(0.0);
		}
	}
	else if (spotCutoff == 180.0) //is this punctual?
	{
		lightDirection = normalize(lightLocation - vertice);
		if(dot(lightDirection, normal) > 0.0)
			resultDiffuseColor = dot(lightDirection, normal)*materialDiffuseColor*sourceDiffuseColor;
		else{
			resultDiffuseColor = vec3(0.0);
			
		}
	}
}

void setSpecularLight(in vec3 materialSpecularColor, in vec3 sourceSpecularColor, in vec3 lightDirection, in vec3 vertice,
					  in vec3 expectatorPosition, in vec3 normal, in float shineness, out vec3 resultSpecularColor)
{
	vec3 R = -lightDirection + 2*(dot(lightDirection, normal))*normal;
	vec3 V = normalize(expectatorPosition - vertice);

	if(dot(lightDirection, normal) > 0){
		resultSpecularColor = pow(max(dot(R,V), 0.0), shineness)*sourceSpecularColor*materialSpecularColor;
	}
	else
		resultSpecularColor = vec3(0.0);
}
