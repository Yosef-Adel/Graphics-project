#pragma once

#include "../ecs/component.hpp"
#include "../ecs/transform.hpp"
#include <unordered_map>
#include <string>
#include <type_traits>
#include <glm/glm.hpp>


//here we are creating a class for the light component

namespace our 
{
    // An enum that defines the type of the light source
    //0 is directional
    //1 is spot 
    //2 is point
    enum class LightType 
    {
        DIRECTIONAL,
        SPOT,
        POINT
    };


    class LightComponent : public Component 
    //the light component class which inherits from the component class
    {
    public:
        //the light properties
        //everything that will be read from the json file
        //EXCEPT the position and direction 
        //which will be calculated from the entity component
        //in the forward renderer
        int lightType; 
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 color;
        glm::vec3 attenuation; 
        glm::vec2 cone_angles; 
        
        //overriding the deserializer function 
        //to read the light data from the json file
        void deserialize(const nlohmann::json& data) override;
        
        //the id is "light"
        //which is read from the json file and specifies
        //that this is a light component
        static std::string getID() { return "light"; }

    };

}