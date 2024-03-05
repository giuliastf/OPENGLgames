// TEMA 1 EGC 331CB IMBREA GIULIA STEFANIA
#include "Object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* Object2D::CreateSquare(
    const std::string &name,
    bool fill)
{
    glm::vec3 corner = glm::vec3(0, 0, 0);
    glm::vec3 color = glm::vec3(0,0,0);
    float halfSide = 0.5f;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-halfSide, -halfSide, 0), color),
        VertexFormat(corner + glm::vec3(halfSide, -halfSide, 0), color),
        VertexFormat(corner + glm::vec3(halfSide, halfSide, 0), color),
        VertexFormat(corner + glm::vec3(-halfSide, halfSide, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3};

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* Object2D::CreateRhombus(const std::string& name)
{

    float size1 = 1.0f;
    float size2 = size1/ 2.0f;
    glm::vec3 center = glm::vec3(0, 0, 1);
    glm::vec3 color = glm::vec3(0,0,0);
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(0, size1, 1), color),    // top 
        VertexFormat(center + glm::vec3(-size2, 0, 1), color),   // Left
        VertexFormat(center + glm::vec3(0, -size1, 1), color),   // Bottom
        
        
        VertexFormat(center + glm::vec3(size2 / 2.0f, -0.35f * size1, 1), color),    // bottom left
        VertexFormat(center + glm::vec3(1.5f*size2, -0.35f * size1, 1), color),    //  bottom right
        VertexFormat(center + glm::vec3(1.5f*size2, 0.35f * size1, 1), color),    // up right
        VertexFormat(center + glm::vec3(size2 / 2.0f, 0.35f * size1, 1), color),   //up left

    };

    Mesh* rhombus = new Mesh(name);
    std::vector<unsigned int> indices = { 1, 0, 6, 1, 5, 6, 1, 4, 5, 1, 3, 4, 1, 2, 3};

    
    rhombus->SetDrawMode(GL_TRIANGLE_FAN);
    

    rhombus->InitFromData(vertices, indices);

    return rhombus;
}

Mesh* Object2D::CreateStar(
    const std::string &name)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 color = glm::vec3(0,0,0);
    const int numPoints = 5;
    glm::vec3 center = glm::vec3(0, 0, 2);
    float outerRadius = 30.0f;
    float innerRadius = 60.0f;

    float angleIncrement = 72.0f;// 2.0f * M_PI / numPoints;
    float startAngle = glm::radians(-90.0f);
    
    for (int i = 0; i < numPoints; ++i) {
        float outerAngle = startAngle + glm::radians(i * angleIncrement);
        float innerAngle = startAngle + glm::radians(i * angleIncrement + angleIncrement / 2.0f);

        glm::vec3 outerVertex = center + glm::vec3(outerRadius * cos(outerAngle), outerRadius * sin(outerAngle), 1.0f);
        glm::vec3 innerVertex = center + glm::vec3(innerRadius * cos(innerAngle), innerRadius * sin(innerAngle), 1.0f);

        vertices.push_back(VertexFormat(outerVertex, color));
        vertices.push_back(VertexFormat(innerVertex, color));
    }

    Mesh* star = new Mesh(name);



    star->SetDrawMode(GL_TRIANGLE_FAN);
    for(int i = 0; i<10; i++)
        indices.push_back(i);
    
    star->InitFromData(vertices, indices);

    return star;
}

Mesh* Object2D::CreateHexagon(const std::string& name, float z)
{
    float size = 100;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 color = glm::vec3(0,0,0);
    glm::vec3 center = glm::vec3(0,0,z);
    
    const int numPoints = 6;
    float angleIncrement = 60.0f;
    for(int i=0; i < numPoints; i++)
    {
        float angle = glm::radians(i * angleIncrement);
        glm::vec3 vertex = center + glm::vec3(size * cos(angle), size * sin(angle), 2.0f);
        vertices.push_back(VertexFormat(vertex, color));
    }

    Mesh* hexagon = new Mesh(name);

    hexagon->SetDrawMode(GL_TRIANGLE_FAN);
    for(int i = 0; i<6; i++)
    indices.push_back(i);
    
    hexagon->InitFromData(vertices, indices);
    return hexagon;
}

// Http://mathworld.wolfram.com/HeartCurve.html
Mesh* Object2D::CreateHeart(const std::string& name, bool fill)
{
    glm::vec3 corner = glm::vec3(0, 0, 0);
    glm::vec3 color = glm::vec3(1, 0, 0);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int steps = 50;
    for (int i = 0; i < steps; i++) {
        float t = 2.0f * M_PI * float(i) / float(steps);
        float x = 10 * 16 * pow(sin(t), 3);
        float y = 10 * (13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t));
        vertices.push_back(VertexFormat(glm::vec3(x, y, 3), color));
    }

    if (fill) {
        indices.push_back(0);
        for (int i = 1; i < steps; i++) {
            indices.push_back(i);
        }
        indices.push_back(1);
    } else {
        for (int i = 1; i < steps; i++) {
            indices.push_back(i);
        }
        indices.push_back(1);
    }

    Mesh* heart = new Mesh(name);

    if (fill) {
        heart->SetDrawMode(GL_TRIANGLE_FAN);
    } else {
        heart->SetDrawMode(GL_LINE_STRIP);
    }

    heart->InitFromData(vertices, indices);

    return heart;
}


Mesh* Object2D::CreateSpiral(const std::string& name)
{
    glm::vec3 color = glm::vec3(0, 0, 0);
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float radius = 0.0f;
    for (float angle = 0; angle < 1440; angle += 1)
    {
        float x = cos(angle * M_PI / 180) * radius;
        float y = sin(angle * M_PI / 180) * radius;
        radius += 0.1f;
        vertices.push_back(VertexFormat(glm::vec3(x, y, 3), color));
    }

    for (unsigned int i = 0; i < vertices.size(); ++i)
    {
        indices.push_back(i);
    }

    Mesh* spiral = new Mesh(name);

    spiral->SetDrawMode(GL_LINE_STRIP);
    spiral->InitFromData(vertices, indices);

    return spiral;
}
