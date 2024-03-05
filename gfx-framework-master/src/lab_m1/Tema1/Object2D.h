// TEMA 1 EGC 331CB IMBREA GIULIA STEFANIA
#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace Object2D
{
    Mesh* CreateSquare(const std::string &name, bool fill = false);
    Mesh* CreateRhombus(const std::string &name);
    Mesh* CreateStar(const std::string &name);
    Mesh* CreateHexagon(const std::string &name, float z);
    Mesh* CreateHeart(const std::string& name, bool fill = false);
    Mesh* CreateSpiral(const std::string& name);
}

