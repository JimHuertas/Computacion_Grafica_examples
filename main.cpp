#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "headers/3_cubos/plano.h"
#include "headers/3_cubos/cubo.h"


/*/////////////////////////////////////////////////
// CONTROLES //

    A => GIRAR A LA IZQUIERDA
    D => GIRAR A LA DERECHA

/////////////////////////////////////////////////*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

///////////////////////////////////////////////////
// CONFIGURACION DE VALORES //

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

///////////////////////////////////////////////////

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

class Columna {
private:
    glm::mat4 model = glm::mat4(1.0f);
    // Colores
    glm::vec4 g = glm::vec4(0.0f, 0.607f, 0.282f, 1.0f);
    glm::vec4 w = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 r = glm::vec4(0.717f, 0.070f, 0.203f, 1.0f);
    glm::vec4 y = glm::vec4(1.0f, 0.835f, 0.0f, 1.0f);
    glm::vec4 b = glm::vec4(0.0f, 0.274f, 0.678f, 1.0f);
    glm::vec4 o = glm::vec4(1.0f, 0.345f, 0.0f, 1.0f);
    glm::vec4 bk = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    Cubo columna_cubos[3] = {
    Cubo(std::vector<glm::vec4>{r, b, g, w, y, bk}, glm::vec3(0.0f,  1.1f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), model),
    Cubo(std::vector<glm::vec4>{b, g, w, y, bk, r}, glm::vec3(0.0f,  0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), model),
    Cubo(std::vector<glm::vec4>{g, w, bk, r, b, g}, glm::vec3(0.0f, -1.1f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), model) };
public:
    Columna() {
        ;
    }
    void render(GLFWwindow* window, glm::mat4 view, glm::mat4 projection) {
        // cubo transformation
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            for (int i = 0; i < 3; i++)
                columna_cubos[i].set_rotation(0.8, glm::vec3(0.0f, 0.0f, 1.0f));
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            for (int i = 0; i < 3; i++)
                columna_cubos[i].set_rotation(-0.8, glm::vec3(0.0f, 0.0f, 1.0f));
        // cubo

        for (int i = 0; i < 3; i++)
            columna_cubos[i].render(view, projection);
    }

};

int main()
{
    // glfw: initialize and configure 
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Columna HHH;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------                                                                                       
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        HHH.render(window, view, projection);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, <35P10 /> this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}