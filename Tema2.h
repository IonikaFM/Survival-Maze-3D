#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

        struct Maze
        {
            Maze() : cellType(0), visited(false) {}
            Maze(int cellType, bool visited) : cellType(cellType), visited(visited) {}
            int cellType;
            bool visited;
        };

        class Bullet
        {
        public:
            float speed = 15;
            glm::vec3 position, forward;

            Bullet(glm::vec3 position, glm::vec3 forward) {
                this->position = position;
                this->forward = forward;
            }
        };

        class Enemy
        {
        public:
            glm::vec3 position;

            Enemy(float x, float z) {
                position.x = x;
                position.y = 0;
                position.z = z;
            }
        };

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderPlayer();
        void RenderEnemy(Enemy enemy);
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        std::pair<std::vector<VertexFormat>, std::vector<unsigned int>> CreateCube(glm::vec3 color);
        void InitLabirinth();
        void RenderScene();
        std::vector<std::pair<Maze, std::pair<int, int>>> GetNeighbours(int x, int y);
        bool CanChooseThisNeighbour(int x, int y);
        bool CheckPlayerWallCollision(std::pair<int, int> wall);
        bool CheckEnemyPlayerCollision(Enemy enemy, glm::vec3 position);
        bool CheckEnemyBulletCollision(Enemy enemy, Bullet bullet);
        bool CheckBulletWallCollision(Bullet bullet, std::pair<int, int> wall);
        bool CheckBulletHeightPosition(Bullet bullet);
        void DrawBullet(Bullet bullet, float deltaTimeSeconds);
        void DrawHealthBar();
        void DrawTimer();
        void DrawGameOver();
        void DrawYouWon();

     protected:
        implemented::MyCamera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        int matrixSize = 25;
        Maze maze[25][25];
        std::vector<std::pair<Maze, std::pair<int, int>>> visited;
        std::vector<std::pair<Maze, std::pair<int, int>>> way;
        int posX, posZ;
        float angle;
        std::vector<std::pair<int, int>> walls;
        std::vector<Bullet> bullets;
        std::vector<Enemy> enemies;
        bool canShoot = true;
        float fireRate = 0.25;
        float enemyOffsetX = 0, enemyOffsetZ = 0;
        float enemySpeed = 10;
        glm::vec4 direction = glm::vec4(1, 0, 0, 0);
        int playerHealth = 3;
        float timer = 60;
        bool gameOver = false;
        bool gameWon = false;
        int score = 0;
    };
}   // namespace m1
