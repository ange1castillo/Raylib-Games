#include "raylib.h"
#include "raymath.h"

#include <iostream>

Color Green = Color {38, 185, 154, 255};
Color Dark_Green = Color {20, 160, 133, 255};
Color Light_Green = Color {129, 204, 184, 255};
Color Yellow = Color {243, 213, 91, 255};

class Player{
    protected:
        void stayInWindow(){
            if(m_posY <= 0){
                m_posY = 0;
            }

            if(m_posY + m_height >= GetScreenHeight()){
                m_posY = GetScreenHeight() - m_height;
            }
        }

    public:
        float m_posX {};
        float m_posY {};
        float m_width {};
        float m_height {};
        int m_speedY {6};
        Color m_color {};
        int m_score {};

        Player(float posX, float posY, float width, float height, Color color)
            : m_posX {posX}, m_posY {posY}, m_width {width}, m_height {height}, m_color {color}{

            }
    
        void drawPaddle(){
            DrawRectangleRounded({m_posX, m_posY, m_width, m_height}, 0.8, 0, WHITE);
        }

        void updatePaddle(){
            if(IsKeyDown(KEY_UP)){
                m_posY -= m_speedY;
            }

            if(IsKeyDown(KEY_DOWN)){
                m_posY += m_speedY;
            }

            stayInWindow();
        }
};

class CPU: public Player{
    public:
        CPU(float posX, float posY, float width, float height, Color color)
            : Player {posX, posY, width, height, color}{

            }

        void updatePaddle(float ballPosY){
            if(m_posY + m_height / 2 > ballPosY){
                m_posY -= m_speedY;
            }

            if(m_posY + m_height / 2 <= ballPosY){
                m_posY += m_speedY;
            }

            stayInWindow();
        }
};

class Ball{
    public:
        float m_posX {};
        float m_posY {};
        int m_speedX {7};
        int m_speedY {7};
        int m_radius {};
        Color m_color {};

        Ball(float posX, float posY, int radius, Color color)
            : m_posX {posX}, m_posY {posY}, m_radius {radius}, m_color {color}{

        }

        void draw(){
            DrawCircle(m_posX, m_posY, m_radius, m_color);
        }

        void update(Player& player, CPU& cpu){
            m_posX += m_speedX;
            m_posY += m_speedY;

            if(m_posY + m_radius >= GetScreenHeight() || m_posY - m_radius <= 0){
                m_speedY *= -1;
            }

            if(m_posX + m_radius >= GetScreenWidth()){
                player.m_score++;
                reset();
            }

            if(m_posX - m_radius <= 0){
                cpu.m_score++;
                reset();
            }
        }

        void reset(){
            m_posX = GetScreenWidth() / 2;
            m_posY = GetScreenHeight() / 2;

            constexpr std::array<int, 2> speedChoices{-1, 1};
            m_speedX *= speedChoices[GetRandomValue(0,1)];
            m_speedY *= speedChoices[GetRandomValue(0,1)];
        }
};

int main(){
    constexpr int screen_width { 1280 };
    constexpr int screen_height { 800 };

    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    Ball ball {screen_width / 2, screen_height / 2, 20, Yellow};
    Player player {10, (screen_height / 2) - 60, 25, 120, WHITE}; 
    CPU cpu {screen_width - 35, (screen_height / 2) - 60, 25, 120, WHITE};

    while(!WindowShouldClose()) {
        BeginDrawing();

        ball.update(player, cpu);
        cpu.updatePaddle(ball.m_posY);
        player.updatePaddle();

        if(CheckCollisionCircleRec(Vector2{ball.m_posX, ball.m_posY}, ball.m_radius, 
                                    Rectangle{player.m_posX, player.m_posY, player.m_width, player.m_height})){
            ball.m_speedX *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.m_posX, ball.m_posY}, ball.m_radius, 
                                    Rectangle{cpu.m_posX, cpu.m_posY, cpu.m_width, cpu.m_height})){
            ball.m_speedX *= -1;
        }

        ClearBackground(Dark_Green);

        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

        ball.draw();
        cpu.drawPaddle();
        player.drawPaddle();
        DrawText(TextFormat("%i", player.m_score), (screen_width / 4) - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", cpu.m_score), (3 * screen_width / 4) - 20, 20, 80, WHITE);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
