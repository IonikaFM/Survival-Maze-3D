#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace Object3D
{
    std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> CreateCube(glm::vec3 color);
}
