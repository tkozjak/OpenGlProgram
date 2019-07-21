#include "openglrenderer.h"

OpenGlRenderer::OpenGlRenderer(QObject *parent) : QObject(parent)
{

}

void OpenGlRenderer::setWindow(QQuickWindow * const window)
{
    m_window = window;
}

const QString OpenGlRenderer::stringFromShaderFile(QString url)
{
    QString data;
    QString fileName(url);

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"Shader file not opened."<<endl;
    }
    else
    {
        data = file.readAll();
    }

    file.close();

    return data;
}

void OpenGlRenderer::setT(qreal t)
{
    if (t == m_time)
        return;
    m_time = t;

    emit timeChanged();
    if( m_window )
        m_window->update();
}

void OpenGlRenderer::paint()
{
    qDebug() << "paint called";

    float greenValue = ( sin(m_time) / 1.0f ) + 0.0f;

    initializeOpenGLFunctions();

    int window_width = m_window->width();
    int window_height = m_window->height();

    glViewport(0,0,100.0f,100.0f);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);


    // SETUP VAO for quad
    unsigned int VAO_quad;
    glGenVertexArrays(1, &VAO_quad);
    glBindVertexArray(VAO_quad);

    unsigned int VBO_quad;
    glGenBuffers(1, &VBO_quad);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_quad), vertices_quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);


    // diagnostic items
    int  success;
    char infoLog[512];

    // SETUP VERTEX SHADER
    QByteArray data = stringFromShaderFile(":/Shaders/simple.vert").toLocal8Bit();
    const char* vertexShaderSource = data.data();
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
    data = stringFromShaderFile(":/Shaders/simple.frag").toLocal8Bit();
    const char* fragmentShaderSource = data.data();
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

    // GET UNIFORMS
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

    glUseProgram(shaderProgram);

    // SET UNIFORMS
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glBindVertexArray(VAO_tri);
    glDrawArrays(GL_TRIANGLES, 0, 6);



    m_window->resetOpenGLState();
}
