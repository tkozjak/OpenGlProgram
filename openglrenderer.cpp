#include "openglrenderer.h"

OpenGlRenderer::OpenGlRenderer(QObject *parent) : QObject(parent)
{

}

void OpenGlRenderer::setWindow(QWindow * const window)
{
    m_window = window;
}

void OpenGlRenderer::paint()
{
    qDebug() << "paint called";

    initializeOpenGLFunctions();

    int window_width = m_window->width();
    int window_height = m_window->height();

    glViewport(0,0,window_width,window_height);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices_triangle[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    float vertices_quad[] = {
        -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f,  0.5f, 0.0f,
          0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f,  0.5f, 0.0f
    };

    // SETUP VAO for triangle
    unsigned int VAO_tri;
    glGenVertexArrays(1, &VAO_tri);
    glBindVertexArray(VAO_tri);

    unsigned int VBO_tri;
    glGenBuffers(1, &VBO_tri);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_tri);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle), vertices_triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // SETUP VAO for quad
    unsigned int VAO_quad;
    glGenVertexArrays(1, &VAO_quad);
    glBindVertexArray(VAO_quad);

    unsigned int VBO_quad;
    glGenBuffers(1, &VBO_quad);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_quad), vertices_quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // diagnostic items
    int  success;
    char infoLog[512];

    // SETUP VERTEX SHADER
    const char* vertexShaderSource = "#version 330 core\n layout (location = 0) in vec3 aPos;\n void main(){ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog;
    }


    // SETUP FRAGMENT SHADER
    const char* fragmentShaderSource = "#version 330 core\n out vec4 FragColor;\n void main(){ FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog;
    }

    // SETUP SHADER PROGRAM
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        qDebug() << "ERROR::PROGRAM::COMPILATION_FAILED" << infoLog;
    }


    glUseProgram(shaderProgram);
    glBindVertexArray(VAO_quad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
