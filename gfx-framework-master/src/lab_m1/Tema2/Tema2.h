#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/Tema2_camera.h"
#include "lab_m1/Tema2/Elements.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderTank(Tank *tank, float deltaTimeSeconds, bool isEnemy);
        void RenderProjectiles(float deltaTimeSeconds);
        void RenderEnemies(float deltaTimeSeconds); //problema la limitare in plan
        void SpawnEnemies();
        void TankTankCollision(float deltaTimeSeconds); // este ok
        void ProjectileTankCollision(float deltaTimeSeconds); // este ok
        void TankBuildingCollision(float deltaTimeSeconds);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4 & modelMatrix, const glm::vec3 &color);
        void ProjectileBuildingCollision(float deltaTimeSeconds);
        void RenderBuildings();
        void SpawnBuildings();
        void Freeze();
        void Attack(float deltaTimeSeconds);
        void RenderEnemiesProjectiles(float deltaTimeSeconds);
    protected:
        implemented::Camera_Tema2 *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        std::vector<Projectile> projectiles;
        std::vector<Projectile> enemy_projectiles;
        std::vector<Tank> enemies;
        std::vector<Building> buildings;

        // TODO(student): If you need any other class variables, define them here.
        float fov;
        float height, width;

        float tunkSpeed = 0.0f;
        float angle = 0.0f;
        glm::mat4 modelMatrix;
        Tank tank;
        float cameraOffsetX;
        float cameraOffsetY;
        float cameraOffsetZ;
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        bool rightClick = false;


        float projectile_timer = 1.0f;

        float tankSpeed = 5.0f;
        int no_enemies = 2;
        bool enemySpawned = false;
        float enemySpeed = 3.0f;


        glm::vec3 lightPosition;
        bool isRotating = false;


        float tank_radius = 1.559f;
        float build1_radius = 1.486f;
        float build2_radius = 3.296f;
        float projectile_radius = 0.096;

        float time_playing = 0.0f;
        bool playing = true;

        glm::vec2 limit = glm::vec2(20.0f, 20.0f);

    };
}   // namespace m1
