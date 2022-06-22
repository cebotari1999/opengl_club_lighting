#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
    x = -1;
    y = 0;
    z = -24;
}


Tema2::~Tema2()
{
}

void Tema2::InitVisited() {

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            visited[i][j] = 0;
        }
    }
}

void Tema2::InitMaze() {
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 24; j++) {
            maze[i][j] = 0;

        }

    }

    for (int i = 0; i < 11; i++) {
        maze[i][0] = 1;
        maze[0][i] = 1;
        maze[i][23] = 1;
        maze[23][i] = 1;

    }

    for (int i = 13; i < 24; i++) {
        maze[i][0] = 1;
        maze[0][i] = 1;
        maze[i][23] = 1;
        maze[23][i] = 1;
    }
}

void Tema2::CompleteMaze() {

    int startX, startY, count = 0;

    startX = rand() % 9;
    startY = rand() % 9;


    while (count < 89) {
        int oldStartX, oldStartY;
        oldStartX = startX;
        oldStartY = startY;

        if (visited[oldStartX][oldStartY] == 0) {
            // labirinth[2 * startX - 1][2 * startY - 1] = 1;
            visited[oldStartX][oldStartY] = 1;
            count++;

        }

        if (startX > 0 && startX < 9 && startY > 0 && startY < 9) {
            startX += -1 + rand() % 3;
            startY += -1 + rand() % 3;
        }

        if (startX == 0) {
            startX += rand() % 2;

            if (startY == 0) {
                startY += rand() % 2;
            }

            if (startY == 9) {
                startY += -1 + rand() % 2;
            }

            if (startY > 0 && startY < 9) {
                startY += -1 + rand() % 3;
            }

        }


        if (startX == 9) {
            startX += -1 + rand() % 2;

            if (startY == 0) {
                startY += rand() % 2;
            }

            if (startY == 9) {
                startY += -1 + rand() % 2;
            }

            if (startY > 0 && startY < 9) {
                startY += -1 + rand() % 3;
            }

        }


        if (startY == 0) {
            startY += rand() % 2;

            if (startX == 0) {
                startX += rand() % 2;
            }

            if (startX == 9) {
                startX += -1 + rand() % 2;
            }

            if (startX > 0 && startX < 9) {
                startX += -1 + rand() % 3;
            }

        }

        if (startY == 9) {
            startY += -1 + rand() % 2;

            if (startX == 0) {
                startX += rand() % 2;
            }

            if (startX == 9) {
                startX += -1 + rand() % 2;
            }

            if (startX > 0 && startX < 9) {
                startX += -1 + rand() % 3;
            }

        }

        if (visited[startX][startY] == 0) {

            if (oldStartY == startY) {
                maze[2 * oldStartX + 3][2 * oldStartY + 2] = 1;
                maze[2 * oldStartX + 3][2 * oldStartY + 3] = 1;


            }

            if (oldStartY != startY) {
                maze[2 * oldStartX + 2][2 * oldStartY + 3] = 1;
                maze[2 * oldStartX + 3][2 * oldStartY + 3] = 1;

            }

        }
    }

}


void Tema2::Init()
{
    {
        InitVisited();
        InitMaze();
        CompleteMaze();
    }

    {
        renderCameraTarget = false;
        camera = new implemented::Camera();
        camera->Set(glm::vec3(0, 7,  -6 + z), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
        left = -5;
        right = 5;
        bottom = -5;
        top = 5;
        zNear = 1;
        zFar = 30;
        fov = 80.0f;
        aspectRatio = 16 / 9.0f;
        angle = 0;
        direction1 = 0;
        direction2 = 0;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, zNear, zFar);

    }

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

  
    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab8", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab8", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        lightPosition = glm::vec3(x, 3, z);
        lightDirection = glm::vec3(0, -1, 0);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;

        spotLight = 0;
        cutOff = 30;
    }
    
}

void Tema2::DrawMaze() {
    glm::mat4 modelMatrix;

    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 24; j++) {
            if (maze[i][j] == 1) {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(2 * i - 24, 1, 2 * j - 24));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 0));
            }
        }
    }
}

void Tema2::DrawPlayer() {
    glm::mat4 modelMatrix;
  

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 0.5, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.9, 0.3));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0.3, 1));
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4 + x, 0.5, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.9, 0.3));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0.3, 1));
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2 + x, 1.4, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7, 0.8, 0.3));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.1, 0.3, 0.3));
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2 + x, 2, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.35, 0.3));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.8, 0.7));
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.73 + x, 1.6, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.4, 0.3));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.1, 0.3, 0.3));
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.33 + x, 1.6, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.4, 0.3));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.1, 0.3, 0.3));
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.73 + x, 1.17, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.4, 0.3));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.8, 0.7));
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.33 + x,  1.17, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.4, 0.3));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.8, 0.7));
    }
    
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    DrawMaze();
    DrawPlayer();

    {
        glm::mat4 modelMatrix;
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 0, 50));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1));
    }
 
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, x, 3, z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = camera->position;
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

    // TODO(student): Set any other shader uniforms that you need
    int spot_light = glGetUniformLocation(shader->program, "spot_light");
    glUniform1i(spot_light, spotLight);

    int cut_off_input = glGetUniformLocation(shader->program, "cut_off_input");
    glUniform1f(cut_off_input, cutOff);


    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));


    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 2;

   
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->TranslateForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->TranslateForward(cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }
    }

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
       
        if (window->KeyHold(GLFW_KEY_W)) {
            z += deltaTime * speed;
            camera->TranslateForward(speed * deltaTime);
        } 

        if (window->KeyHold(GLFW_KEY_A)) {
            x += deltaTime * speed;
            camera->TranslateRight(-speed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            z -= deltaTime * speed;
            camera->TranslateForward(-speed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            x -= deltaTime * speed;
            camera->TranslateRight(speed * deltaTime);
        }
        
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event

    // TODO(student): Set keys that you might need
    if (key == GLFW_KEY_F) {
        if (spotLight == 0) {
            spotLight = 1;
        }
        else {
            spotLight = 0;
        }
    }

   

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            camera->RotateFirstPerson_OX(-sensivityOY * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOX * deltaX);
           // angle += 2;
            direction1 -= deltaX* 0.05;
            direction2 -= deltaY * 0.05;

        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            camera->RotateThirdPerson_OX(-sensivityOY * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOX * deltaX);
           // angle -= 2;
            direction1 += deltaX * 0.05;
            direction2 += deltaY * 0.05;

        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
