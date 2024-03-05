// TEMA 1 EGC 331CB IMBREA GIULIA STEFANIA
#pragma once

#include "components/simple_scene.h"

namespace m1{

    class Square{ 
    public:

        glm::vec3 center;
        float size;
        glm::vec3 color;
        Mesh *mesh;

        Square(Mesh *mesh, float size, glm::vec3 center, glm::vec3 color) {
            this->mesh = mesh;
            this->size = size;
            this->center = center;
            this->color = color;
        }
        Square (){}

        bool CheckClick(int mouseX, int mouseY){
            float cond_a = (mouseX > center.x - size/2);
            float cond_b = (mouseX < center.x + size/2);
            float cond_c = (mouseY > center.y - size/2);
            float cond_d = (mouseY < center.y + size/2);

            return cond_a && cond_b && cond_c  && cond_d;
        }
    };


    class Rhombus{
    public:

        glm::vec3 center;
        float size;
        int price;
        glm::vec3 color;    
        Mesh *mesh; 
        bool isAlive = true;
        int row;
        float time = 1.0f;
        bool fire = true;
        int grid;

        Rhombus(Mesh *mesh, float size, int price, glm::vec3 center, glm::vec3 color) {
            this->mesh = mesh;
            this->size = size;
            this->price = price;
            this->center = center;
            this->color = color;
        }

        Rhombus(){ }

        void TakeABreath(float deltaTimeSeconds) {
            if(time > 0)
                time -= deltaTimeSeconds;
            else
                time = 0;
        }

    };

    class GUISquare{
    public:

        Square square;
        Rhombus rhombus;

        GUISquare(Square square, Rhombus rhombus){
            this->square = square;
            this->rhombus = rhombus;
        }
        GUISquare(){}

        
    };


    class Star{
    public:

        int type;
        float size;
        float real_size = 60.0f;
        glm::vec3 color;
        Mesh *mesh;
        glm::vec3 center;
        float ttl = 8;
        float spin = 0;
        int row;
        bool isAlive = true;
 
        Star(Mesh* mesh, float size, glm::vec3 center, glm::vec3 color) {
            this->center = center;
            this->size = size;
            this->color = color;
            this->mesh = mesh;
        }

        Star(){

        }

        bool checkClick(int mouseX, int mouseY){
            float distance = sqrt((center.x - mouseX) * (center.x - mouseX) + (center.y - mouseY) * (center.y - mouseY));
            return distance < size*real_size;
        }
    };
    
    class GridSquare{
        public:
            Square square;
            Rhombus rhombus;  
            bool occupied = false;
            int row;

            GridSquare(Square square, int row){
                this->square = square;
                this->row = row;
            }

            GridSquare(){}
    };

    class Hexagon{
    public:

        glm::vec3 center;
        float size;
        glm::vec3 color;    
        Mesh *mesh; 

        Hexagon(Mesh *mesh, float size, glm::vec3 center, glm::vec3 color) {
            this->center = center;
            this->size = size;
            this->color = color;
            this->mesh = mesh;
        }

        Hexagon(){

        }
    };

    class Enemy{
    public:

        Hexagon outerHexa;
        Hexagon innerHexa;
        glm::vec3 color;
        int row;
        int life = 3;
        bool isAlive = true;

        Enemy(Hexagon outer, Hexagon inner, glm::vec3 color, int row){
            this->outerHexa = outer;
            this->innerHexa = inner;
            this->color = color;
            this->row = row;
        }

        Enemy(){
        }
    };

    class Heart{
    public:
        Mesh *mesh;
        glm::vec3 center;
        float scale;
        glm::vec3 color;
        bool fill =  true;

        Heart(Mesh *mesh, float size, glm::vec3 center, glm::vec3 color) {
            this->center = center;
            this->scale = size;
            this->color = color;
            this->mesh = mesh;
        }

        Heart(){

        }   
    };

}