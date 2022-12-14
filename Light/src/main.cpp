/**************************************************************
**************************************************************
**
** $RCSfile: Simple.ccp $
**
** $Author: jj-jc $
**
**************************************************************
**************************************************************/

// Include of the GLEW. always first than the glfw3
#include <GL/glew.h>

// Include of the GLFW
#include <GLFW/glfw3.h>

// Standard headers
#include <iostream>

// External headers
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// OpenGL Mathematics 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Own headers
#include "internal/shader.h"
#include "internal/camera.h"

#define glCheckError() glCheckError_(__FILE__, __LINE__)

// Debug 

// #################################################
// ############ Global variables ###################
// #################################################
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera system variables
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
// mouse controllers
float lastX = (float)SCR_WIDTH/2;
float lastY = (float)SCR_HEIGHT/2;
bool firstMouse = true;
// Time management
double deltaTime = 0.0f; // Time between current frame and last frame
double lastFrame = 0.0f;  // Time of last frame

// #################################################
// ############ Prototypes ###################
// #################################################
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void loadGPUTexture(const char* path, unsigned int &textureID, GLenum textureUnit);
// Debug 
GLenum glCheckError_(const char* file, int line);
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);


int main(void)
{

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
      fprintf(stderr, "ERROR: could not start GLFW3\n");
      return 1;
    } 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Dont know for what do this
    #ifdef MY_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // requesting a debug context //TODO: COMMENT THIS LINE IN RELEASE BUILD   
        std::cout << "Created a debug context\n";
    #endif
    /* Create a windowed mode window and its OpenGL context */
    // ------------------------------------------------------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyOpenGL", NULL, NULL);
    if (!window)
    {
    	std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // hide the cursor and capture it
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glCheckError();
    /* This has to be done after make a valid OpenGL rendering context */
    // start GLEW extension handler
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
        std::cout << "Error with the glewInit" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;


    // #################################################
    // ################ Debug output  ##################
    // #################################################
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if(flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        // Initialize debug output
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }


    // #################################################
    // ################ OpenGLObjects ##################
    // #################################################
    /* Definitions of objects */
    // float vertices[] = {
    //     // vertices          // colors           // texture coords
    //      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    //      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    //     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    // };

    float vertices[] = {
    // vertices            // Colors                    // Texture Coords
-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 0.0f,
0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 0.0f,
0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 1.0f,
0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 0.0f,
0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 0.0f,
0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 1.0f,
0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 0.0f,

-0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                1.0f, 0.0f,
-0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                1.0f, 1.0f,
-0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                1.0f, 0.0f,

0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 0.0f,
0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 1.0f,
0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                 0.0f, 1.0f,
0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                 0.0f, 1.0f,
0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,     1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 1.0f,
0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 1.0f,
0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 0.0f,
0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,     0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 1.0f,
0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 1.0f,
0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 0.0f,
0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,                 1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,                 0.0f, 1.0f
    };

    float cubeVertices[] = {
            // vertices         // normal vectors    // TextureCoords
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,    0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,  
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f,  
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f,  
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f
    };

    unsigned int elements[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // Containers
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    // Positions attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // Colors attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // // Texture coords 
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // build and compile Shader progam
    Shader ourShader("/home/jjjurado/Dev/MyOpenGL/resources/shaders/CoordinateSystem.vs", "/home/jjjurado/Dev/MyOpenGL/resources/shaders/CoordinateSystem.fs");
    

    // Cube Lightning 
    unsigned int lightVAO, cubeVBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    Shader lightShader("/home/jjjurado/Dev/MyOpenGL/resources/shaders/Light.vs", "/home/jjjurado/Dev/MyOpenGL/resources/shaders/Light.fs");
    Shader lightSourceShader("/home/jjjurado/Dev/MyOpenGL/resources/shaders/LigthSource.vs", "/home/jjjurado/Dev/MyOpenGL/resources/shaders/LightSource.fs");

    // Material shaders
    Shader cubeMaterials("/home/jjjurado/Dev/MyOpenGL/resources/shaders/Materials.vs", "/home/jjjurado/Dev/MyOpenGL/resources/shaders/Materials.fs");    
    // Material with light shaders
    Shader cubeMaterialsLight("/home/jjjurado/Dev/MyOpenGL/resources/shaders/MaterialLight.vs", "/home/jjjurado/Dev/MyOpenGL/resources/shaders/MaterialLight.fs");
    // Materials with lightning maps
    Shader lightningMaps("/home/jjjurado/Dev/MyOpenGL/resources/shaders/LightningMaps.vs", "/home/jjjurado/Dev/MyOpenGL/resources/shaders/LightningMaps.fs");
    Shader lightCasters("/home/jjjurado/Dev/MyOpenGL/resources/shaders/LightCasters.vs", "/home/jjjurado/Dev/MyOpenGL/resources/shaders/LightCasters.fs");
    
    
    // #################################################
    // ################### Textures ####################
    // #################################################
    // Create, wraps/filters texture object 1
    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2, texture3, texture4;
    // texture 1
    loadGPUTexture("/home/jjjurado/Dev/MyOpenGL/resources/textures/container.jpg", texture1, GL_TEXTURE0);
    // texture 2
    loadGPUTexture("/home/jjjurado/Dev/MyOpenGL/resources/textures/awesomeface.png", texture2, GL_TEXTURE1);
    // texture 3 
    loadGPUTexture("/home/jjjurado/Dev/MyOpenGL/resources/textures/container2.png", texture3, GL_TEXTURE2);
    // texture4
    loadGPUTexture("/home/jjjurado/Dev/MyOpenGL/resources/textures/container2_specular.png", texture4, GL_TEXTURE3);


    // tell openGL for each sampler to which texture unit it belongs to
    ourShader.use(); // activate the shaderProgram to setting uniforms
    ourShader.setInt("textureSampler", 0); // texture unit0
    ourShader.setInt("textureSampler2", 1); // texture unit1

    // #################################################
    // ############## Coordinate System ################
    // #################################################
    // enable z-buffer
    glEnable(GL_DEPTH_TEST);
    // model 
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // view
    glm::mat4 view = glm::mat4(1.0f);
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    // projection
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
    
    ourShader.use();
    ourShader.setMat4("model", model);
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

    // more cubes creation
    glm::vec3 cubePositions[] = {
        glm::vec3( 5.0f,  2.0f,  -4.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    // #################################################
    // ##################### Loop ######################
    // #################################################
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindTexture(GL_TEXTURE_2D, texture3);
        glBindTexture(GL_TEXTURE_2D, texture4);
        // time
        // gl

        // render 
        ourShader.use();
        // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
             if(i%3 == 0)
             {
                model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
             }
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // Light Source
        lightSourceShader.use();
        glm::vec3 lightPos = glm::vec3(1.2f, 0.0f, 3.0f);
        glm::mat4 modelLightSource = glm::mat4(1.0f);
        // modelLightSource = glm::rotate(modelLightSource, glm::radians(glm::cos(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));        
        // modelLightSource = glm::rotate(modelLightSource, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        modelLightSource = glm::translate(modelLightSource, lightPos);
        modelLightSource = glm::scale(modelLightSource, glm::vec3(0.2f));
        lightSourceShader.setMat4("model", modelLightSource);
        lightSourceShader.setMat4("view", view);
        lightSourceShader.setMat4("projection", projection);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Light Cube
        glBindVertexArray(lightVAO);
        lightShader.use();
        glm::mat4 modelCube = glm::mat4(1.0f);
        lightShader.setMat4("model", modelCube);
        glm::mat3 normalCube = glm::mat3(transpose(inverse(modelCube))); // normal matrix to transform the normal vectors into the world coordinates, this avoid translation and problems with non-uniform scale
        lightShader.setMat3("normal", normalCube); // from the world coordinates
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        lightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightShader.setVec3("lightPos", glm::vec3(modelLightSource * glm::vec4(lightPos, 1.0f)));
        lightShader.setVec3("viewPos", camera.Position);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    {
        // Cube with Materials
        glBindVertexArray(lightVAO);
        cubeMaterials.use();
        glm::mat4 modelCube = glm::mat4(1.0f);
        modelCube = glm::translate(modelCube, glm::vec3(3.0f, 0.0f, 0.0f));
        cubeMaterials.setMat4("model", modelCube);
        glm::mat3 normalCube = glm::mat3(transpose(inverse(modelCube))); // normal matrix to transform the normal vectors into the world coordinates, this avoid translation and problems with non-uniform scale
        cubeMaterials.setMat3("normal", normalCube); // from the world coordinates
        cubeMaterials.setMat4("view", view);
        cubeMaterials.setMat4("projection", projection);
        cubeMaterials.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        cubeMaterials.setVec3("lightPos", glm::vec3(modelLightSource * glm::vec4(lightPos, 1.0f)));
        cubeMaterials.setVec3("viewPos", camera.Position);
        cubeMaterials.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        cubeMaterials.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        cubeMaterials.setVec3("material.specular", 0.5f, 0.5f, 0.31f);
        cubeMaterials.setFloat("material.shininess", 32.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    {
        // Cube with Materials and LIght
        glBindVertexArray(lightVAO);
        cubeMaterialsLight.use();
        glm::mat4 modelCube = glm::mat4(1.0f);
        modelCube = glm::translate(modelCube, glm::vec3(1.5f, 0.0f, 0.0f));
        cubeMaterialsLight.setMat4("model", modelCube);
        glm::mat3 normalCube = glm::mat3(transpose(inverse(modelCube))); // normal matrix to transform the normal vectors into the world coordinates, this avoid translation and problems with non-uniform scale
        cubeMaterialsLight.setMat3("normal", normalCube); // from the world coordinates
        cubeMaterialsLight.setMat4("view", view);
        cubeMaterialsLight.setMat4("projection", projection);
        cubeMaterialsLight.setVec3("lightPos", glm::vec3(modelLightSource * glm::vec4(lightPos, 1.0f)));
        cubeMaterialsLight.setVec3("viewPos", camera.Position);
        cubeMaterialsLight.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        cubeMaterialsLight.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        cubeMaterialsLight.setVec3("material.specular", 0.5f, 0.5f, 0.31f);
        cubeMaterialsLight.setFloat("material.shininess", 32.0f);
        cubeMaterialsLight.setVec3("light.ambient", 0.2f, 0.0f, 0.0f);
        cubeMaterialsLight.setVec3("light.diffuse", 0.5f, 0.0f, 0.0f);
        cubeMaterialsLight.setVec3("light.specular", 1.0f, 0.0f, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    {
        // Cube with lightningMaps
        glBindVertexArray(lightVAO);
        lightningMaps.use();
        glm::mat4 modelCube = glm::mat4(1.0f);
        modelCube = glm::translate(modelCube, glm::vec3(4.5f, 0.0f, 0.0f));
        lightningMaps.setMat4("model", modelCube);
        glm::mat3 normalCube = glm::mat3(transpose(inverse(modelCube))); // normal matrix to transform the normal vectors into the world coordinates, this avoid translation and problems with non-uniform scale
        lightningMaps.setMat3("normal", normalCube); // from the world coordinates
        lightningMaps.setMat4("view", view);
        lightningMaps.setMat4("projection", projection);
        lightningMaps.setVec3("lightPos", glm::vec3(modelLightSource * glm::vec4(lightPos, 1.0f)));
        lightningMaps.setVec3("viewPos", camera.Position);
        lightningMaps.setInt("material.diffuse", 2);
        lightningMaps.setInt("material.specular", 3);
        lightningMaps.setFloat("material.shininess", 256.0f);
        lightningMaps.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightningMaps.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightningMaps.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    {   // Cube with lightningMaps and lightCasters
        glBindVertexArray(lightVAO);
        lightCasters.use();
        glm::mat4 modelCube = glm::mat4(1.0f);
        modelCube = glm::translate(modelCube, glm::vec3(7.0f, 0.0f, 0.0f));
        lightCasters.setMat4("model", modelCube);
        glm::mat3 normalCube = glm::mat3(transpose(inverse(modelCube))); // normal matrix to transform the normal vectors into the world coordinates, this avoid translation and problems with non-uniform scale
        lightCasters.setMat3("normal", normalCube); // from the world coordinates
        lightCasters.setMat4("view", view);
        lightCasters.setMat4("projection", projection);
        lightCasters.setVec3("viewPos", camera.Position);
        lightCasters.setInt("material.diffuse", 2);
        lightCasters.setInt("material.specular", 3);
        lightCasters.setFloat("material.shininess", 256.0f);

        lightCasters.setVec3("dirLight.direction", 0.0f, 0.0f, 1.0f);
        lightCasters.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        lightCasters.setVec3("dirLight.diffuse", 0.3f, 0.3f, 0.3f);
        lightCasters.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

        lightCasters.setVec3("pointLight[0].position", 9.0f, 0.0f, -1.0f);
        lightCasters.setVec3("pointLight[0].ambient", 0.2f, 0.2f, 0.2f);
        lightCasters.setVec3("pointLight[0].diffuse", 0.5f, 0.5f, 0.5f);
        lightCasters.setVec3("pointLight[0].specular", 1.0f, 1.0f, 1.0f);
        lightCasters.setFloat("pointLight[0].constant", 1.0f);
        lightCasters.setFloat("pointLight[0].linear", 0.7f);
        lightCasters.setFloat("pointLight[0].quadratic", 1.8f);

        lightCasters.setVec3("pointLight[1].position", 6.0f, 0.0f, -1.0f);
        lightCasters.setVec3("pointLight[1].ambient", 0.2f, 0.2f, 0.2f);
        lightCasters.setVec3("pointLight[1].diffuse", 0.5f, 0.5f, 0.5f);
        lightCasters.setVec3("pointLight[1].specular", 1.0f, 1.0f, 1.0f);        
        lightCasters.setFloat("pointLight[1].constant", 1.0f);
        lightCasters.setFloat("pointLight[1].linear", 0.35f);
        lightCasters.setFloat("pointLight[1].quadratic", 0.44f);

        lightCasters.setVec3("spotLight.position", camera.Position);
        lightCasters.setVec3("spotLight.direction", camera.Front);
        lightCasters.setFloat("spotLight.cutOff", glm::cos(glm::radians(5.5f)));
        lightCasters.setFloat("spotLight.outerOff", glm::cos(glm::radians(10.0f)));
        lightCasters.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        lightCasters.setVec3("spotLight.diffuse", 0.3f, 0.3f, 0.3f);
        lightCasters.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);     
        glDrawArrays(GL_TRIANGLES, 0, 36);

        {   // Point Light 1
            lightSourceShader.use();
            glm::vec3 lightPos = glm::vec3(9.0f, 0.0f, -1.0f);
            glm::mat4 modelLightSource = glm::mat4(1.0f);
            // modelLightSource = glm::rotate(modelLightSource, glm::radians(glm::cos(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));        
            // modelLightSource = glm::rotate(modelLightSource, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            modelLightSource = glm::translate(modelLightSource, lightPos);
            modelLightSource = glm::scale(modelLightSource, glm::vec3(0.2f));
            lightSourceShader.setMat4("model", modelLightSource);
            lightSourceShader.setMat4("view", view);
            lightSourceShader.setMat4("projection", projection);
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        {   // Point Light 2
            lightSourceShader.use();
            glm::vec3 lightPos = glm::vec3(6.0f, 0.0f, -1.0f);
            glm::mat4 modelLightSource = glm::mat4(1.0f);
            // modelLightSource = glm::rotate(modelLightSource, glm::radians(glm::cos(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));        
            // modelLightSource = glm::rotate(modelLightSource, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            modelLightSource = glm::translate(modelLightSource, lightPos);
            modelLightSource = glm::scale(modelLightSource, glm::vec3(0.2f));
            lightSourceShader.setMat4("model", modelLightSource);
            lightSourceShader.setMat4("view", view);
            lightSourceShader.setMat4("projection", projection);
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // { // spot light
        //     lightSourceShader.use();
        //     glm::vec3 lightPos = glm::vec3(7.0f, 0.0f, -2.0f);
        //     glm::mat4 modelLightSource = glm::mat4(1.0f);
        //     // modelLightSource = glm::rotate(modelLightSource, glm::radians(glm::cos(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));        
        //     // modelLightSource = glm::rotate(modelLightSource, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        //     modelLightSource = glm::translate(modelLightSource, lightPos);
        //     modelLightSource = glm::scale(modelLightSource, glm::vec3(0.2f));
        //     lightSourceShader.setMat4("model", modelLightSource);
        //     lightSourceShader.setMat4("view", view);
        //     lightSourceShader.setMat4("projection", projection);
        //     glBindVertexArray(lightVAO);
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

    }

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents(); // Poll for and process events
    }
    glfwTerminate(); // Close OpenGlL window and terminate GLFW
    exit(EXIT_SUCCESS);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);   
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);


    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// load textures simply
void loadGPUTexture(const char* path, unsigned int &textureID, GLenum textureUnit)
{

    glGenTextures(1, &textureID);
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}


GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                   error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                  error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:              error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                 error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:                error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                  error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}





