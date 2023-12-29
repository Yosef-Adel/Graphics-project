#include "collisions.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include <iostream>
namespace our
{
    // Reads linearVelocity & angularVelocity from the given json object
    void CollisionComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;



        std::string obstacleTypeStr = data.value("obstacleType", "score");
        
        if (obstacleTypeStr == "score")
        {
                        std::cout << "score wall" << std::endl;
            obstacleType = CollisionType::SCORE;
        }
        else if (obstacleTypeStr == "wall")
        {
                std::cout << " wall" << std::endl;
            obstacleType = CollisionType::WALL;
        }
        else
        {
            std::cout << "win wall" << std::endl;
            obstacleType = CollisionType::WIN;
        }

    }
}