#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/player.hpp"
#include "../application.hpp"
#include "../components/collisions.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        std::string obstacleTypeToString(CollisionType type)
        {
            switch (type)
            {
            case CollisionType::WALL:
                return "wall";
            case CollisionType::WIN:
                return "win";
            case CollisionType::SCORE:
                return "score";

            default:
                return "Unknown Obstacle";
            }
        }
        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            PlayerComponent *player = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            bool upCollider = false;
            bool downCollider = false;
            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }
            for (auto entity : world->getEntities())
            {
                player = entity->getComponent<PlayerComponent>();
                if (player)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller))
            {
                return;
            }

            if (!player)
            {
                return;
            }
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = player->getOwner();

            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;

            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;

            position[2] += (deltaTime * current_sensitivity.z);
            float min_distance = INT_MAX - 1;
            float act_ball;
            float act_coliding;
            float mov;
            float distance;
            CollisionComponent *collision = nullptr;
            std::string act_Collision_Type;
            Entity *Collision_entity = nullptr;
            Entity *actual_collision_entity = nullptr;
            for (auto entity : world->getEntities())
            {
                collision = entity->getComponent<CollisionComponent>();
                if (!(collision))
                    continue;
                Collision_entity = collision->getOwner();

                std::string name = Collision_entity->name;
                glm::vec3 &objPosition = Collision_entity->localTransform.position;
                glm::vec3 &objScale = Collision_entity->localTransform.scale;
                bool collisionY = false;
                mov = deltaTime * current_sensitivity.z;

                float act_ball_top = position.z;
                // float act_ball_below = position.z ;

                float act_wall_top = objPosition.z + objScale.z + 1;
                float act_wall_below = objPosition.z - objScale.z - 1;

                float act_coliding_leftmost = objPosition.x + objScale.x;
                float act_coliding_writemost = objPosition.x - objScale.x;
                act_ball = position.x;

                distance = act_ball - act_coliding;

                if (act_ball <= act_coliding_leftmost && act_ball >= act_coliding_writemost) // ball is left of wall
                {
                    // std::cout<<"###########"<<std::endl;
                    if (act_ball_top > act_wall_below && act_ball_top < act_wall_top)
                    {
                        std::cout << obstacleTypeToString(collision->getobstacleType()) << std::endl;

                        if (collision->getobstacleType() == CollisionType::SCORE)
                        {
                            act_Collision_Type = "score";
                            actual_collision_entity = Collision_entity;
                        }
                        if (collision->getobstacleType() == CollisionType::WALL)
                        {
                            std::cout << "wall" << std::endl;
                            act_Collision_Type = "wall";
                            actual_collision_entity = Collision_entity;
                        }
                        else if (collision->getobstacleType() == CollisionType::WIN)

                        {
                            std::cout << "$$$$$$$$$$$" << std::endl;
                            act_Collision_Type = "win";
                            actual_collision_entity = Collision_entity;
                        }

                        min_distance = distance;
                    }
                }
            }

            if (act_Collision_Type == "wall")
            {
                app->dead = true;
            }

            if (act_Collision_Type == "win")
            {
                app->winner = true;
            }
            if (act_Collision_Type == "score")
            {
                app->score += 10;
                world->markForRemoval(actual_collision_entity);
                //position -= front * (deltaTime * current_sensitivity.z);
                actual_collision_entity = nullptr;
                act_Collision_Type = "";
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_D))
                position -= right * (deltaTime * current_sensitivity.x * 100000);
            if (app->getKeyboard().isPressed(GLFW_KEY_A))
                position += right * (deltaTime * current_sensitivity.x * 100000);
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };

}
