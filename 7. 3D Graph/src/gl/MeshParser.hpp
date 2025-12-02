#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "vertex_layout.hpp"
#include "Mesh.hpp"

namespace gl {

    struct MeshData {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        vertex_layout layout;
        std::unordered_map<std::string, std::string> metadata;
    };

    class MeshParser {
    public:
        static bool parseFile(const std::string& path, MeshData& out);

        static Mesh loadModel(const std::string& path);
    };

}
