// TEMA 1 EGC 331CB IMBREA GIULIA STEFANIA
#include <vector>
#include <iostream>

#include "lab_m1/Tema1/Transform2D.h"
#include "lab_m1/Tema1/Object2D.h"
#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/Elements.h"


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

void Tema1::InitGUI(){
    int i = 0;

    float x = (i+2)* space + size_square * i + size_square/2;
    float y = maxY - space - size_square/2;
    gui.push_back(GUISquare(
                            Square(meshes["guiSquare"], size_square, glm::vec3(x, y, 0), white),
                            Rhombus(meshes["rhombus"], size_rhombus, 1, glm::vec3(x, y, 0), orange)));
    i++;
    x = (i+2) * space + size_square * i + size_square/2;
    gui.push_back(GUISquare(
                            Square(meshes["guiSquare"], size_square, glm::vec3(x, y, 0), white),
                            Rhombus(meshes["rhombus"], size_rhombus, 2, glm::vec3(x, y, 0), blue)));

    i++;
    x =  (i+2)* space + size_square * i + size_square/2;
    gui.push_back(GUISquare(
                            Square(meshes["guiSquare"], size_square, glm::vec3(x, y, 0), white),
                            Rhombus(meshes["rhombus"], size_rhombus, 2, glm::vec3(x, y, 0), yellow)));
    i++;
    x = (i+2) * space + size_square * i + size_square/2;
    gui.push_back(GUISquare(
                            Square(meshes["guiSquare"],  size_square, glm::vec3(x, y, 0), white),
                            Rhombus(meshes["rhombus"], size_rhombus, 3, glm::vec3(x, y, 0), purple)));
}


void Tema1::InitScene(){
    float x = 2*space + rectangle_width + size_square/2;
    float y = space + size_square/2;
    float aux = x;
    glm::vec3 center = glm::vec3(x, y, 0);
    for (int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            grid.push_back(GridSquare(Square(meshes["square"], size_square, center, green), i));
            center.x += size_square + space;
        }
        center.x = aux;
        center.y += size_square + space;
    }    
}

void Tema1::InitLifeBar(){
    float y = maxY - space - size_square/2;
    float x = maxX - 3*size_square- 2*space;
    for(int i = 0; i < lives; i++) {
        hearts.push_back(Heart(meshes["heart"], heart_scale, glm::vec3(x, y, 0), red));
        x += size_square + space;
    }
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false); 

    Mesh *square = Object2D::CreateSquare("square", true);
    AddMeshToList(square);
    Mesh *guiSquare = Object2D::CreateSquare("guiSquare", false);
    AddMeshToList(guiSquare);
    Mesh *rhombus = Object2D::CreateRhombus("rhombus");
    AddMeshToList(rhombus);
    Mesh* star = Object2D::CreateStar("star");
    AddMeshToList(star);
    Mesh* big_hexagon = Object2D::CreateHexagon("big_hexagon", 1);
    AddMeshToList(big_hexagon);
    Mesh* small_hexagon = Object2D::CreateHexagon("small_hexagon", 2);
    AddMeshToList(small_hexagon);
    Mesh *heart = Object2D::CreateHeart("heart", true);
    AddMeshToList(heart);
    Mesh *empty_heart = Object2D::CreateHeart("empty_heart", false);
    AddMeshToList(empty_heart);
    Mesh *spiral = Object2D::CreateSpiral("spiral");
    AddMeshToList(spiral);

    InitGUI();
    InitScene();
    InitLifeBar();
}
                            

void Tema1::FrameStart()
{
    glClearColor(grey.x, grey.y, grey.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::RenderGUI(){
    for(int i = 0; i<gui.size(); i++){
        modelMatrix = glm::mat3(1); 
        modelMatrix *= Transform2D::Translate(gui[i].square.center.x, gui[i].square.center.y);
        modelMatrix *= Transform2D::Scale(gui[i].square.size, gui[i].square.size);
        RenderMesh2D(gui[i].square.mesh, modelMatrix, gui[i].square.color);
        modelMatrix = glm::mat3(1); 
        modelMatrix *= Transform2D::Translate(gui[i].rhombus.center.x, gui[i].rhombus.center.y);
        modelMatrix *= Transform2D::Scale(gui[i].rhombus.size, gui[i].rhombus.size);
        RenderMesh2D(gui[i].rhombus.mesh, modelMatrix, gui[i].rhombus.color);

        int noStars;
        if(gui[i].rhombus.color == orange){
            noStars = 1;
        } else if(gui[i].rhombus.color == blue || gui[i].rhombus.color == yellow){
            noStars = 2;
        } else{
            noStars = 3;
        }
        
        int extra_space = 0;
        for(int j = 0; j < noStars; j++) {
            modelMatrix = glm::mat3(1); 
            extra_space = j * 1.5 * space;
            modelMatrix *= Transform2D::Translate(gui[i].square.center.x - gui[i].square.size/2 + space + extra_space , gui[i].square.center.y - gui[i].square.size/2 - space);
            modelMatrix *= Transform2D::Scale(size_guiStar, size_guiStar);
            RenderMesh2D(meshes["star"], modelMatrix, yellow2);
        }
    }
}


void Tema1::RenderScene() {
    glm::vec3 center= glm::vec3(space+rectangle_width/2, space+rectangle_height/2, 0); // center of the red rectangle
    modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(center.x, center.y);
    modelMatrix *= Transform2D::Scale(size_square, size_square);
    modelMatrix *= Transform2D::Scale(rectangle_width/size_square, rectangle_height/size_square);

    RenderMesh2D(meshes["square"], modelMatrix, red);

    center += glm::vec3(rectangle_width/2 + space, space + size_square/2, 0); // start for the squares
    
    for(int i = 0; i < grid.size(); i++){
            modelMatrix = glm::mat3(1);
	        modelMatrix *= Transform2D::Translate(grid[i].square.center.x, grid[i].square.center.y);
            modelMatrix *= Transform2D::Scale(size_square, size_square);
            RenderMesh2D(grid[i].square.mesh, modelMatrix, grid[i].square.color);
    }   
}


void Tema1::RenderLifeBar(){
    for(int i = 0; i < hearts.size(); i++){
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(hearts[i].center.x, hearts[i].center.y);
        modelMatrix *= Transform2D::Scale(hearts[i].scale, hearts[i].scale);
        RenderMesh2D(hearts[i].mesh, modelMatrix, hearts[i].color);
    }
}

void Tema1::RenderCoins(){
    float y = maxY - 2*space - size_square;
    float x = maxX - 3*size_square - 2*space - size_square/2;

    for(int i=0; i < coins; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(x, y);
        modelMatrix *= Transform2D::Scale(size_guiStar, size_guiStar);
        RenderMesh2D(meshes["star"], modelMatrix, yellow2);

        x += size_guiStar + 1.5 * space;
    }
}



void Tema1::RenderRhombuses(float deltaTimeSeconds){
    for(int i=0; i < rhombuses.size(); i++) {
        for(int j=0; j< enemies.size(); j++) {
            if(rhombuses[i].row == enemies[j].row) {
                float distance = enemies[j].outerHexa.center.x - rhombuses[i].center.x;
                float maxDistance = enemies[j].outerHexa.size/2 + rhombuses[i].size/2; 
                //if(distance < 0)
                if(distance <= maxDistance && distance >= 0)
                    rhombuses[i].isAlive = false; // marchez moartea rombului
            }
        }
        RhombusDies(&i, deltaTimeSeconds);
    }
}


void Tema1::RhombusDies(int *idx_rhombus, float deltaTimeSeconds) {
    Rhombus *r = &rhombuses[*idx_rhombus];

    if (!r->isAlive) { // shrink
        r->size -= 100 * deltaTimeSeconds;

        if (r->size <= 0) {
            grid[r->grid].occupied = false;
            rhombuses.erase(rhombuses.begin() + *idx_rhombus);
            (*idx_rhombus)--;
        }
    }

    // render the rhombus no matter what
    modelMatrix = glm::mat3(1);
    modelMatrix *= Transform2D::Translate(r->center.x, r->center.y);
    modelMatrix *= Transform2D::Scale(r->size, r->size);
    RenderMesh2D(r->mesh, modelMatrix, r->color);
}

void Tema1::DragAndDrop(){
    if(isDragging){
        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(draggedRhombus.center.x,draggedRhombus.center.y);
        modelMatrix *= Transform2D::Scale(draggedRhombus.size, draggedRhombus.size);
        RenderMesh2D(draggedRhombus.mesh, modelMatrix, draggedRhombus.color);
    }
}

void Tema1::SpawnEnemies(float deltaTimeSeconds) {
    lastEnemy += deltaTimeSeconds;
    if(lastEnemy > enemyInterval) {
        lastEnemy = 0.0f;
        enemyInterval = rand() % 5 + 2; // between 2 and 6 seconds
        int noEnemies = rand() % 4;
        int row = rand() % 3;
        float y;
        switch (row)
        {
        case 0:
            y = grid[0].square.center.y;
            break;
        case 1:
            y = grid[3].square.center.y;
            break;
        case 2:
            y = grid[6].square.center.y;
            break;
        }

        int type = rand() % 4;
        glm::vec3 color1, color2;
        switch (type)
        {
            case 0:
                color1 = orange;
                color2 = orange2;
                break;
            case 1:
                color1 = blue;
                color2 = blue2;
                break;
            case 2:
                color1 = purple;
                color2 = purple2;
                break;
            case 3:
                color1 = yellow;
                color2 = yellow2;
                break;  
        }

        Enemy enemy = Enemy(
                            Hexagon(meshes["big_hexagon"], 1, glm::vec3(maxX, y, 1), color2),
                            Hexagon(meshes["small_hexagon"], 0.7, glm::vec3(maxX, y, 2), color1), 
                            color1, row);
        enemies.push_back(enemy);
    }
}


void Tema1::EnemyDies(int *idx_enemy, float deltaTimeSeconds) {
    Enemy *e = &enemies[*idx_enemy];

    if (!e->isAlive) { 
        e->outerHexa.size -= 3* e->outerHexa.size *deltaTimeSeconds;
        e->innerHexa.size -= 3* e->innerHexa.size *deltaTimeSeconds;

        if (e->outerHexa.size <= 0.1f) { // it will never be 0
            enemies.erase(enemies.begin() + *idx_enemy);
            (*idx_enemy)--;
        }
    }
    int j = *idx_enemy;
    modelMatrix = glm::mat3(1);
    modelMatrix *= Transform2D::Translate(e->outerHexa.center.x, e->outerHexa.center.y);
    modelMatrix *= Transform2D::Scale(e->outerHexa.size, e->outerHexa.size);
    RenderMesh2D(e->outerHexa.mesh, modelMatrix, e->outerHexa.color);

    modelMatrix = glm::mat3(1);
    modelMatrix *= Transform2D::Translate(e->innerHexa.center.x, e->innerHexa.center.y);
    modelMatrix *= Transform2D::Scale(e->innerHexa.size, e->innerHexa.size);
    RenderMesh2D(e->innerHexa.mesh, modelMatrix, e->innerHexa.color);
}

// move enemies + check collision rectangle - enemy
void Tema1::RenderEnemies(float deltaTimeSeconds){
    for(int i = 0; i < enemies.size(); i++){
        if(enemies[i].outerHexa.size == 1.0f) {
        enemies[i].outerHexa.center.x -= enemy_speed * deltaTimeSeconds;
        enemies[i].innerHexa.center.x -= enemy_speed * deltaTimeSeconds;
        }
        float distance = enemies[i].outerHexa.center.x - enemies[i].outerHexa.size;
        if(distance < space+rectangle_width/2 && enemies[i].isAlive) {
            lives --;
            for(int j = lives; j < hearts.size(); j++) {
                hearts[j].fill = false;
                hearts[j].mesh = meshes["empty_heart"];
            }
            enemies[i].isAlive = false;
        }
        EnemyDies(&i, deltaTimeSeconds);
    }
}

void Tema1::SpawnBullets(float deltaTimeSeconds) {
    for(int i = 0; i < rhombuses.size(); i++){
        if(rhombuses[i].time > 0)
            rhombuses[i].time -= deltaTimeSeconds;
        else
            rhombuses[i].time = 0;
        for(int j = 0; j < enemies.size(); j++){
            if(rhombuses[i].row == enemies[j].row && rhombuses[i].color == enemies[j].color){
                float distance = enemies[j].outerHexa.center.x - rhombuses[i].center.x;
                if(distance > 0 && rhombuses[i].time <= 0.0f) {
                    Star bullet = Star(meshes["star"], size_star, rhombuses[i].center + glm::vec3(space, 0, 1), rhombuses[i].color);
                    bullet.row = rhombuses[i].row;
                    bullets.push_back(bullet);

                    rhombuses[i].time = 1.0f; // spawns a bullet every second
                }
            }
        }
    }
}


// move bullets + check collision bullet - enemy
void Tema1::StartFire(float deltaTimeSeconds){
    for(int i = 0; i < bullets.size(); i++) {
        for(int j = 0; j < enemies.size(); j++) {
            if(bullets[i].row == enemies[j].row) {
                float distance = enemies[j].outerHexa.center.x - bullets[i].center.x;
                float max = enemies[j].outerHexa.size/2 + bullets[i].size/2;    
                if(distance <= max) {
                    if(bullets[i].color == enemies[j].color) { // attack
                        enemies[j].life --;
                        if(enemies[j].life == 0) {
                            enemies[j].isAlive = false;
                        }
                    }
                    bullets[i].isAlive = false;
                }
            }
            EnemyDies(&j, deltaTimeSeconds);
        }

        if(bullets[i].isAlive == false) { // delete bullet
            bullets.erase(bullets.begin() + i);
            i--;
        } else { // move bullet 
            bullets[i].center.x += bullet_speed * deltaTimeSeconds;
            if(bullets[i].spin > 2*M_PI)
                bullets[i].spin = 0;
            else
                bullets[i].spin += bullet_rot * deltaTimeSeconds;
            
            modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D::Translate(bullets[i].center.x, bullets[i].center.y);
            modelMatrix *= Transform2D::Rotate(bullets[i].spin);
            modelMatrix *= Transform2D::Scale(bullets[i].size, bullets[i].size);
            RenderMesh2D(bullets[i].mesh, modelMatrix, bullets[i].color);
        }
    }
}


void Tema1::RenderStars(float deltaTimeSeconds){
    int up = maxY - 3*space - size_square;
    lastStar += deltaTimeSeconds;
    if(lastStar > starInterval) {
        lastStar = 0.0f;    
        starInterval = rand() % 2 + 3; // between 3 and 5 seconds
        int noStars = rand() % 3; 

        glm::vec3 center = glm::vec3(rand() % maxX, rand() % up, 2);
        Star star = Star(meshes["star"], size_star, center, white);
        stars.push_back(star);
    }

    for (auto it = stars.begin(); it != stars.end();) {
        it->ttl -= deltaTimeSeconds;
        if (it->ttl < 0) {
            it = stars.erase(it);
        } else {
            modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D::Translate(it->center.x, it->center.y);
            modelMatrix *= Transform2D::Scale(it->size, it->size);
            RenderMesh2D(it->mesh, modelMatrix, it->color);
            ++it;
        }
    }
}

// bonus
void Tema1::RenderSpiral(float deltaTimeSeconds){
    spiral_spin += 20*deltaTimeSeconds;
    modelMatrix = glm::mat3(1);
    modelMatrix *= Transform2D::Translate(maxX/2, maxY/2);
    modelMatrix *= Transform2D::Rotate(spiral_spin);
    modelMatrix *= Transform2D::Scale(2,2);

    RenderMesh2D(meshes["spiral"], modelMatrix, white);
}

void Tema1::Update(float deltaTimeSeconds)
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    totalTime += deltaTimeSeconds;
    if(pauseGame == true || lives == 0){
        RenderSpiral(deltaTimeSeconds);
    } else {
        RenderGUI();
        RenderScene();
        RenderLifeBar();
        RenderCoins();
        

        DragAndDrop(); // drag and drop for rhombuse from gui to grid
        RenderRhombuses(deltaTimeSeconds); // draws rhombuse or not
        SpawnEnemies(deltaTimeSeconds); // spawn enemies
        RenderEnemies(deltaTimeSeconds); // move enemies + collision rectangle - enemy
        SpawnBullets(deltaTimeSeconds); // spawn bullets
        StartFire(deltaTimeSeconds); // move bullets + collision bullet - enemy
        RenderStars(deltaTimeSeconds); // random stars/coins
    }
}



void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema1::OnKeyPress(int key, int mods)
{
    if(key == GLFW_KEY_SPACE) { // pause game
        if(pauseGame == false)
            pauseGame = true;
        else
            pauseGame = false;
    }
    
}


void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // click on star / coin
    if(button == GLFW_MOUSE_BUTTON_2) {
        for(auto it = stars.begin(); it != stars.end(); ) {
            if(it->checkClick(mouseX * rX , maxY - rY*mouseY) ) {
                it = stars.erase(it);
                coins++;
            } else {
                ++it;
            }
        }
    }

    // drag and drop
    if(button == GLFW_MOUSE_BUTTON_2) {
        for(int i = 0; i < gui.size(); i++) {
            if(gui[i].square.CheckClick(mouseX * rX , maxY - rY*mouseY) && coins >= gui[i].rhombus.price) {
                isDragging =  true;
                draggedRhombus = gui[i].rhombus;
            }
        }
    }

    // right click to delete rhombus
    if(button == GLFW_MOUSE_BUTTON_3) {
            for(int i = 0; i < grid.size(); i++) {
                if(grid[i].square.CheckClick(mouseX * rX , maxY - rY*mouseY) && grid[i].occupied == true) {
                    for(int j = 0; j < rhombuses.size(); j++) {
                        if(rhombuses[j].grid == i)
                            rhombuses[j].isAlive = false;       
                    }
                }
            }
    }
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    if(isDragging){
        draggedRhombus.center.x = mouseX * rX;
        draggedRhombus.center.y = maxY - rY*mouseY;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    if(button == GLFW_MOUSE_BUTTON_2){
        if(isDragging){
            for(int i=0; i<grid.size(); i++){
                if(grid[i].occupied == false && grid[i].square.CheckClick(mouseX * rX , 1440.0f - rY*mouseY)){
                    grid[i].rhombus = draggedRhombus;
                    grid[i].occupied = true; // square has a rhombus

                    draggedRhombus.center = grid[i].square.center;
                    draggedRhombus.row = grid[i].row;
                    draggedRhombus.grid = i;
                    rhombuses.push_back(draggedRhombus);

                    isDragging = false;
                    coins -= draggedRhombus.price;
                    return;
                }
            }
            isDragging = false;
        }
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{   
    // rescale on MacBook Pro, resolution is 2*1280 x 2*720 even though it's set to 1280 x 720
    rX = 4*1280.0f / (float)width;
    rY = 4*720.0f / (float)height; 
}