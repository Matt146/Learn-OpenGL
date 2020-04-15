#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

/******COMPILE:************************
 *  g++ main.cpp -lGLEW -lGL -lSDL2
**************************************/

#define WIDTH 1080
#define HEIGHT 720

int main() {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Setting the video mode to OpenGL stuff
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create a window
    SDL_Window* window = NULL;
    window = SDL_CreateWindow("OpenGL Learning Stuff",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WIDTH,
                            HEIGHT,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("[Error - SDL2]: Unable to create a window!\n");
        printf("\tExiting with status code -1...\n");
        exit(-1);
    }

    // Create a valid OpenGL rendering context
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == NULL) {
        printf("[Error - SDL2]: Unable to create a valid OpenGL rendering context!!\n");
        printf("\tExiting with status code -1...\n");
        exit(-1);
    }

    // initialize glew
    glewExperimental = GL_FALSE;
    unsigned int glew_status = glewInit();
    if(glew_status) {
        printf("[Error- GLEW]: Unable to initialize GLEW\n");
        printf("\tCode: %d\n", glew_status);
        printf("\tExiting with status code -2\n");
        exit(-2);
    } // WE MUST CALL THIS FUNCTION OR ALL HELL WILL BREAK LOOSE

    // Specify a vertex buffer for the triangle
    float vertices[6] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f
    };
    
    // Generate and bind a vertex buffer
    unsigned int v_buffer;
    std::cout << "1" << std::endl;
    glGenBuffers(1, &v_buffer);
    std::cout << "2" << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertices, GL_STATIC_DRAW);

    // Enable vertex attributes and assign them
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);


    // Set up and run game loop here
    bool running = true;
    SDL_Event ev;
    while (running == true) {
        // Handle events 
        while (!SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    if (ev.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                    break;
                default:
                    break;
            }
        }

        // Game loop operations here

        // Render here
        glClear( GL_COLOR_BUFFER_BIT );
        glDrawArrays(GL_TRIANGLES, 0, 3); // either use this or glDrawElements
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}