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

        std::vector<Laser> GetLasers() const {
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

        friend class Game;
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
            for(auto& laser: spaceship.m_lasers){
                laser.Draw();
            }
        }

        void Update(){
            for(auto& laser: spaceship.m_lasers){
                laser.Update();
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

    while(!WindowShouldClose()){

        game.HandleInput();
        game.Update();

        BeginDrawing();

        ClearBackground(grey);

        game.Draw();

        EndDrawing();
    }

    CloseWindow();
}
