#include "light.hpp"
#include "../deserialize-utils.hpp"
#include "./component-deserializer.hpp"
#include <glm/gtx/euler_angles.hpp>

namespace our
{
    // the deserializer function that reads the data from the json file
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object()) // if it is not an object asln
            return;
        std::string lightTypeStr;
        // setting a default value to everything we read
        lightTypeStr = data.value("typeOfLight", "DIRECTIONAL");

        // setting the light type integer based on what we read from the json file
        if (lightTypeStr == "DIRECTIONAL")
            lightType = 0;

        else if (lightTypeStr == "POINT")
            lightType = 1;

        else if (lightTypeStr == "SPOT")
            lightType = 2;

        // reading the diffuse vector
        diffuse = glm::vec3(data.value("diffuse", glm::vec3(1, 1, 1)));
        // reading the specular vector
        specular = glm::vec3(data.value("specular", glm::vec3(1, 1, 1)));

        color = glm::vec3(data.value("color", glm::vec3(0, 0, 0)));

        // if it is a point or a spot light
        // then there is an attenuation vector we have to read as well
        //(q,l,c)
        if (lightType != 0)
        {
            attenuation = glm::vec3(data.value("attenuation", glm::vec3(1, 0, 0)));
        }

        // specific for the spot light
        // we have to read the inner and outer cone angles
        // we will read them in degrees from the json and we will convert them to radians here
        if (lightType == 2)
        {
            cone_angles.x = glm::radians((float)data.value("cone_angles.in", 10));
            cone_angles.y = glm::radians((float)data.value("cone_angles.out", 80));
        }
    }

}