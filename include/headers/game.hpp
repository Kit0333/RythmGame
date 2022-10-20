#pragma once

#include <sdl2/SDL.h>
#include <sdl2/SDL_image.h>

enum class GameState{PLAY,EXIT};
class Game{
public:
    Game(const char* title,Uint32 flags);
    ~Game();

    SDL_Texture* loadTexture(const char* filePath);
    void render(SDL_Texture* text);
    GameState getGameState();
    int& mousePress(SDL_MouseButtonEvent& b);
    void Game::gameStateChange(GameState x);
         
    void run();
private:

    void clear();
    void display();
    void cleanup();

    void UserScreenSize(int&width, int &height);
    GameState m_gameState;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};

class Board{
public:
    Board();
    ~Board();
private:
    int *m_board;
};
enum class Player{P1,P2};
class Pawn{
public:
    Pawn(int type,Player master);
    ~Pawn();

private:
    int m_type; 
    Player m_master;  
};
