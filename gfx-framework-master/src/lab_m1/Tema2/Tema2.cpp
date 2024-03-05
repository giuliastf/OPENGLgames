#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/Elements.h"


#include <vector>
#include <string>
#include <iostream>
// #include <__random/random_device.h>
#include <random>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2() {
    isRotating = false;
}


Tema2::~Tema2() {
}


int GetRandomNumberInRange(int minInclusive, int maxInclusive) {
    static std::random_device randomDevice;
    static std::default_random_engine randomEngine(randomDevice());

    std::uniform_int_distribution<int> uniformDist(minInclusive, maxInclusive);

    return uniformDist(randomEngine);
}


MovementState GetNextMovementState(MovementState currentState) {
    int randomChange = GetRandomNumberInRange(0, 1);
    MovementState nextState = currentState;

    switch (currentState) {
        case MovementState::GoingForward:
        case MovementState::GoingBackward:
            nextState = (randomChange == 1)
                        ? MovementState::InPlaceRotationLeft
                        : MovementState::InPlaceRotationRight;
            break;

        case MovementState::InPlaceRotationLeft:
        case MovementState::InPlaceRotationRight:
            nextState = (randomChange == 1)
                        ? MovementState::GoingForward
                        : MovementState::GoingBackward;
            break;

        default:
            break;
    }

    return nextState;
}


std::string GetMovementStateName(MovementState state) {
    static const std::map<MovementState, std::string> kStateNames
            {
                    {MovementState::GoingForward,         "GoingForward"},
                    {MovementState::GoingBackward,        "GoingBackward"},
                    {MovementState::InPlaceRotationLeft,  "InPlaceRotationLeft"},
                    {MovementState::InPlaceRotationRight, "InPlaceRotationRight"},
            };

    std::string s = "";

    auto it = kStateNames.find(state);
    if (it != kStateNames.end()) {
        s = it->second;
    }

    return s;
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;
    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    // TODO(student): Set light position uniform
    glUniform3fv(glGetUniformLocation(shader->program, "lightPosition"), 1, glm::value_ptr(lightPosition));
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    // TODO(student): Set eye position (camera position) uniform
    glUniform3fv(glGetUniformLocation(shader->program, "eyePosition"), 1, glm::value_ptr(eyePosition));
    // TODO(student): Set material property uniforms (shininess, kd, ks, object color)

    glm::vec3 materialKa = color;
    glm::vec3 materialKd = color;
    glm::vec3 materialKs = color;
    int materialShininess = 30;
    glUniform3fv(glGetUniformLocation(shader->program, "materialKa"), 1, glm::value_ptr(materialKa));
    glUniform3fv(glGetUniformLocation(shader->program, "materialKs"), 1, glm::value_ptr(materialKs));
    glUniform3fv(glGetUniformLocation(shader->program, "materialKd"), 1, glm::value_ptr(materialKd));
    glUniform1i(glGetUniformLocation(shader->program, "materialShininess"), materialShininess);
    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
    shader->Use();
    mesh->Render();
}


void Tema2::Init() {
    fov = 60.0f;
    height = 30.0f;
    width = 30.0f;

    lightPosition = glm::vec3(0, 4, 1);

    camera = new implemented::Camera_Tema2();

    {
        Mesh *mesh = new Mesh("body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "body.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("tracks");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "tracks.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("turret");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "turret.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("gun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "gun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("bullet");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "bullet.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("build1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "build1.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("build2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "build2.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh *mesh = new Mesh("plane50");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"),
                          GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"),
                          GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    SpawnEnemies();
    SpawnBuildings();
}

void Tema2::SpawnBuildings() {
    {
        Building b = Building(meshes["build1"], -5, 0, -5);
        buildings.push_back(b);
        b = Building(meshes["build1"], -20, 0, -25);
        buildings.push_back(b);
        b = Building(meshes["build1"], 5, 0, 10);
        buildings.push_back(b);
        b = Building(meshes["build1"], 15, 0, 0);
        buildings.push_back(b);
        b = Building(meshes["build1"], -15, 0, 15);
        buildings.push_back(b);
        b = Building(meshes["build1"], 10, 0, -20);
        buildings.push_back(b);
        b = Building(meshes["build2"], -15, 0, -5);
        buildings.push_back(b);
        b = Building(meshes["build2"], 5, 0, -10);
        buildings.push_back(b);
        b = Building(meshes["build2"], 10, 0, 0);
        buildings.push_back(b);
        b = Building(meshes["build2"], -5, 0, 5);
        buildings.push_back(b);
        b = Building(meshes["build2"], 15, 0, 15);
        buildings.push_back(b);
        b = Building(meshes["build2"], 0, 0, 20);


    }
}

void Tema2::RenderBuildings() {
    for (int i = 0; i < 7; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        buildings[i].radius = build1_radius;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(buildings[i].x, buildings[i].y, buildings[i].z));
        RenderSimpleMesh(meshes["build1"], shaders["LabShader"], modelMatrix,
                         glm::vec3((float) 255 / 228, (float) 255 / 225, (float) 216 / 255)); //
    }
    for (int i = 7; i < buildings.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        buildings[i].radius = build2_radius;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(buildings[i].x, buildings[i].y, buildings[i].z));
        RenderSimpleMesh(meshes["build2"], shaders["LabShader"], modelMatrix,
                         glm::vec3((float) 255 / 228, (float) 255 / 225, (float) 216 / 255));
    }

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(5, 1, 5));
    RenderSimpleMesh(meshes["plane50"], shaders["LabShader"], modelMatrix, glm::vec3(0.7f, 0.9f, 0.5f));

}

void Tema2::RenderTank(Tank *my_tank, float deltaTimeSeconds, bool isPlayer) {
    // Render tank body
    glm::vec3 color_body;
    if (isPlayer) // daca este tankul jucatorului
        color_body = glm::vec3(1.0f, 1.0f, 0.0f); //yellow
    else
        color_body = glm::vec3(0.0f, 1.0f, 0.0f); //green

    {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, glm::vec3(my_tank->x, my_tank->y, my_tank->z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(my_tank->angle), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(my_tank->scale));
        RenderSimpleMesh(meshes["body"], shaders["LabShader"], modelMatrix, color_body);
        RenderSimpleMesh(meshes["tracks"], shaders["LabShader"], modelMatrix,
                         glm::vec3((float) 255 / 114, (float) 255 / 122, (float) 255 / 107));
    }

    // Render tank turret
    {
        glm::mat4 turretMatrix = glm::mat4(1);
        turretMatrix = glm::translate(turretMatrix, glm::vec3(my_tank->x, my_tank->y, my_tank->z));
        turretMatrix = glm::rotate(turretMatrix, RADIANS(my_tank->turret.angle), glm::vec3(0, 1, 0));
        turretMatrix = glm::scale(turretMatrix, glm::vec3(my_tank->scale));
        RenderSimpleMesh(meshes["gun"], shaders["LabShader"], turretMatrix,
                         glm::vec3((float) 255 / 114, (float) 255 / 122, (float) 255 / 107));
        RenderSimpleMesh(meshes["turret"], shaders["LabShader"], turretMatrix,
                         glm::vec3((float) 255 / 114, (float) 255 / 122, (float) 255 / 107));
    }
}

void Tema2::RenderProjectiles(float deltaTimeSeconds) {
    projectile_timer += deltaTimeSeconds;

    for (int i = 0; i < projectiles.size(); i++) {
        if (projectiles[i].isAlive) {
            projectiles[i].x += projectiles[i].speed * deltaTimeSeconds * sin(glm::radians(projectiles[i].angle));
            projectiles[i].z += projectiles[i].speed * deltaTimeSeconds * cos(glm::radians(projectiles[i].angle));

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(projectiles[i].x, projectiles[i].y, projectiles[i].z));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(projectiles[i].angle), glm::vec3(0, 1, 0));  // Rotate first
            modelMatrix = glm::scale(modelMatrix, glm::vec3(tank.scale));
            RenderSimpleMesh(meshes["bullet"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1));

            projectiles[i].ttl -= deltaTimeSeconds;
            if (projectiles[i].ttl < 0.0f) {
                projectiles[i].isAlive = false;
            }

        }

    }
}

void Tema2::RenderEnemiesProjectiles(float deltaTimeSeconds) {
    for (int j = 0; j < enemies.size(); j++) {
        if (enemies[j].isAlive) {
            enemies[j].projectile_timer += deltaTimeSeconds;
            for (int i = 0; i < enemy_projectiles.size(); i++) {
                enemy_projectiles[i].x +=
                        enemy_projectiles[i].speed * deltaTimeSeconds * sin(glm::radians(enemy_projectiles[i].angle));
                enemy_projectiles[i].z +=
                        enemy_projectiles[i].speed * deltaTimeSeconds * cos(glm::radians(enemy_projectiles[i].angle));

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(enemy_projectiles[i].x, enemy_projectiles[i].y,
                                                                    enemy_projectiles[i].z));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(enemy_projectiles[i].angle),
                                          glm::vec3(0, 1, 0));  // Rotate first
                modelMatrix = glm::scale(modelMatrix, glm::vec3(tank.scale));
                RenderSimpleMesh(meshes["bullet"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1));
                enemy_projectiles[i].ttl -= deltaTimeSeconds;
                if (enemy_projectiles[i].ttl < 0.0f) {
                    enemy_projectiles[i].isAlive = false;
                }
            }
        }
    }
}

void Tema2::SpawnEnemies() {

    for (int i = 0; i < 5; i++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-15.0, 15.0);

        double x = dis(gen);
        double z = dis(gen);
        Tank enemy = Tank(Tank_Body(meshes["tank_body"], x, 0.0f, z),
                          Tank_Gun(meshes["tank_gun"], x, 0.0f, z),
                          Tank_Tracks(meshes["tank_tracks"], x, 0.0f, z),
                          Tank_Turret(meshes["tank_turret"], x, 0.0f, z), x, 0.0f, z);
        enemies.push_back(enemy);
    }
}


void Tema2::RenderEnemies(float deltaTimeSeconds) {
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].isAlive) {
            if (enemies[i].movementState == MovementState::GoingForward) {
                enemies[i].x += enemySpeed * deltaTimeSeconds * sin(glm::radians(enemies[i].angle));
                enemies[i].z += enemySpeed * deltaTimeSeconds * cos(glm::radians(enemies[i].angle));
                // Mergi înainte
            } else if (enemies[i].movementState == MovementState::GoingBackward) {
                enemies[i].x -= enemySpeed * deltaTimeSeconds * sin(glm::radians(enemies[i].angle));
                enemies[i].z -= enemySpeed * deltaTimeSeconds * cos(glm::radians(enemies[i].angle));
                // Mergi înapoi
            } else if (enemies[i].movementState == MovementState::InPlaceRotationLeft) {
                enemies[i].angle += 60.0f * deltaTimeSeconds;
                // Rotire în loc spre stânga
            } else if (enemies[i].movementState == MovementState::InPlaceRotationRight) {
                enemies[i].angle -= 60.0f * deltaTimeSeconds;
                // Rotire în loc spre dreapta
            }
            if (!enemies[i].attacks)
                enemies[i].turret.angle = enemies[i].angle;

            enemies[i].ttl -= deltaTimeSeconds;
            if (enemies[i].ttl <=
                0) { // or (enemies[i].x >= 20.0f or enemies[i].x <= -20.0f) or (enemies[i].z >= 20.0f or enemies[i].z <= -20.0f)){
                enemies[i].ttl = 2;
                enemies[i].movementState = GetNextMovementState(enemies[i].movementState);
            }
        }
        RenderTank(&enemies[i], deltaTimeSeconds, false);
    }
}


void Tema2::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Dark Blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x / 2, resolution.y / 2);
}

void Tema2::ProjectileTankCollision(float deltaTimeSeconds) {
    for (int i = 0; i < projectiles.size(); i++) {
        for (int j = 0; j < enemies.size(); j++) {
            if (projectiles[i].isAlive) {// && enemies[j].isAlive) {
                float distance = glm::length(
                        glm::vec2(projectiles[i].x - enemies[j].x, projectiles[i].z - enemies[j].z));

                if (distance < (projectile_radius + tank_radius)) {
                    std::cout << enemies[j].hp << " ";
                    enemies[j].hp--;
                    if (enemies[j].hp == 0)
                        enemies[j].isAlive = false;
                    projectiles[i].isAlive = false;
                }
            }
        }
    }
    for (int i = 0; i < enemy_projectiles.size(); i++) {
        if (enemy_projectiles[i].isAlive) {
            float distance = glm::length(glm::vec2(enemy_projectiles[i].x - tank.x, enemy_projectiles[i].z - tank.z));
            if (distance < (projectile_radius + tank_radius)) {
                tank.hp--;
                if (tank.hp == 0)
                    tank.isAlive = false;
                enemy_projectiles[i].isAlive = false;
            }
        }
    }
}

void Tema2::ProjectileBuildingCollision(float deltaTimeSeconds) {
    for (int i = 0; i < projectiles.size(); i++) {
        for (int j = 0; j < buildings.size(); j++) {
            if (projectiles[i].isAlive) {
                float distance = glm::length(
                        glm::vec2(projectiles[i].x - buildings[j].x, projectiles[i].z - buildings[j].z));

                if (distance < (projectile_radius + buildings[j].radius)) {
                    projectiles[i].isAlive = false;
                }
            }
        }
    }
    for (int i = 0; i < enemy_projectiles.size(); i++) {
        for (int j = 0; j < buildings.size(); j++) {
            if (enemy_projectiles[i].isAlive) {
                float distance = glm::length(
                        glm::vec2(enemy_projectiles[i].x - buildings[j].x, enemy_projectiles[i].z - buildings[j].z));

                if (distance < (projectile_radius + buildings[j].radius)) {
                    enemy_projectiles[i].isAlive = false;
                }
            }
        }
    }
}

void Tema2::TankTankCollision(float deltaTimeSeconds) {
    for (int i = 0; i < enemies.size(); i++) {
        for (int j = i + 1; j < enemies.size(); j++) {
            glm::vec2 center1(enemies[i].x, enemies[i].z);
            glm::vec2 center2(enemies[j].x, enemies[j].z);
            glm::vec2 dif = center2 - center1;
            float distance = glm::length(dif);
            if (distance < (2 * tank_radius)) {
                // Calculate the penetration vector
                glm::vec2 penetration = (2 * tank_radius - distance) * glm::normalize(dif);

                enemies[i].x -= penetration.x * 0.5f;
                enemies[i].z -= penetration.y * 0.5f;

                enemies[j].x += penetration.x * 0.5f;
                enemies[j].z += penetration.y * 0.5f;
            }
        }
    }
    for (int i = 0; i < enemies.size(); i++) {
        glm::vec2 center1(enemies[i].x, enemies[i].z);
        glm::vec2 center2(tank.x, tank.z);
        glm::vec2 dif = center2 - center1;
        float distance = glm::length(dif);
        if (distance < (2 * tank_radius)) {
            glm::vec2 penetration = (2 * tank_radius - distance) * glm::normalize(dif);

            enemies[i].x -= penetration.x * 0.5f;
            enemies[i].z -= penetration.y * 0.5f;

            tank.x += penetration.x * 0.5f;
            tank.z += penetration.y * 0.5f;
        }
    }


}


void Tema2::TankBuildingCollision(float deltaTimeSeconds) {
    for (int j = 0; j < buildings.size(); j++) {
        for (int i = 0; i < enemies.size(); i++) {
            //if (enemies[i].isAlive) {
            glm::vec2 center1(enemies[i].x, enemies[i].z);
            glm::vec2 center2(buildings[j].x, buildings[j].z);
            glm::vec2 dif = center2 - center1;
            float distance = glm::length(dif);
            if (distance < (tank_radius + buildings[j].radius)) {
                glm::vec2 penetration = (tank_radius + buildings[j].radius - distance) * glm::normalize(dif);
                enemies[i].x -= penetration.x * 0.5f;
                enemies[i].z -= penetration.y * 0.5f;
            }
        }
    }

    for (int j = 0; j < buildings.size(); j++) {
        glm::vec2 center1(tank.x, tank.z);
        glm::vec2 center2(buildings[j].x, buildings[j].z);
        glm::vec2 dif = center2 - center1;
        float distance = glm::length(dif);
        if (distance < (tank_radius + buildings[j].radius)) {
            glm::vec2 penetration = (tank_radius + buildings[j].radius - distance) * glm::normalize(dif);
            // Update the positions of the colliding enemies
            tank.x -= penetration.x * 0.5f;
            tank.z -= penetration.y * 0.5f;
        }
    }
}

void Tema2::Attack(float deltaTimeSeconds) {
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].isAlive) {
            glm::vec2 center1(tank.x, tank.z);
            glm::vec2 center2(enemies[i].x, enemies[i].z);
            glm::vec2 dif = center2 - center1;
            float distance = glm::length(dif);
            if (distance < 5.0f) {
                enemies[i].attacks = true;
                glm::vec2 dir = glm::normalize(dif);
                float new_angle = atan2(dir.y, dir.x);
                new_angle = glm::degrees(new_angle);
                enemies[i].turret.angle = new_angle;// + 90.0f;


                if (enemies[i].projectile_timer >= 1.0f) { //spawn projectiles
                    enemies[i].projectile_timer = 0.0f;
                    Projectile projectile = Projectile(meshes["projectile"], enemies[i].x, enemies[i].y, enemies[i].z);
                    projectile.angle = enemies[i].turret.angle;
                    projectile.speed = 5.0f;
                    projectile.isAlive = true;
                    enemy_projectiles.push_back(projectile);
                }
            } else {
                enemies[i].attacks = false;
            }
        }
    }
}


void Tema2::Update(float deltaTimeSeconds) {

    cameraOffsetX = -7.0f * sin(glm::radians(tank.angle));
    cameraOffsetY = 5.0f;
    cameraOffsetZ = -7.0f * cos(glm::radians(tank.angle));

    if (!rightClick) {
        cameraPosition = glm::vec3(tank.x + cameraOffsetX, tank.y + cameraOffsetY, tank.z + cameraOffsetZ);
        cameraTarget = glm::vec3(tank.x, tank.y, tank.z);
        camera->Set(cameraPosition, cameraTarget, glm::vec3(0, 1, 0));
    }


    RenderTank(&tank, deltaTimeSeconds, true);
    RenderProjectiles(deltaTimeSeconds);

    RenderEnemies(deltaTimeSeconds);
    RenderBuildings();
    ProjectileTankCollision(deltaTimeSeconds);
    TankTankCollision(deltaTimeSeconds);
    TankBuildingCollision(deltaTimeSeconds);
    ProjectileBuildingCollision(deltaTimeSeconds);
    Attack(deltaTimeSeconds);
    RenderEnemiesProjectiles(deltaTimeSeconds);
    if (time_playing >= 60.0f) {
        //std::cout<<"GAME OVER";
        Freeze();
    }
    time_playing += deltaTimeSeconds;
}

void Tema2::Freeze() {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].isAlive = false;
    }
    // tankSpeed = 0.0f;
    playing = false;
}

void Tema2::FrameEnd() {
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) {
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods) {
    if (!playing)
        return;
    if (window->KeyHold(GLFW_KEY_W)) {
        tank.x += tankSpeed * deltaTime * sin(glm::radians(tank.angle));
        tank.z += tankSpeed * deltaTime * cos(glm::radians(tank.angle));
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        tank.x -= tankSpeed * deltaTime * sin(glm::radians(tank.angle));
        tank.z -= tankSpeed * deltaTime * cos(glm::radians(tank.angle));
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        tank.angle += 60.0f * deltaTime;
        tank.turret.angle = lerp(tank.turret.angle, tank.angle, 0.05f);
        if (!rightClick)
            camera->RotateThirdPerson_OY(glm::radians(-tankSpeed * tank.angle));
    } else
        isRotating = false;

    if (window->KeyHold(GLFW_KEY_D)) {
        tank.angle -= 60.0f * deltaTime;
        tank.turret.angle = lerp(tank.turret.angle, tank.angle, 0.05f);
        if (!rightClick)
            camera->RotateThirdPerson_OY(glm::radians(tankSpeed * tank.angle));
    } else
        isRotating = false;
}


void Tema2::OnKeyPress(int key, int mods) {

}

void Tema2::OnKeyRelease(int key, int mods) {
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {


        camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
        camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
    }
    tank.turret.angle -= deltaX;
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    if (button == 2) {
        rightClick = true;
    } else if (button == 1 and projectile_timer >= 1.0f) { //spawn projectiles
        projectile_timer = 0.0f;
        Projectile projectile = Projectile(meshes["projectile"], tank.x, tank.y, tank.z);
        projectile.angle = tank.turret.angle;
        projectile.speed = 7.0f;
        projectile.isAlive = true;
        projectiles.push_back(projectile);
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
    if (button == 2) {
        rightClick = false;
    }
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}


void Tema2::OnWindowResize(int width, int height) {
}
