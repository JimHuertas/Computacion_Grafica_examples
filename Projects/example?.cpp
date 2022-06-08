#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>
#include <vector>
#include <queue>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
const double PI = atan(1) * 4;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.878, 0.667f, 1.0f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.780f, 0.490f, 1.0f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.615f, 0.0305, 0.866f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource4 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.482f, 0.172f, 0.749f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource5 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.352f, 0.094f, 0.603f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource6 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.235f, 0.035f, 0.423f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource7 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.141f, 0.0f, 0.274f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource8 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.062f, 0.0f, 0.168f, 1.0f);\n"
"}\n\0";

int main()
{
    // glfw: initialize and configure   <35P10 />
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
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    
    //===============  vertex shader

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //===============   fragment Shader

    // List of fragmentShaderSource
    const GLchar* FragmentShaderSourceList[] = {
        fragmentShaderSource,
        fragmentShaderSource2,
        fragmentShaderSource3,
        fragmentShaderSource4,
        fragmentShaderSource5,
        fragmentShaderSource6,
        fragmentShaderSource7,
        fragmentShaderSource8
    };

    // create  fragmentShader;
     std::vector<unsigned int> fragmentShaderList;
    for (int i = 0; i < 8; i++) {
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &FragmentShaderSourceList[i], NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        fragmentShaderList.push_back(fragmentShader);
    }

    // link shaders
    std::deque <unsigned int> shaderPrograms;
    for (int i = 0; i < 8; i++) {
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShaderList[i]);
        glLinkProgram(shaderProgram);

        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        shaderPrograms.push_back(shaderProgram);
    }

    glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);
    fragmentShaderList.clear();
    fragmentShaderList.shrink_to_fit();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    const float radio_interno = 0.3;
    const float radio_externo = 0.7;
    const float n_triangles = 8;
    const float m = PI * 2 / n_triangles;

    float vertices_figuras_lista[8][12];
    double pendiente = 0;

    for (int i = 0; i < n_triangles; i++) {

        // calcular los vertices de cada triangulo:
        // siendo coordenada (x,y)
        // teniendo la pendiente o theta y el radio.
        //
        // cos(pendiente) = x / radio => x = cos(pendiente) * radio
        //
        // sen(pendiente) = y / radio => y = sen(pendiente) * radio

        //     2 =========== 3
        //    ||            ||
        //    ||            ||
        //    ||            ||
        //     0 =========== 1

        // interno vertice 1        //global vertice 0
        vertices_figuras_lista[i][0] = 0.0f;//x
        vertices_figuras_lista[i][1] = 0.0f;//y
        vertices_figuras_lista[i][2] = 0.0f;//z
        // externo vertice 1        //global vertice 1
        vertices_figuras_lista[i][3] = float(cos(pendiente) * radio_externo); //x 
        vertices_figuras_lista[i][4] = float(sin(pendiente) * radio_externo); //y
        vertices_figuras_lista[i][5] = 0.0f;                                     //z

        pendiente = pendiente + m;

        //  interno vertice 2        //global vertice 2
        vertices_figuras_lista[i][6] = 0.0f;//x
        vertices_figuras_lista[i][7] = 0.0f;//y
        vertices_figuras_lista[i][8] = 0.0f;//z
        //  externo vertice 2        //global vertice 3
        vertices_figuras_lista[i][9] = float(cos(pendiente) * radio_externo); //x 
        vertices_figuras_lista[i][10] = float(sin(pendiente) * radio_externo); //y
        vertices_figuras_lista[i][11] = 0.0f;
    }

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2, 
        1, 2, 3   
    };

    float vertices1[12];
    unsigned int VBO[8], VAO[8], EBO;
    glGenVertexArrays(8, VAO);
    glGenBuffers(8, VBO);
    glGenBuffers(1, &EBO);

       
    for (int i = 0; i < n_triangles; i++) {
        glBindVertexArray(VAO[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices_figuras_lista[i], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int x = 0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.062f, 0.0f, 0.168f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
 
        for (int i = 0; i < n_triangles; i++) {
            glUseProgram(shaderPrograms[i]);
            glBindVertexArray(VAO[i]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    
        //velocidad de cambio de color
        if (x == 10) {
            shaderPrograms.push_back(shaderPrograms.front());
            shaderPrograms.pop_front();
            x = 0;
        }

        x++;

        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(8, VAO);
    glDeleteBuffers(8, VBO);
    glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);
    shaderPrograms.clear();
    shaderPrograms.shrink_to_fit();


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