// TEMA 1 EGC 331CB IMBREA GIULIA STEFANIA
#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/Elements.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;


     private:
        

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        
        

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void InitGUI();
        void InitScene();
        void InitLifeBar();

        void RenderGUI();
        void RenderScene();
        void RenderLifeBar();
        void RenderCoins();

        

        void RenderRhombuses(float deltaTimeSeconds);
        void RhombusDies(int *idx_enemy, float deltaTimeSeconds);
        void DragAndDrop();
        
        void SpawnEnemies(float deltaTimeSeconds);
        void EnemyDies(int *idx_enemy, float deltaTimeSeconds);
        void RenderEnemies(float deltaTimeSeconds);
        
        void SpawnBullets(float deltaTimeSeconds);
        void StartFire(float deltaTimeSeconds);
        
        void RenderStars(float deltaTimeSeconds);
        
        void RenderSpiral(float deltaTimeSeconds);
     protected:
        glm::mat3 modelMatrix;
       
         
        std::vector<GridSquare> grid;
        std::vector<Rhombus> rhombuses;
        std::vector<Rhombus> rhombusToDelete;
        std::vector<Star> stars;
        std::vector<Star> bullets;
        std::vector<Enemy> enemies;
        std::vector<Enemy> enemiesToDelete;
        std::vector<Heart> hearts;
        std::vector<GUISquare> gui;
        

         // bullet
         float bullet_speed = 200.0f;
         float bullet_rot = 5.0f;

         // enemy
         float enemy_speed = 150.0f;

         //time 
         float totalTime = 0.0f;
         float lastStar = 0.0f;
         float starInterval = 5.0f;
         float lastEnemy = 0.0f;
         float enemyInterval = 5.0f;
         
         // general
         float size_square = 250;
         int space = 50;
         float size_rhombus = 100;
         float size_star = 0.7f;
         int maxX = 2560.0f;
         int maxY = 1440.0f;
         float heart_scale = 0.7f;
         float size_guiStar = 0.5;
         float rectangle_height = 850;
         float rectangle_width = 125;
         bool pauseGame =  false;
         float spiral_spin = 0.0f;
         
         // gui
         int lives = 3;
         int coins = 5;
         
         // ratio
         float rX = 2.0f;
         float rY = 2.0f;
         
         // drag & drop
         bool isDragging = false;
         Rhombus draggedRhombus;

         // general
         int mac_resolution_x = 3456; //16-inch (3456 × 2234)
         int mac_resolution_y = 2234; //16-inch (3456 × 2234)

        glm::vec3 grey = glm::vec3((float)56/255,(float)57/255,(float)63/255);
        glm::vec3 green = glm::vec3((float)145/255,(float)196/255,(float)108/255);
        glm::vec3 orange = glm::vec3((float)255/255,(float)121/255,(float)23/255);
        glm::vec3 blue = glm::vec3((float)11/255, (float)131/255,(float)140/255);
        glm::vec3 yellow = glm::vec3((float)255/255,(float)214/255,(float)0/255);
        glm::vec3 purple = glm::vec3((float)147/255,(float)112/255,(float)219/255);
        glm::vec3 pink = glm::vec3((float)255/255,(float)182/255,(float)193/255);
        glm::vec3 orange2 = glm::vec3((float)255/255,(float)161/255,(float)92/255);
        glm::vec3 blue2 = glm::vec3((float)118/255, (float)236/255,(float)245/255);
        glm::vec3 yellow2 = glm::vec3((float)245/255,(float)235/255,(float)103/255);
        glm::vec3 purple2 = glm::vec3((float)177/255,(float)156/255,(float)217/255);
        glm::vec3 white = glm::vec3((float)245/255,(float)245/255,(float)245/255); 
        glm::vec3 red = glm::vec3((float)250/255,(float)35/255,(float)62/255);
         
      
         
    };
}  
