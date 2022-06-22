#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5, 6, 7 };
    int i = 0;
    for (float angle = 0.0f; angle <= (2.0f * 3.14); angle += 0.01f)
    {
        float x = 50.0f * sin(angle);
        float y = 50.0f * cos(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0.0f), color));
        indices.push_back(i);
        i++;
    }
    

    Mesh* square = new Mesh(name);

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
