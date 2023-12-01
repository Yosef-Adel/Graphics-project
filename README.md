# CMPN205 Project: Graphics Rendering Engine

## Overview

This project focuses on building a graphics rendering engine using OpenGL. The primary learning outcomes include shader programming, mesh rendering, texture mapping, and implementing a simple game using the developed engine.

## Learning Outcomes

- Shader programming for GPU render pipeline
- Mesh data storage and rendering
- Object manipulation using matrices
- Depth testing, face culling, blending, and masks
- Texture drawing and sampling
- Material object combining shaders, pipeline states, and uniform data
- Entity-Component-System (ECS) framework for scene processing
- Implementing a forward renderer and sky sphere rendering
- Framebuffer-based post-processing effects
- Lighting model implementation and multiple light support
- Game implementation using the engine

## Project Structure

### Assets and Configurations

- Shader, model, and texture files in "assets" folder
- JSON configuration files in "config" folder
- Testing scripts in "scripts" folder

### Requirements

- Each requirement is associated with specific files and TODOs in the codebase
- Requirements include shader programs, meshes, transformations, pipeline states, textures, samplers, materials, entity and component systems, forward rendering system, sky rendering, and post-processing

## Running the Project

- Requires CMake and a C++17 compiler
- For Visual Studio Code users: Find executable in `bin/GAME_APPLICATION.exe`
- Usage example: `./bin/GAME_APPLICATION.exe -c='config/app.jsonc'`
- Scripts for running configurations and comparing outputs available

---

**Note:** This README is a guideline. Team members should update it as needed throughout the project to reflect any changes or additional information.
