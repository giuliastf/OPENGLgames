#pragma once

#include "components/simple_scene.h"

namespace m1{

    enum class MovementState{
        GoingForward,
        GoingBackward,
        InPlaceRotationLeft,
        InPlaceRotationRight
    };

    class Tank_Body{ 
    public:

        float x;
        float y;
        float z;
        Mesh *mesh;

        Tank_Body(Mesh *mesh, float x, float y, float z) {
            this->mesh = mesh;
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Tank_Body(){}

    };

    class Tank_Gun{ 
    public:

        float x;
        float y;
        float z;
        Mesh *mesh;

        Tank_Gun(Mesh *mesh, float x, float y, float z) {
            this->mesh = mesh;
            this->x = x;
            this->y = y;
            this->z = z;
        }
        
        Tank_Gun(){}

    };

    class Tank_Tracks{ 
    public:

        float x;
        float y;
        float z;
        Mesh *mesh;

        Tank_Tracks(Mesh *mesh, float x, float y, float z) {
            this->mesh = mesh;
            this->x = x;
            this->y = y;
            this->z = z;
        }
        
        Tank_Tracks(){}

    };

    class Tank_Turret{ 
    public:

        float x;
        float y;
        float z;
        Mesh *mesh;
        float angle = 0.0f;
        float speed = 2.0f;

        Tank_Turret(Mesh *mesh, float x, float y, float z) {
            this->mesh = mesh;
            this->x = x;
            this->y = y;
            this->z = z;
        }
        
        Tank_Turret(){}

    };

    class Tank{
    public:
        Tank_Body body;
        Tank_Gun gun;
        Tank_Tracks tracks;
        Tank_Turret turret;
        float x = 0.0f;
        float y = 0.5f;
        float z = 0.0f;
        float angle = 0.0f;
        float scale = 1.0f;
        bool isAlive = true;
        float speed = 3.0f;
        float ttl = 3.0f;
        int hp = 3;
        float projectile_timer = 1.0f;
        bool attacks = false;
        
        MovementState movementState = MovementState::GoingForward;
        
        Tank(Tank_Body body, Tank_Gun gun, Tank_Tracks tracks, Tank_Turret turret, float x, float y, float z){
            this->body = body;
            this->gun = gun;
            this->tracks = tracks;
            this->turret = turret;
            this->x = x;
            this->y = 0.5f;
            this->z = z;
        }

        Tank(){
        }
    };

    class Projectile{
        public:
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float angle;
        float scale;
        float speed;
        Mesh* mesh;
        bool isAlive = true;
        float ttl = 2.0f;

        Projectile(Mesh *mesh, float x, float y, float z){
            this->x = x;
            this->y = y;
            this->z = z;
            this->mesh = mesh;
        }

        Projectile(){
        }
    };

    class Building{
    public:

        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        Mesh *mesh;
        float radius;

        Building(Mesh *mesh, float x, float y, float z) {
            this->mesh = mesh;
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Building(){ }
    };
}