#include "raylib.h"
#include "raymath.h"

#include <iostream>
#include <vector>

class Laser{
    private:
        Vector2 m_position {};
        int m_speed {};
        bool m_active {true};

    public:
        Laser(Vector2 position, int speed)
            : m_position {position}, m_speed {speed}
        {

        }

        bool isActive(){
            return m_active;
        }

        void Update(){
            m_position.y += m_speed;
            if(m_active){
                if(m_position.y < 0 || m_position.y > GetScreenHeight()){
                    m_active = false;
                    std::cout << "Laser Inactive" << '\n';
                }
            }
        }

        void Draw(){
            if(m_active){
                DrawRectangle(m_position.x, m_position.y, 4, 15, YELLOW);
            }
        }
};

class Spaceship{
    private:
        Texture2D m_image {LoadTexture("Graphics/spaceship.png")};
        Vector2 m_position {static_cast<float>(GetScreenWidth() - m_image.width) / 2, 
                            static_cast<float>(GetScreenHeight() - m_image.height)};
        int m_speed {7};
        std::vector<Laser> m_lasers {};
        double m_lastFiredTime {0.0};
    
    public:
        ~Spaceship(){
            UnloadTexture(m_image);
        }

        std::vector<Laser>& GetLasers() {
            return m_lasers;
        }

        void Draw(){
            DrawTextureV(m_image, m_position, WHITE);
        }

        void MoveLeft(){
            m_position.x -= m_speed;

            if(m_position.x <= 0){
                m_position.x = 0;
            }
        }

        void MoveRight(){
            m_position.x += m_speed;

            if(m_position.x >= GetScreenWidth() - m_image.width){
                m_position.x = static_cast<float>(GetScreenWidth() - m_image.width);
            }
        }

        void shoot(){
            if(GetTime() - m_lastFiredTime >= 0.35){
                m_lasers.push_back(Laser({m_position.x + m_image.width / 2 - 2, m_position.y}, - 6));
                m_lastFiredTime = GetTime();
            }
        }
};

class Block{
    private:
        Vector2 m_position {};

    public:
        Block(Vector2 position)
            : m_position {position}
        {

        }
        void Draw(){
            DrawRectangle(m_position.x, m_position.y, 3, 3, BLUE);

        }
};

class Obstacle{
    private:
        Vector2 m_position {};
        std::vector<Block> m_blocks {};
        std::vector<std::vector<int>> m_grid{};

    public: 
        Obstacle(Vector2 position)
            : m_position {position}
        {
            m_grid = {
                {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}
            };

            for(std::size_t row{0}; row < m_grid.size(); ++row){
                for(std::size_t column {0}; column < m_grid[row].size(); ++column){
                    if(m_grid[row][column] == 1){
                        float posX {m_position.x + column * 3};
                        float posY {m_position.y + row * 3};
                        m_blocks.push_back(Block({posX, posY}));
                    }
                }
            }
        }

        void Draw(){
            for(auto& block: m_blocks){
                block.Draw();
            }
        }
};

class Game{
    private:
        Spaceship spaceship {};

    public:
        Game(){

        }

        ~Game(){

        }

        void Draw(){
            spaceship.Draw();
            for(auto& laser: spaceship.GetLasers()){
                laser.Draw();
            }
        }

        void Update(){
            for(auto p{spaceship.GetLasers().begin()}; p != spaceship.GetLasers().end();){

                p->Update();
                if(!p->isActive()){
                    spaceship.GetLasers().erase(p);
                }
                else{
                    ++p;
                }
            }
        }

        void HandleInput(){
            if(IsKeyDown(KEY_LEFT)){
                spaceship.MoveLeft();
            }

            if(IsKeyDown(KEY_RIGHT)){
                spaceship.MoveRight();
            }

            if(IsKeyDown(KEY_SPACE)){
                spaceship.shoot();
            }
        }
};

int main(void) {
    constexpr Color grey = {29, 29, 27, 255};
    constexpr int windowWidth {750};
    constexpr int windowHeight {700};

    InitWindow(windowWidth, windowHeight, "Space Invaders");
    SetTargetFPS(60);

    Game game {};
    Obstacle obstacle {{100, 100}};

    while(!WindowShouldClose()){

        game.HandleInput();
        game.Update();

        BeginDrawing();

        ClearBackground(grey);

        game.Draw();
        obstacle.Draw();

        EndDrawing();
    }

    CloseWindow();
}
