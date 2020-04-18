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

// take in the actual source code for vertex and fragment shaders
static unsigned  int CreateShaderProgram(const std::string& v_shader, const std::string& f_shader) {
    unsigned int program = glCreateProgram();   // id to shader program

    // compile the vertex shader
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);  // create the vertex shader
    const char* vertex_shader_src = v_shader.c_str();
    glShaderSource(vertex_shader_id, 1, &vertex_shader_src, NULL);     // specify the shader source code we are using
    glCompileShader(vertex_shader_id);  // actually compiles the shader

    // Error handling
    int result;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length_of_error_message;
        glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &length_of_error_message);
        char* message = new char[length_of_error_message * sizeof(char)];
        glGetShaderInfoLog(vertex_shader_id, length_of_error_message, &length_of_error_message, message);
        printf("[Error:] Failed to compile vertex shader:\n");
        printf("\t%s\n", message);
        glDeleteShader(vertex_shader_id);
    }

    // compile the fragment shader
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);  // create the fragment shader
    const char* fragment_shader_src = f_shader.c_str();
    glShaderSource(fragment_shader_id, 1, &fragment_shader_src, NULL);  // specify the shader source code we are using
    glCompileShader(fragment_shader_id); // actually compiles the shader

    // Error handling
    result = 0;
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length_of_error_message;
        glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &length_of_error_message);
        char* message = new char[length_of_error_message * sizeof(char)];
        glGetShaderInfoLog(fragment_shader_id, length_of_error_message, &length_of_error_message, message);
        printf("[Error:] Failed to compile fragment shader:\n");
        printf("\t%s\n", message);
        glDeleteShader(fragment_shader_id);
    }

    // Attach vertex and fragment shader together now
    glAttachShader(program, vertex_shader_id);
    glAttachShader(program, fragment_shader_id);

    // Link the program
    glLinkProgram(program);

    // Validates the program
    glValidateProgram(program); // performs validation on the program

    // Now we delete the intermediate shaders (ie: we delete the .obj files)
    // calling glDeleteShader frees up memory; glDetatchShader(unsigned int shader_id) would
    // delete the actual shader source code
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // return the actual program
    return program;
}

int main() {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Setting the video mode to OpenGL stuff
    int result = SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    if (result != 0) {
        printf("[Error - OpenGL]: Unable to use version 3.3 of OpenGL. Please update your drivers!\n");
        printf("\tExiting with status code -3\n");
    }

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
        printf("\tExiting with status code -2...\n");
        exit(-2);
    } // WE MUST CALL THIS FUNCTION OR ALL HELL WILL BREAK LOOSE

    // Specify a vertex buffer for the triangle
    float positions[] = {
        -0.5f, -0.5f,   // 0
        0.5f, 0.5f,     // 1
        0.5f, -0.5f,    // 2 this is unique
        -0.5f, 0.5f     // 3 this is also unique
    };

    unsigned int indecies[] = {
        0, 2, 1,
        0, 3, 1
    };
    
    // Generate and bind a vertex buffer
    unsigned int v_buffer;
    glGenBuffers(1, &v_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, positions, GL_STATIC_DRAW);

    // Enable vertex attributes and assign them
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

    // Generate and bind the index buffer
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indecies, GL_STATIC_DRAW);

    // Create and compile shaders here
    std::string vertex_shader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec2 position;\n"    // saying that we are taking in index 0 of glVertexAttribPointer as the vec4 position
        "\n"
        "void main() {\n"
        "   gl_Position = vec4(position, 0.0, 1.0);"
        "}\n";
    std::string fragment_shader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main() {\n"
        "   color = vec4(0.0, 0.0, 1.0, 1.0);\n"
        "}\n";

    glDisable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glViewport( 0, 0, WIDTH, HEIGHT );
    unsigned int shader_program = CreateShaderProgram(vertex_shader, fragment_shader);
    glUseProgram(shader_program);   // here we bind the shader by telling OpenGL to use this program
                                    // in order to use it the next time we glDrawArrays()


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
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3); // either use this or glDrawElements if you have an index buffer
        //glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);  // we can use a nullptr here because we already bound the index buffer.
                                                                    // otherwise we would have to specify it
                                                                    // BTW, the 6 is the number of INDICIES
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    glDeleteProgram(shader_program);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}