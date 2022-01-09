#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = true;

    camera = new implemented::MyCamera();

    posX = std::rand() % (matrixSize - 2) + 1;
    posZ = std::rand() % (matrixSize - 2) + 1;

    camera->Set(glm::vec3(posX * 4 + 1, 0.8, posZ * 4 + 1), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(0.458, 0.219, 0.219));
        CreateMesh("floor_red", mypair.first, mypair.second);
    }
    
    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(1, 1, 0));
        CreateMesh("floor_yellow", mypair.first, mypair.second);
    }

    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(1, 0.855, 0.725));
        CreateMesh("cube_tan", mypair.first, mypair.second);
    }
    
    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(0, 0.4, 0.8));
        CreateMesh("cube_jeans", mypair.first, mypair.second);
    }

    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(0.294, 0, 0.510));
        CreateMesh("cube_shirt", mypair.first, mypair.second);
    }

    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(1, 0, 0));
        CreateMesh("cube_enemy", mypair.first, mypair.second);
    }

    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(0, 0.2, 0));
        CreateMesh("cube_obstacle", mypair.first, mypair.second);
    }

    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(0.5, 0.5, 0.5));
        CreateMesh("bullet", mypair.first, mypair.second);
    }

    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(1, 0, 0));
        CreateMesh("healthBar", mypair.first, mypair.second);
    }

    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(0, 0, 0));
        CreateMesh("healthBarBorder", mypair.first, mypair.second);
    }

    {
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = CreateCube(glm::vec3(0.5, 0.5, 0.5));
        CreateMesh("timer", mypair.first, mypair.second);
    }

    InitLabirinth();

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}


std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> Tema2::CreateCube(glm::vec3 color)
{
    {
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, -1,  1), color, glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(1, -1,  1), color, glm::vec3(0.9, 0.4, 0.2)),
            VertexFormat(glm::vec3(-1,  1,  1), color, glm::vec3(0.7, 0.7, 0.1)),
            VertexFormat(glm::vec3(1,  1,  1), color, glm::vec3(0.7, 0.3, 0.7)),
            VertexFormat(glm::vec3(-1, -1, -1), color, glm::vec3(0.3, 0.5, 0.4)),
            VertexFormat(glm::vec3(1, -1, -1), color, glm::vec3(0.5, 0.2, 0.9)),
            VertexFormat(glm::vec3(-1,  1, -1), color, glm::vec3(0.7, 0.0, 0.7)),
            VertexFormat(glm::vec3(1,  1, -1), color, glm::vec3(0.1, 0.5, 0.8)),
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            0, 4, 1,        1, 4, 5,
            2, 6, 4,        0, 2, 4,
        };

        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> mypair = std::make_pair(vertices, indices);

        return mypair;
    }
}


void Tema2::InitLabirinth()
{
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            if (i != 0 && j != 0 && i != matrixSize - 1 && j != matrixSize - 1)
            {
                maze[i][j] = Maze();
            }
            else
            {
                maze[i][j] = Maze(0, true);
                visited.push_back(make_pair(maze[i][j], make_pair(i, j)));
            }
        }
    }
    maze[posX][posZ] = Maze(1, true);
    vector<std::pair<Tema2::Maze, std::pair<int, int>>> neighbours = GetNeighbours(posX, posZ);
    way.push_back(make_pair(maze[posX][posZ], make_pair(posX, posZ)));
    int random = std::rand() % neighbours.size();
    std::pair<Tema2::Maze, std::pair<int, int>> chosenNeighbour = neighbours[random];
    maze[chosenNeighbour.second.first][chosenNeighbour.second.second] = Maze(1, true);
    chosenNeighbour.first = Maze(1, true);
    visited.push_back(chosenNeighbour);
    way.push_back(chosenNeighbour);

    while(visited.size() < matrixSize * matrixSize && way.size() != 0)
    {
        if (neighbours.size() == 0) 
        {   
            while (neighbours.size() == 0 && way.size() != 0)
            {
                random = std::rand() % way.size();
                neighbours = GetNeighbours(way[random].second.first, way[random].second.second);
                if (neighbours.size() == 0)
                {
                    way.erase(way.begin() + random);
                }
                else
                {
                    chosenNeighbour = way[random];
                }
            }
        }
        while (neighbours.size() != 0)
        {
            neighbours.clear();
            neighbours = GetNeighbours(chosenNeighbour.second.first, chosenNeighbour.second.second);

            if (neighbours.size())
            {
                do {
                    random = std::rand() % neighbours.size();
                    chosenNeighbour = neighbours[random];
                    neighbours.erase(neighbours.begin() + random);
                } while (CanChooseThisNeighbour(chosenNeighbour.second.first, chosenNeighbour.second.second) == false && neighbours.size() != 0);

                if (CanChooseThisNeighbour(chosenNeighbour.second.first, chosenNeighbour.second.second) == true)
                {
                    maze[chosenNeighbour.second.first][chosenNeighbour.second.second] = Maze(1, true);
                    chosenNeighbour.first = Maze(1, true);
                    neighbours.push_back(chosenNeighbour);
                    visited.push_back(chosenNeighbour);
                    way.push_back(chosenNeighbour);
                }
            }
        }
    }

    do {
        random = std::rand() % (matrixSize - 2) + 1;
    } while (maze[1][random].cellType == 0);
    maze[0][random].cellType = 2;

    do {
        random = std::rand() % (matrixSize - 2) + 1;
    } while (maze[matrixSize - 2][random].cellType == 0);
    maze[matrixSize - 1][random].cellType = 2;

    do {
        random = std::rand() % (matrixSize - 2) + 1;
    } while (maze[random][1].cellType == 0);
    maze[random][0].cellType = 2;

    do {
        random = std::rand() % (matrixSize - 2) + 1;
    } while (maze[random][matrixSize - 2].cellType == 0);
    maze[random][matrixSize - 1].cellType = 2;


    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            if (maze[i][j].cellType == 0) {
                walls.push_back(make_pair(i, j));
            }
        }
    }

    for (int i = 0; i < 25; i++) {
        int rand1 = std::rand() % (matrixSize - 2) + 1;
        int rand2 = std::rand() % (matrixSize - 2) + 1;
        if (maze[rand1][rand2].cellType != 0 && (rand1 != posX || rand2 != posZ)) {
            Enemy enemy(rand1 * 4 - 1.5, rand2 * 4 - 1.5);
            enemies.push_back(enemy);
        }
        else 
            i--;
    }
}


vector<std::pair<Tema2::Maze, std::pair<int, int>>> Tema2::GetNeighbours(int x, int y)
{
    vector<std::pair<Tema2::Maze, std::pair<int, int>>> neighbours;
    if (x > 0 && maze[x - 1][y].visited == false) {
        neighbours.push_back(make_pair(maze[x - 1][y], make_pair(x - 1, y)));
    }
    if (x < matrixSize - 1 && maze[x + 1][y].visited == false) {
        neighbours.push_back(make_pair(maze[x + 1][y], make_pair(x + 1, y)));
    }
    if (y > 0 && maze[x][y - 1].visited == false) {
        neighbours.push_back(make_pair(maze[x][y - 1], make_pair(x, y - 1)));
    }
    if (y < matrixSize - 1 && maze[x][y + 1].visited == false) {
        neighbours.push_back(make_pair(maze[x][y + 1], make_pair(x, y + 1)));
    }

    return neighbours;
}


bool Tema2::CanChooseThisNeighbour(int x, int y)
{
    int count = 0;
    if (x > 0 && maze[x - 1][y].cellType == 1) {
        count++;
    }
    if (x < matrixSize - 1 && maze[x + 1][y].cellType == 1) {
        count++;
    }
    if (y > 0 && maze[x][y - 1].cellType == 1) {
        count++;
    }
    if (y < matrixSize - 1 && maze[x][y + 1].cellType == 1) {
        count++;
    }
    if (count > 1)
    {
        if (maze[x][y].visited == false)
        {
            maze[x][y].visited = true;
            visited.push_back(make_pair(maze[x][y], make_pair(x, y)));
        }
        return false;
    }
    return true;
}


Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
    window->DisablePointer();
}


void Tema2::Update(float deltaTimeSeconds)
{
    if (playerHealth > 0 && timer > 0 && gameWon == false) {
        if (renderCameraTarget)
        {
            RenderPlayer();
        }

        if (direction.x == 1 && enemyOffsetX <= 3) {
            enemyOffsetX += deltaTimeSeconds * enemySpeed;
            if (enemyOffsetX >= 3) {
                direction.x = 0;
                direction.y = 1;
            }
        }

        if (direction.y == 1 && enemyOffsetZ <= 3) {
            enemyOffsetZ += deltaTimeSeconds * enemySpeed;
            if (enemyOffsetZ >= 3) {
                direction.y = 0;
                direction.z = 1;
            }
        }

        if (direction.z == 1 && enemyOffsetX >= 0) {
            enemyOffsetX -= deltaTimeSeconds * enemySpeed;
            if (enemyOffsetX <= 0) {
                direction.z = 0;
                direction.w = 1;
            }
        }

        if (direction.w == 1 && enemyOffsetZ >= 0) {
            enemyOffsetZ -= deltaTimeSeconds * enemySpeed;
            if (enemyOffsetZ <= 0) {
                direction.w = 0;
                direction.x = 1;
            }
        }

        for (int i = 0; i < enemies.size(); i++) {
            RenderEnemy(enemies[i]);
        }

        for (int i = 0; i < bullets.size(); i++) {
            bullets[i].position += bullets[i].forward * bullets[i].speed * deltaTimeSeconds;
            DrawBullet(bullets[i], deltaTimeSeconds);
        }

        for (int i = 0; i < enemies.size(); i++) {
            if (CheckEnemyPlayerCollision(enemies[i], camera->GetTargetPosition())) {
                enemies.erase(enemies.begin() + i);
                playerHealth--;
            }
        }

        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < enemies.size(); j++) {
                if (CheckEnemyBulletCollision(enemies[j], bullets[i])) {
                    enemies.erase(enemies.begin() + j);
                    bullets.erase(bullets.begin() + i);
                    cout << "You killed " << ++score;
                    if (score == 1) {
                        cout << " enemy !" << endl;
                    }
                    else {
                        cout<< " enemies !" << endl;
                    }
                    break;
                }
            }
        }

        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < walls.size(); j++)
            {
                if (CheckBulletWallCollision(bullets[i], walls[j]))
                {
                    bullets.erase(bullets.begin() + i);
                    break;
                }
            }
        }

        for (int i = 0; i < bullets.size(); i++) {
            if (!CheckBulletHeightPosition(bullets[i])) {
                bullets.erase(bullets.begin() + i);
            }
        }

        if (maze[(int)camera->GetTargetPosition().x / 4][(int)camera->GetTargetPosition().z / 4].cellType == 2) {
            cout << "You won !" << endl;
            gameWon = true;
        }

        if (fireRate <= 0) {
            fireRate = 0.25;
            canShoot = true;
        }
        else {
            fireRate -= deltaTimeSeconds;
        }

        if (timer) {
            timer -= deltaTimeSeconds;
        }

        DrawHealthBar();
        DrawTimer();
        RenderScene();
    }
    else if(gameWon == false){
        if (gameOver == false)
        {
            cout << "Game over !" << endl;
            gameOver = true;
        }
        DrawGameOver();
    }
    else {
        DrawYouWon();
    }
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Tema2::RenderPlayer()
{
    glm::vec3 position = camera->GetTargetPosition() - glm::vec3(0, 0.15, 0);

    //chest
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.22, 0.3, 0.1));
    RenderMesh(meshes["cube_shirt"], shaders["VertexColor"], modelMatrix);

    //left leg
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, - glm::vec3(0.12, 0.61, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.3, 0.1));
    RenderMesh(meshes["cube_jeans"], shaders["VertexColor"], modelMatrix);

    //right leg
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, - glm::vec3(-0.12, 0.61, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.3, 0.1));
    RenderMesh(meshes["cube_jeans"], shaders["VertexColor"], modelMatrix);

    //left top arm
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, -glm::vec3(0.34, -0.1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.2, 0.1));
    RenderMesh(meshes["cube_shirt"], shaders["VertexColor"], modelMatrix);

    //right top arm
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, -glm::vec3(-0.34, -0.1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.2, 0.1));
    RenderMesh(meshes["cube_shirt"], shaders["VertexColor"], modelMatrix);

    //left bottom arm
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, -glm::vec3(0.34, 0.21, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.1));
    RenderMesh(meshes["cube_tan"], shaders["VertexColor"], modelMatrix);

    //right bottom arm
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, -glm::vec3(-0.34, 0.21, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.1));
    RenderMesh(meshes["cube_tan"], shaders["VertexColor"], modelMatrix);

    //head
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, +glm::vec3(0, 0.46, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15, 0.15, 0.1));
    RenderMesh(meshes["cube_tan"], shaders["VertexColor"], modelMatrix);
}


void Tema2::RenderEnemy(Enemy enemy) {
    glm::mat4 modelMatrix = glm::mat4(1);
    enemy.position += glm::vec3(enemyOffsetX, 0, enemyOffsetZ);
    modelMatrix = glm::translate(modelMatrix, enemy.position);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, - glm::vec3(0, 0.2, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35, 0.6, 0.3));
    RenderMesh(meshes["cube_enemy"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, enemy.position);
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, + glm::vec3(0, 0.8, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25, 0.3, 0.3));
    RenderMesh(meshes["cube_enemy"], shaders["VertexColor"], modelMatrix);
}


void Tema2::RenderScene()
{
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            if (maze[i][j].cellType == 0) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i*4, 0, j*4));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
                RenderMesh(meshes["cube_obstacle"], shaders["VertexColor"], modelMatrix);
            }
            if (maze[i][j].cellType != 2) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i * 4, -1.1, j * 4));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1, 2));
                RenderMesh(meshes["floor_red"], shaders["VertexColor"], modelMatrix);
            }
            else {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i * 4, -1.1, j * 4));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1, 2));
                RenderMesh(meshes["floor_yellow"], shaders["VertexColor"], modelMatrix);
            }
        }
    }
}


void Tema2::DrawHealthBar()
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.95, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1 * playerHealth, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.95, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.05, 0.01));
    RenderMesh(meshes["healthBarBorder"], shaders["VertexColor"], modelMatrix);
}


void Tema2::DrawTimer() {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.82, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.005 * timer, 0.05, 0.01));
    RenderMesh(meshes["timer"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.82, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.05, 0.01));
    RenderMesh(meshes["healthBarBorder"], shaders["VertexColor"], modelMatrix);
}


void Tema2::DrawBullet(Bullet bullet, float deltaTimeSeconds)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, bullet.position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.1));
    RenderMesh(meshes["bullet"], shaders["VertexColor"], modelMatrix);
}


void Tema2::DrawGameOver() {
    //G
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 0.95, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.28, 0.7, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.2, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 0.45, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.8, 0.6, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.075, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.98, 0.62, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //A
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5, 0.65, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.33, 0.95, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.16, 0.65, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.33, 0.6, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //M
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.1, 0.65, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.27, 0.95, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.38, 0.7, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.1, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.49, 0.95, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.65, 0.65, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //E
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.17, 0.95, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.9, 0.7, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.2, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.17, 0.45, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.17, 0.7, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.16, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //O

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.035, 0, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.17, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.28, -0.25, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.2, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.035, -0.5, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.17, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.8, -0.25, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.2, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //V
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5, -0.2, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.16, -0.2, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.33, -0.5, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //E
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.1, -0.25, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.2, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.37, 0, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.37, -0.5, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.37, -0.25, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.16, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //R
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.9, -0.3, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.12, 0, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.33, -0.12, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.075, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.12, -0.25, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.33, -0.48, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.06, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.22, -0.38, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.06, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);
}


void Tema2::DrawYouWon() {
    //Y
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.7, 0.5, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.18, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.82, 0.75, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.06, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.94, 0.87, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.06, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.58, 0.75, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.06, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.46, 0.87, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.06, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //O
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.03, 0.88, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.17, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2, 0.62, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.2, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.03, 0.37, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.17, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.26, 0.62, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.2, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //U
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.55, 0.67, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.78, 0.37, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.17, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.015, 0.67, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //W
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.1, -0.2, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.93, -0.5, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.77, -0.33, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.1, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.6, -0.5, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.44, -0.2, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.25, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //O
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.03, 0, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.17, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2, -0.25, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.2, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.03, -0.5, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.17, 0.05, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.26, -0.25, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.2, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //N
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.55, -0.25, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.3, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.66, -0.1, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.06, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.78, -0.23, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.06, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.9, -0.36, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.06, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.015, -0.25, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.3, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    //!
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.3, -0.15, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.2, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.3, -0.49, 0.015));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.06, 0.01));
    RenderMesh(meshes["healthBar"], shaders["VertexColor"], modelMatrix);
}


bool Tema2::CheckPlayerWallCollision(std::pair<int, int> wall)
{
    glm::vec3 position = camera->GetTargetPosition();
    bool collisionX = wall.first * 4 + 2 >= position.x - 0.5 && wall.first * 4 - 2 <= position.x + 0.5;
    bool collisionZ = wall.second * 4 + 2 >= position.z - 0.5 && position.z + 0.5 >= wall.second * 4 - 2;
    bool edgeCollX = position.x <= 0 || position.x >= 98;
    bool edgeCollZ = position.z <= 0 || position.z >= 98;

    return (collisionX && collisionZ) || edgeCollX || edgeCollZ;
}


bool Tema2::CheckEnemyPlayerCollision(Enemy enemy, glm::vec3 position)
{
    enemy.position += glm::vec3(enemyOffsetX, 0, enemyOffsetZ);
    bool collisionX = enemy.position.x - 0.3 <= position.x + 0.1 && enemy.position.x + 0.3 >= position.x - 0.5;
    bool collisionZ = enemy.position.z - 0.2 <= position.z + 0.05 && enemy.position.z + 0.2 >= position.z - 0.05;

    return collisionX && collisionZ;
}


bool Tema2::CheckEnemyBulletCollision(Enemy enemy, Bullet bullet) {
    enemy.position += glm::vec3(enemyOffsetX, 0, enemyOffsetZ);
    bool collisionX = enemy.position.x - 0.3 <= bullet.position.x + 0.1 && enemy.position.x + 0.3 >= bullet.position.x - 0.1;
    bool collisionY = enemy.position.y - 0.5 <= bullet.position.y + 0.1 && enemy.position.y + 0.8 >= bullet.position.y - 0.1;
    bool collisionZ = enemy.position.z - 0.2 <= bullet.position.z + 0.1 && enemy.position.z + 0.2 >= bullet.position.z - 0.1;

    return collisionX && collisionY && collisionZ;;
}


bool Tema2::CheckBulletWallCollision(Bullet bullet, std::pair<int, int> wall) {
    bool collisionX = wall.first * 4 + 2 >= bullet.position.x - 0.1 && wall.first * 4 - 2 <= bullet.position.x + 0.1;
    bool collisionZ = wall.second * 4 + 2 >= bullet.position.z - 0.1 && bullet.position.z + 0.1 >= wall.second * 4 - 2;

    return collisionX && collisionZ;
}


bool Tema2::CheckBulletHeightPosition(Bullet bullet) {
    return bullet.position.y <= 5 && bullet.position.y >= - 0.9;
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 10.0f;
    bool collision;

    if (window->KeyHold(GLFW_KEY_W)) {
        camera->MoveForward(cameraSpeed * deltaTime);
        collision = false;
        for each (auto wall in walls)
        {
            if (CheckPlayerWallCollision(wall))
            {
                collision = true;
                break;
            }
        }
        if (collision == true)
        {
            camera->MoveForward(- cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        collision = false;
        camera->TranslateRight(-cameraSpeed * deltaTime);
        for each (auto wall in walls)
        {
            if (CheckPlayerWallCollision(wall))
            {
                collision = true;
                break;
            }
        }
        if (collision == true)
        {
            camera->TranslateRight(cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        collision = false;
        camera->MoveForward(-cameraSpeed * deltaTime);
        for each (auto wall in walls)
        {
            if (CheckPlayerWallCollision(wall))
            {
                collision = true;
                break;
            }
        }
        if (collision == true)
        {
            camera->MoveForward(cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        collision = false;
        camera->TranslateRight(cameraSpeed * deltaTime);
        for each (auto wall in walls)
        {
            if (CheckPlayerWallCollision(wall))
            {
                collision = true;
                break;
            }
        }
        if (collision == true)
        {
            camera->TranslateRight(- cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_SPACE) && canShoot && renderCameraTarget == false) {
        Bullet bullet(camera->GetTargetPosition(), camera->forward);
        bullets.push_back(bullet);
        canShoot = false;
        fireRate = 0.25;
    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) || window->GetSpecialKeyState() & GLFW_MOD_CONTROL)
    {
        renderCameraTarget = false;
    }
    else {
        renderCameraTarget = true;
    }
}


void Tema2::OnKeyPress(int key, int mods)
{

}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    if (renderCameraTarget == false) {
        camera->RotateFirstPerson_OY(- sensivityOY * deltaX);
        camera->RotateFirstPerson_OX(- sensivityOX * deltaY);
    }
    else  
    {
        camera->RotateThirdPerson_OY(- sensivityOY * deltaX);
        camera->RotateThirdPerson_OX(- sensivityOX * deltaY);
    }

    angle = atan2(camera->forward.x, camera->forward.z);
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && canShoot && renderCameraTarget == false) {
        Bullet bullet(camera->GetTargetPosition(), camera->forward);
        bullets.push_back(bullet);
        canShoot = false;
        fireRate = 0.25;
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
