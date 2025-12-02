#include "MeshParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

namespace gl {

    Mesh MeshParser::loadModel(const std::string& path) {  
      gl::Mesh mo;
      gl::MeshData md;
      if(!parseFile(path, md)) {
        throw std::runtime_error("Could not load model: " + path);
      }
      mo.upload(md);
      return mo;
    }

    bool MeshParser::parseFile(const std::string& path, MeshData& out) {
        std::ifstream file(path);
        if (!file) {
            std::cerr << "Error: cannot open " << path << "\n";
            return false;
        }

        enum Section { NONE, VERTEX, INDEX };
        Section section = NONE;
        bool headerParsed = false;
        std::string line;

        auto trim = [](std::string& s) {
            while (!s.empty() && std::isspace((unsigned char)s.back()))
                s.pop_back();
            size_t start = 0;
            while (start < s.size() && std::isspace((unsigned char)s[start]))
                ++start;
            s = s.substr(start);
            };

        while (std::getline(file, line)) {
            // Remove comments
            auto hash = line.find('#');
            if (hash != std::string::npos)
                line = line.substr(0, hash);

            trim(line);
            if (line.empty())
                continue;

            // Metadata: @key:value
            if (line[0] == '@') {
                auto colon = line.find(':');
                if (colon != std::string::npos) {
                    std::string key = line.substr(1, colon - 1);
                    std::string val = line.substr(colon + 1);
                    trim(key);
                    trim(val);
                    out.metadata[key] = val;
                }
                continue;
            }

            // Section headers
            if (line == ":vertex") {
                section = VERTEX;
                headerParsed = false;
                continue;
            }
            if (line == ":index") {
                section = INDEX;
                continue;
            }

            // Read vertex section
            if (section == VERTEX) {
                // First line in vertex section defines layout
                if (!headerParsed) {
                    std::istringstream ss(line);
                    std::string token;
                    while (ss >> token) {
                        if (token.rfind("float", 0) == 0) {
                            int comps = std::stoi(token.substr(token.find('(') + 1));
                            out.layout.add<float>(comps);
                        }
                        else if (token.rfind("int", 0) == 0) {
                            int comps = std::stoi(token.substr(token.find('(') + 1));
                            out.layout.add<int>(comps);
                        }
                        else if (token.rfind("uint", 0) == 0) {
                            int comps = std::stoi(token.substr(token.find('(') + 1));
                            out.layout.add<unsigned int>(comps);
                        }
                        else if (token.rfind("double", 0) == 0) {
                            int comps = std::stoi(token.substr(token.find('(') + 1));
                            out.layout.add<double>(comps);
                        }
                    }
                    headerParsed = true;
                    continue;
                }

                // Vertex data lines
                std::istringstream ss(line);
                float val;
                while (ss >> val)
                    out.vertices.push_back(val);
            }

            // Read index section
            else if (section == INDEX) {
                std::istringstream ss(line);
                unsigned int idx;
                while (ss >> idx)
                    out.indices.push_back(idx);
            }
        }

        return true;
    }

}
