#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"
//#include <iostream>

namespace our {

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json& config){
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if(config.contains("sky")){
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));
            
            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram* skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();
            
            //TODO: (Req 10) Pick the correct pipeline state to draw the sky
            // Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            // We will draw the sphere from the inside, so what options should we pick for the face culling.
            
            PipelineState skyPipelineState;
            skyPipelineState.faceCulling.enabled = true;
            skyPipelineState.faceCulling.frontFace = GL_CCW;
            skyPipelineState.faceCulling.culledFace = GL_FRONT;
            skyPipelineState.depthTesting.enabled = true;
            skyPipelineState.depthTesting.function = GL_LEQUAL;

            
            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D* skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky 
            Sampler* skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material 
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if(config.contains("postprocess")){
            //TODO: (Req 11) Create a framebuffer
            glGenFramebuffers(1, &postprocessFrameBuffer);
            //TODO: (Req 11) Create a color and a depth texture and attach them to the framebuffer
            // Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            // The depth format can be (Depth component with 24 bits).
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);

            colorTarget = texture_utils::empty(GL_RGBA, windowSize);
            depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT, windowSize);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);

            //TODO: (Req 11) Unbind the framebuffer just to be safe
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler* postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram* postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy(){
        // Delete all objects related to the sky
        if(skyMaterial){
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if(postprocessMaterial){
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World* world){
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent* camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();
        for(auto entity : world->getEntities()){
            // If we hadn't found a camera yet, we look for a camera in this entity
            if(!camera) camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if(auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer){
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if(command.material->transparent){
                    transparentCommands.push_back(command);
                } else {
                // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if(camera == nullptr) return;

        //TODO: (Req 9) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        // HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        glm::mat4 VM = camera->getViewMatrix();
        glm::vec3 cameraForward = glm::vec3(VM[2][0], VM[2][1], VM[2][2]); // 3rd row
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand& first, const RenderCommand& second){
            //TODO: (Req 9) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second". 
            return first.center.z < second.center.z;
        });

        //TODO: (Req 9) Get the camera ViewProjection matrix and store it in VP
        glm::mat4 VP = camera->getProjectionMatrix(windowSize) * camera->getViewMatrix();
        
        //TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
        glViewport(0, 0, windowSize.x, windowSize.y);

        // TODO: (Req 9) Set the clear color to black and the clear depth to 1
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClearDepth(1);

        // TODO: (Req 9) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        // If there is a postprocess material, bind the framebuffer
        if(postprocessMaterial){
            //TODO: (Req 11) bind the framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);
        }

        //TODO: (Req 9) Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //TODO: (Req 9) Draw all the opaque commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        glm::vec3 cameraPosition = camera->getOwner()->localTransform.position;
        for (const auto& command : opaqueCommands)
        {
            // Set the "transform" uniform to be equal to the model-view-projection matrix
            glm::mat4 MVP = VP * command.localToWorld;

            command.material->transparent = false;
            command.material->setup();
            command.material->shader->set("transform", MVP);
            command.material->shader->set("objectToWorld", command.localToWorld);
            //command.material->shader->set("objectToInvTranspose", glm::transpose(glm::inverse(command.localToWorld)));
            //command.material->shader->set("cameraPosition", cameraPosition);

            command.mesh->draw();
        }
        
        // If there is a sky material, draw the sky
        if(this->skyMaterial){
            //TODO: (Req 10) setup the sky material
            this->skyMaterial->setup();
            //TODO: (Req 10) Get the camera position
            // already gotten above
            // glm::vec3 cameraPosition = camera->getOwner()->localTransform.position; 
            //TODO: (Req 10) Create a model matrix for the sy such that it always follows the camera (sky sphere center = camera position)
            glm::mat4 model = glm::mat4(1.0f); // Start with an identity matrix
            model[0][3] = cameraPosition[0]; // Set the x-position to the camera's x-position
            model[1][3] = cameraPosition[1]; // Set the y-position to the camera's y-position
            model[2][3] = cameraPosition[2]; // Set the z-position to the camera's z-position
            // A model matrix is created such that the sky sphere always follows the camera. 
            // This is done by setting the translation part of the model matrix to the camera position.
            // In the case of the sky sphere, we want it to always be centered around the camera.
            // This gives the illusion that the sky is infinitely far away, because no matter where the camera moves, the skybox appears to stay in the same place.
            
            //TODO: (Req 10) We want the sky to be drawn behind everything (in NDC space, z=1)
            // We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?
            glm::mat4 alwaysBehindTransform = glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 1.0f
            );

            //This matrix is designed to manipulate the depth values of the skybox in Normalized Device Coordinates (NDC) space. 
            //NDC space is a coordinate system where the visible portion of the scene fits within a cube that spans from -1 to 1 in all three dimensions (x, y, and z).
            //In NDC space, a z-value of 1 represents the farthest visible depth.
            //By multiplying the skybox’s vertices by alwaysBehindTransform, we’re effectively setting their z-values to 1 in NDC space.
            
            //First row [1.0f, 0.0f, 0.0f, 0.0f]: This row corresponds to the x-coordinates. 
            //The 1.0f value means that the x-coordinate of the transformed point will be the same as the original x-coordinate. 
            //The rest of the values in this row are 0, meaning the y, z, and w coordinates of the original point do not contribute to the new x-coordinate.

            //Second row [0.0f, 1.0f, 0.0f, 0.0f]: This row corresponds to the y-coordinates.
            //The 1.0f value means that the y-coordinate of the transformed point will be the same as the original y-coordinate. 
            //The rest of the values in this row are 0, meaning the x, z, and w coordinates of the original point do not contribute to the new y-coordinate.

            //Third row [0.0f, 0.0f, 0.0f, 0.0f]: This row corresponds to the z-coordinates. All values in this row are 0,
            //which means that the z-coordinate of the transformed point will always be 0, regardless of the original z-coordinate.
            //This effectively sets the depth value of the skybox to 0 in NDC space. but here we are applying it after so its in the clip space see comments below

            //Fourth row [0.0f, 0.0f, 1.0f, 1.0f]: This row corresponds to the w-coordinates, which are used for homogeneous coordinates. 
            //The 1.0f for the z-coordinate means that the z-coordinate of the original point contributes to the new w-coordinate. 
            //The other 1.0f means that the original w-coordinate (which is typically 1 for points) also contributes to the new w-coordinate. 
            //The rest of the values in this row are 0, meaning the x and y coordinates of the original point do not contribute to the new w-coordinate.

            //This matrix is applied after the projection matrix, so it operates in clip space. In clip space, the coordinates are homogeneous coordinates, 
            //meaning they have four components: x, y, z, and w. The actual 3D coordinates are obtained by dividing x, y, and z by w.
            //The third row of the matrix sets the z-coordinate to 0, but the fourth row sets the w-coordinate to the original z-coordinate. 
            //When the perspective division (x/w, y/w, z/w) is performed
            
            //The reason this matrix works for rendering the skybox behind everything else is because of how depth testing works in OpenGL. 
            //By default, OpenGL uses a depth range of [0, 1], where 0 is the nearest depth and 1 is the farthest. However, when depth testing is enabled, 
            //fragments (potential pixels) with a z-coordinate of 0 in NDC space pass the depth test and are drawn on top of everything else.
            //So, by setting the z-coordinate of the skybox to 0 in NDC space, you’re effectively disabling depth testing for the skybox. 
            //It will always be drawn, regardless of the depth of other objects in the scene. However, because the skybox is drawn first and doesn’t update the depth buffer, 
            //all other objects will still be drawn on top of it, as long as their depth is less than 1.
            //This is a bit counter-intuitive, but it’s a common trick used in OpenGL to render skyboxes.
            
            glm::mat4 projection = camera->getProjectionMatrix(windowSize);

            // The projection matrix of the camera is retrieved using camera->getProjectionMatrix(windowSize).
            // This matrix is used to transform the 3D points in the world to 2D points on the screen.

            //TODO: (Req 10) set the "transform" uniform
            skyMaterial->shader->set("transform", alwaysBehindTransform * projection * model);
            // the “transform” uniform in the shader program is set to the product of alwaysBehindTransform, projection, and model.
            // This combined transformation matrix is used to correctly position, orient, and scale the skybox in the 3D scene.

            //TODO: (Req 10) draw the sky sphere
            skySphere->draw();
        }
        //TODO: (Req 9) Draw all the transparent commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        //glm::vec3 cameraPosition = camera->getOwner()->localTransform.position;
        for (const auto& command : transparentCommands)
        {
            // Set the "transform" uniform to be equal to the model-view-projection matrix
            glm::mat4 MVP = VP * command.localToWorld;
            command.material->transparent = true;

            command.material->setup();
            command.material->shader->set("transform", MVP);
            command.material->shader->set("objectToWorld", command.localToWorld);
            //command.material->shader->set("objectToInvTranspose", glm::transpose(glm::inverse(command.localToWorld)));
            //command.material->shader->set("cameraPosition", cameraPosition);

            command.mesh->draw();
        }

        // If there is a postprocess material, apply postprocessing
        if(postprocessMaterial){
            //TODO: (Req 11) Return to the default framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            //TODO: (Req 11) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();

            glBindVertexArray(postProcessVertexArray);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
        }

        // GLenum err;
        // while((err = glGetError()) != GL_NO_ERROR)
        // {
        //     std::cerr << "OpenGL error: " << err << std::endl;
        // }

    }

}