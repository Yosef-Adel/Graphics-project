#include "collisions.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    // Reads linearVelocity & angularVelocity from the given json object
    void CollisionComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        std::string obstacleTypeStr = data.value("obstacleType", "wall");
        
        if (obstacleTypeStr == "wall")
        {
            obstacleType = CollisionType::WALL;
        }
        else
        {
            obstacleType = CollisionType::WIN;
        }
    }
}