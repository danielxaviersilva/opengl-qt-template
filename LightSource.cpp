#include "LightSource.h"


LightSource::LightSource(std::string shadingModelName, std::string lightPositionName, std::string spotDirectionName,
                         std::string spotExponentName, std::string spotCutoffName, std::string lightAmbientColorName,
                         std::string lightDiffuseColorName, std::string lightSpecularColorName, std::string specularExpectatorPositionName)
{
    m_shadingModelName = shadingModelName;
    m_lightPositionName = lightPositionName;
    m_spotDirectionName = spotDirectionName;
    m_spotExponentName = spotExponentName;
    m_spotCutoffName = spotCutoffName;
    m_lightAmbientColorName = lightAmbientColorName;
    m_lightDiffuseColorName = lightDiffuseColorName;
    m_lightSpecularColorName = lightSpecularColorName;
    m_specularExpectatorPositionName = specularExpectatorPositionName;

    setToDefault();
}

void LightSource::setToDefault()
{
    m_shadingModel = PHONG;
    m_position      = glm::vec4(-1.0f,0.0,.0,1.0);
    m_spotDirection = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    m_spotExponent  = 5.0f;
    m_spotCutoff    = 180.0f;
    m_ambient  = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    m_diffuse  =  glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    m_specular = glm::vec4(.3f, .3f, .3f, 1.0f);
    m_specularExpectatorPosition = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
}
void LightSource::updateLightSource(Shader *program)
{
    program->useProgram();

    if(program == nullptr){
        std::cerr << "LightSource::LightSource(): program not loaded into the class" << std::endl;
        return;}
    GLint shadingModelLoc                  = program->getUniformLocation(m_shadingModelName              );
    GLint lightPositionLoc                 = program->getUniformLocation(m_lightPositionName             );
    GLint spotDirectionLoc                 = program->getUniformLocation(m_spotDirectionName             );
    GLint spotExponentLoc                  = program->getUniformLocation(m_spotExponentName              );
    GLint spotCutoffLoc                    = program->getUniformLocation(m_spotCutoffName                );
    GLint lightAmbientColorLoc             = program->getUniformLocation(m_lightAmbientColorName         );
    GLint lightDiffuseColorLoc             = program->getUniformLocation(m_lightDiffuseColorName         );
    GLint lightSpecularColorLoc            = program->getUniformLocation(m_lightSpecularColorName        );
    GLint specularExpectatorPositionLoc    = program->getUniformLocation(m_specularExpectatorPositionName);

    if (
          shadingModelLoc       == -1      ||
          lightPositionLoc      == -1      ||
          spotDirectionLoc      == -1      ||
          spotExponentLoc       == -1      ||
          spotCutoffLoc         == -1      ||
          lightAmbientColorLoc  == -1      ||
          lightDiffuseColorLoc  == -1      ||
          lightSpecularColorLoc == -1      ||
          specularExpectatorPositionLoc == -1
        )
            std::cerr << "At least one variable wasnt found on the shader program, please send the arguments as:" << std::endl
                  << "(shadingModelName, lightPositionName,spotDirectionName, spotExponentName, spotCutoffName," << std::endl
                   << "lightAmbientColorName, lightDiffuseColorName, lightSpecularColorName, specularExpectatorPositionName)" << std::endl;

    glUniform1f (shadingModelLoc              ,  m_shadingModel                      );
    glUniform4fv(lightPositionLoc             ,1,glm::value_ptr(m_position     ));
    glUniform3fv(spotDirectionLoc             ,1,glm::value_ptr(m_spotDirection     ));
    glUniform1f (spotExponentLoc              ,  m_spotExponent                      );
    glUniform1f (spotCutoffLoc                ,  m_spotCutoff                        );
    glUniform3fv(lightAmbientColorLoc         ,1,glm::value_ptr(m_ambient));
    glUniform3fv(lightDiffuseColorLoc         ,1,glm::value_ptr(m_diffuse));
    glUniform3fv(lightSpecularColorLoc        ,1,glm::value_ptr(m_specular));
    glUniform4fv(specularExpectatorPositionLoc,1,glm::value_ptr(m_specularExpectatorPosition));
}

void LightSource::setShadingModel(const ShadingMode &shadingModel)
{
    m_shadingModel = shadingModel;
}

void LightSource::setPosition(const glm::vec4 &position)
{
    m_position = position;
}

void LightSource::setSpotDirection(const glm::vec3 &spotDirection)
{
    m_spotDirection = spotDirection;
}

void LightSource::setSpotExponent(float spotExponent)
{
    m_spotExponent = spotExponent;
}

void LightSource::setSpotCutoff(float spotCutoff)
{
    m_spotCutoff = spotCutoff;
}

void LightSource::setAmbient(const glm::vec4 &ambient)
{
    m_ambient = ambient;
}

void LightSource::setDiffuse(const glm::vec4 &diffuse)
{
    m_diffuse = diffuse;
}

void LightSource::setSpecular(const glm::vec4 &specular)
{
    m_specular = specular;
}

void LightSource::setSpecularExpectatorPosition(const glm::vec4 &specularExpectatorPosition)
{
    m_specularExpectatorPosition = specularExpectatorPosition;
}

//Pode ser lento, não me preocupei de pegar a variável e jogar somente ele no shader.
void LightSource::setShadingModel(const ShadingMode &shadingModel, Shader* program)
{
    m_shadingModel = shadingModel;
    updateLightSource(program);
}

void LightSource::setPosition(const glm::vec4 &position, Shader* program)
{
    m_position = position;
    updateLightSource(program);
}

void LightSource::setSpotDirection(const glm::vec3 &spotDirection, Shader* program)
{
    m_spotDirection = spotDirection;
    updateLightSource(program);
}

void LightSource::setSpotExponent(float spotExponent, Shader* program)
{
    m_spotExponent = spotExponent;
    updateLightSource(program);
}

void LightSource::setSpotCutoff(float spotCutoff, Shader* program)
{
    m_spotCutoff = spotCutoff;
    updateLightSource(program);
}

void LightSource::setAmbient(const glm::vec4 &ambient, Shader* program)
{
    m_ambient = ambient;
    updateLightSource(program);
}

void LightSource::setDiffuse(const glm::vec4 &diffuse, Shader* program)
{
    m_diffuse = diffuse;
    updateLightSource(program);
}

void LightSource::setSpecular(const glm::vec4 &specular, Shader* program)
{
    m_specular = specular;
    updateLightSource(program);
}

void LightSource::setSpecularExpectatorPosition(const glm::vec4 &specularExpectatorPosition, Shader* program)
{
    m_specularExpectatorPosition = specularExpectatorPosition;
    updateLightSource(program);
}


