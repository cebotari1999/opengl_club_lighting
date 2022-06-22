#include "lab_m1/tema3/tema3.h"

#include <vector>
#include <string>
#include <iostream>

# define PI           3.14159265358979323846  /* pi */


using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()

{
}



void Tema3::Init()
{
    {
        lightDirection = glm::vec3(0, -1, 0);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    {
        cutOff = 15;
        sphereRotationAngle = 0;
        coneRotationAngle = 0;
        incrementRotation = 0.001;
    }

    {
        addTexture = 0;
        mode = 0;
        isFloor = 0;
    }

    x = 1;
    y = 0;
    time = 0;

    InitMeshes();
    InitTexture();
    InitCone();
    InitFloorPositions();
    InitPositions();
}

void Tema3::InitMeshes() {
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bunny");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

void Tema3::InitTexture() {
    {
        mapTextures["random"] = CreateRandomTexture(25, 25);
    }



    {
        vector<glm::vec2> textureCoords
        {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f)

        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

    }
}

void Tema3::InitCone() {
    {

        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        int  i = 0;
        float angle, x, y;

        for (angle = 0.0f; angle <= (2.0f * PI); angle += 0.1)
        {
            x = 2 * sin(angle);
            y = 2 * cos(angle);
            vertices.push_back(VertexFormat(glm::vec3(x, -6, y)));
            indices.push_back(-6);
            indices.push_back(i);
            indices.push_back(i + 1);
            i++;
        }

        x = 2 * sin(angle);
        y = 2 * cos(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, -6, y)));
        indices.push_back(-6);
        indices.push_back(i);
        indices.push_back(i + 1);

        CreateMesh("cone", vertices, indices);
    }
}

void Tema3::InitFloorPositions() {
    {
        int count = 0;
        for (int i = 1; i < 9; i++) {
            for (int j = 1; j < 9; j++) {
                lightPosition[count] = glm::vec3(i, 1, j);
                float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.9));
                float g = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.9));
                float b = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.9));
                colors[count] = glm::vec3(r, g, b);
                count++;
            }
        }

    }
}

void Tema3::InitPositions() {
    {
        for (int i = 0; i < 6; i++) {
            dancersPositions[i] = glm::vec3(2, 2, 2);
        }

        for (int i = 0; i < 6; i++) {
            nextDancersPositions[i] = glm::vec3(1 + i, 2, 7 - i);
        }
    }

    {
        spotLights[0] = glm::vec3(2, 7, 2);
        spotLights[1] = glm::vec3(2, 7, 7);
        spotLights[2] = glm::vec3(7, 7, 2);
        spotLights[3] = glm::vec3(7, 7, 7);
    }
}

void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   
    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::FrameEnd()
{
    // DrawCoordinateSystem();
}

void Tema3::MoveDancers() {
    for (int i = 0; i < 6; i++) {
        if (dancersPositions[i].x < nextDancersPositions[i].x) {
            dancersPositions[i].x += 0.01f;
        }

        if (dancersPositions[i].x > nextDancersPositions[i].x) {
            dancersPositions[i].x -= 0.01f;
        }

        if (dancersPositions[i].z < nextDancersPositions[i].z) {
            dancersPositions[i].z += 0.01f;
        }

        if (dancersPositions[i].z > nextDancersPositions[i].z) {
            dancersPositions[i].z -= 0.01f;
        }



        if (fabs(dancersPositions[i].x - nextDancersPositions[i].x) < 1
            && fabs(dancersPositions[i].z - nextDancersPositions[i].z) < 1)
        {
            nextDancersPositions[i].x = static_cast<float> (rand() % 7 + 1);
            nextDancersPositions[i].z = static_cast<float> (rand() % 7 + 1);

            while (fabs(dancersPositions[i].x - nextDancersPositions[i].x) > 6
                || fabs(dancersPositions[i].z - nextDancersPositions[i].z) > 6) {
                nextDancersPositions[i].x = static_cast<float> (rand() % 9 + 1);
                nextDancersPositions[i].z = static_cast<float> (rand() % 9 + 1);
            }
        }
    }
}

void Tema3::RenderFloor() {
    if (time == 0) {
        for (int i = 0; i < N; i++) {
            float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.9));
            float g = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.9));
            float b = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.9));
            newColors[i] = glm::vec3(r, g, b);
        }
        time = 0.001;
    }
    else {
        for (int i = 0; i < N; i++) {
            colors[i] = glm::mix(colors[i], newColors[i], time / 5);
        }

        time += 0.001;

        if (time > 0.2) {
            time = 0;
        }
    }

    isFloor = 1;
    for (int i = 0; i < N; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, lightPosition[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, NULL,  colors[i]);
    }

    isFloor = 0;
}

void Tema3::RenderDancers() {
    MoveDancers();

    for (int i = 0; i < 6; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, dancersPositions[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));
        RenderSimpleMesh(meshes["bunny"], shaders["LabShader"], modelMatrix, NULL, glm::vec3(1, 1, 1));
    }
}

void Tema3::RenderGlobe() {
   
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(4.5, 5.5, 4.5));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
        sphereRotationAngle += 0.001;
        addTexture = 1;
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["random"], glm::vec3(1, 1, 1));
        addTexture = 0;

}

void Tema3::RenderWalls() {
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 7; j++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, j, 0));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, NULL, glm::vec3(1, 1, 1));

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, j, 9));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, NULL, glm::vec3(1, 1, 1));

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(9, j, i));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, NULL, glm::vec3(1, 1, 1));
        }
    }

    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 7, j));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, NULL, glm::vec3(1, 1, 1));

        }
    }
}

void Tema3::RenderCones() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    for (int i = 0; i < 4; i++) {

        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec3 position = spotLights[i];
        position.y -= 1;


        if (fabs(coneRotationAngle - 0.3) < 0.05) {
            incrementRotation = -0.001;
        }

        if (fabs(coneRotationAngle + 0.3) < 0.05) {
            incrementRotation = 0.001;
        }

        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.8f, 0.5f));
        modelMatrix = glm::rotate(modelMatrix, coneRotationAngle, glm::vec3(x, 0, y));
        RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, NULL, glm::vec3(1, 1, 1));

        coneRotationAngle += incrementRotation;
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}

void Tema3::Update(float deltaTimeSeconds)
{

    if (mode == 0) {

        RenderFloor();
        RenderDancers();
        RenderGlobe();
        RenderWalls();
        RenderCones();

    }

    if (mode == 1) {
        RenderFloor();
        RenderWalls();
        RenderDancers();
    }

    if (mode == 2) {
        RenderFloor();
        RenderWalls();
        RenderDancers();
        RenderCones();
    }

    if (mode == 3) {
        RenderFloor();
        RenderDancers();
        RenderGlobe();
        RenderWalls();

    }
}

Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    for (int i = 0; i < size; i++) {
        data[i] = rand() % 1000;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);


    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

Mesh* Tema3::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));


    glBindVertexArray(0);

    CheckOpenGLError();

    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    meshes[name]->SetDrawMode(GL_TRIANGLE_FAN);
    return meshes[name];
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, 
    Texture2D* texture1, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    glUniform3fv(glGetUniformLocation(shader->program, "lights_positions"), N, glm::value_ptr(lightPosition[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "lights_colors"), N, glm::value_ptr(colors[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "spot_lights"), 4, glm::value_ptr(spotLights[0]));
    glUniform1i(glGetUniformLocation(shader->program, "add_texture"), addTexture);
    glUniform1i(glGetUniformLocation(shader->program, "mode"), mode);
    glUniform1f(glGetUniformLocation(shader->program, "sphere_rotation_angle"), sphereRotationAngle);
    glUniform1i(glGetUniformLocation(shader->program, "is_floor"), isFloor);
    glUniform1f(glGetUniformLocation(shader->program, "cone_roation_angle"), coneRotationAngle);
    glUniform1f(glGetUniformLocation(shader->program, "x"), x);
    glUniform1f(glGetUniformLocation(shader->program, "y"), y);
 
    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);


    int cut_off_input = glGetUniformLocation(shader->program, "cut_off_input");
    glUniform1f(cut_off_input, cutOff);


    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        int texture_location_1 = glGetUniformLocation(shader->program, "texture_1");
        glUniform1i(texture_location_1, 0);


    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}



void Tema3::OnInputUpdate(float deltaTime, int mods)
{
 
}


void Tema3::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) {
        mode++;

        if (mode == 4) {
            mode = 0;
        }
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
