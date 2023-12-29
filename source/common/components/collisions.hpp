#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{

    enum class CollisionType
    {
        WALL,
        WIN,
        SCORE
    };

    // This component denotes that the MovementSystem will move the owning entity by a certain linear and angular velocity.
    // This component is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/movement.hpp"
    // For a more complex example of how to use the ECS framework, see "free-camera-controller.hpp"
    class CollisionComponent : public Component
    {
    public:
        CollisionType obstacleType;
        // The ID of this component type is "Movement"
        static std::string getID() { return "collision"; }
        CollisionType getobstacleType() { return this->obstacleType; }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json &data) override;
    };

}