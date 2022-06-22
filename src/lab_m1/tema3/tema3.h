#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"

#define N 64

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void InitMeshes();
        void InitTexture();
        void InitCone();
        void InitFloorPositions();
        void InitPositions();
        void FrameStart() override;
        void FrameEnd() override;
        void MoveDancers();
        void RenderFloor();
        void RenderDancers();
        void RenderGlobe();
        void RenderWalls();
        void Update(float deltaTimeSeconds) override;
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
            Texture2D* texture1, const glm::vec3& color = glm::vec3(1));
        void RenderCones();
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unordered_map<std::string, Texture2D*> mapTextures;

        glm::vec3 lightPosition[N], colors[N], newColors[N];
        glm::vec3 dancersPositions[6], nextDancersPositions[6];
        glm::vec3 lightDirection;
        glm::vec3 spotLights[4];


        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        float cutOff;
        float x, y;

        float sphereRotationAngle, coneRotationAngle, incrementRotation;
        int mode, addTexture, isFloor;
        float time;
    };
}   // namespace m1
