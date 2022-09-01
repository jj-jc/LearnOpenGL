#ifndef GAME_H_
#define GAME_H_
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE, 
    GAME_MENU,
    GMAE_WIN
};

class Game
{
    public:
        // game state
        GameState State;
        bool keys[1024];
        unsigned int Width, Height;
        // constructor/destructor
        Game(unsigned int width, unsigned int height);              
        ~Game();
        //initialize game state (load all shaders/textures/levels)
        void Init();
        // game Loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
};

#endif