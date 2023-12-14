#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) Write this function
        
        // desarialize the pipeline state is getting called in the material deserialize
        // the properties are read and we just need to call setup
        pipelineState.setup();
        // same for shader
        shader->use();
        
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        Material::setup();         // call the setup of its parent
        shader->set("tint", tint); // set the tint
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        TintedMaterial::setup();         // call the setup of its parent
        shader->set("alphaThreshold", alphaThreshold); // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
        
        //Specifies which texture unit to make active
        glActiveTexture(GL_TEXTURE0);
        
        if (texture)
            texture->bind(); // check if the texture is not null then bind it
        else
            Texture2D::unbind(); //if null unbind //static method
        if (sampler)
            sampler->bind(0); // check if sampler is not null then bind it
        else
            Sampler::unbind(0); //if null unbind //static method
        shader->set("tex", 0); // send the unit number to the uniform variable "tex"
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }


    void LitMaterial::setup() const
    {
        // calling on the textured material setup function
        // since lit material inherits from textured material
        Material::setup();
        // going through the texture maps defined in the .hpp
        // binding the texture and sampler
        // and then setting the values in the shader file
        if (albedo)
        {
            glActiveTexture(GL_TEXTURE0);
            albedo->bind();
            if (sampler)
                sampler->bind(0);
            shader->set("material.albedo", 0);
        }
        if (specular)
        {
            glActiveTexture(GL_TEXTURE1);
            specular->bind();
            if (sampler)
                sampler->bind(1);
            shader->set("material.specular", 1);
        }
        if (ambient_occlusion)
        {
            glActiveTexture(GL_TEXTURE2);
            ambient_occlusion->bind();
            if (sampler)
                sampler->bind(2);
            shader->set("material.ambient_occlusion", 2);
        }
        if (roughness)
        {
            glActiveTexture(GL_TEXTURE3);
            roughness->bind();
            if (sampler)
                sampler->bind(3);
            shader->set("material.roughness", 3);
        }
        if (emissive)
        {
            glActiveTexture(GL_TEXTURE4);
            emissive->bind();
            if (sampler)
                sampler->bind(4);
            shader->set("material.emissive", 4);
        }
        glActiveTexture(GL_TEXTURE0);
    }

    // the deserializer function to read data from the json object
    void LitMaterial::deserialize(const nlohmann::json &data)
    {
        // calling on the textured material function
        Material::deserialize(data);

        if (!data.is_object())
        {
            return;
        }
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));

        // at last get the sampler
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }
    

}