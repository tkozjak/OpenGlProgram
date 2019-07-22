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

    //    qDebug() << "Data size: " << data.size();

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
    //    qDebug() << "paint called";

    float greenValue = ( static_cast<float>(sin(m_time)) / 1.0f ) + 0.0f;

    initializeOpenGLFunctions();

    int window_width = m_window->width();
    int window_height = m_window->height();

    // VIEWPORT
    glViewport(0,0,window_width,window_height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // GEOMETRY
    float vertices[] = {
        // position             // color               // uv coordiantes
        -0.5f, -0.5f, 0.0f,     1.0, 0.0, 0.0, 1.0,    0.0, 0.0,
        0.5f, -0.5f, 0.0f,     0.0, 1.0, 0.0, 1.0,    1.0, 0.0,
        -0.5f,  0.5f, 0.0f,     0.0, 0.0, 1.0, 1.0,    0.0, 1.0,
        0.5f,  0.5f, 0.0f,     1.0, 1.0, 0.0, 1.0,    1.0, 1.0,
        0.0f,  0.5f, 0.0f,     1.0, 0.0, 1.0, 1.0,    0.5, 1.0
    };
    unsigned int indices_quad[]{
        0, 1, 2,
        2, 1, 3
    };
    unsigned int indices_tri[]{
        0, 1, 4
    };

    // IMAGES and TEXTURES
    int image_width, image_height, image_nrChannels;
    unsigned char *image_data;
    stbi_set_flip_vertically_on_load(true);

    // texture_0
    image_data = stbi_load("container.jpg", &image_width, &image_height, &image_nrChannels, 0);
    unsigned int texture_0;
    glGenTextures(1, &texture_0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_0);
    // filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // copy data
    if( image_data ){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    // free source data
    stbi_image_free(image_data);
    //    glBindTexture(GL_TEXTURE_2D, 0);

    // texture_1
    image_data = stbi_load("awesomeface.png", &image_width, &image_height, &image_nrChannels, 0);
    unsigned int texture_1;
    glGenTextures(1, &texture_1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_1);
    // filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if( image_data ){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    // free source data
    stbi_image_free(image_data);
    //    glBindTexture(GL_TEXTURE_2D, 0);


    // TRANSFORMATIONS
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f); // initialize to identity
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    //    qDebug() << vec.x <<" "<< vec.y <<" "<< vec.z;


    // SETUP VAO for triangle
    unsigned int VAO_tri;
    glGenVertexArrays(1, &VAO_tri);
    glBindVertexArray(VAO_tri);
    // vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // index buffer
    unsigned int EBO_tri;
    glGenBuffers(1, &EBO_tri);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_tri);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_tri), indices_tri, GL_STATIC_DRAW);
    // vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(3* sizeof(float)) );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(7* sizeof(float)) );
    glEnableVertexAttribArray(2);

    //    glBindVertexArray(0);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //    glDeleteBuffers(1,&VBO);
    //    glDeleteBuffers(1,&EBO_tri);


    // SETUP VAO for quad
    unsigned int VAO_quad;
    glGenVertexArrays(1, &VAO_quad);
    glBindVertexArray(VAO_quad);
    // vertex buffer
    unsigned int VBO_quad;
    glGenBuffers(1, &VBO_quad);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // index buffer
    unsigned int EBO_quad;
    glGenBuffers(1, &EBO_quad);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_quad);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_quad), indices_quad, GL_STATIC_DRAW);
    // vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7* sizeof(float)));
    glEnableVertexAttribArray(2);

    //    glBindVertexArray(0);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //    glDeleteBuffers(1,&VBO_quad);
    //    glDeleteBuffers(1,&EBO_quad);


    // diagnostic items
    int  success;
    char infoLog[512];


    // SETUP VERTEX SHADER
    QByteArray data = stringFromShaderFile(":/Shaders/simple.vert").toLocal8Bit();
    const char* vertexShaderSource = data.data();
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    data.clear();
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
    data.clear();
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

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // GET/SET UNIFORMS
    glUniform4f( glGetUniformLocation(shaderProgram, "ourColor"), 0.0f, greenValue, 0.0f, 1.0f);
    glUniform1i( glGetUniformLocation(shaderProgram, "ourTexture_0"), 0 );
    glUniform1i( glGetUniformLocation(shaderProgram, "ourTexture_1"), 1 );

    // bind correct VAO and RENDER
    glBindVertexArray(VAO_quad);
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


    // CLEANUP
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteVertexArrays(1, &VAO_quad);
    glDeleteVertexArrays(1, &VAO_tri);

    glDeleteProgram(shaderProgram);

    glDeleteTextures(1, &texture_0);
    glDeleteTextures(1, &texture_1);

    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO_tri);
    glDeleteBuffers(1,&VBO_quad);
    glDeleteBuffers(1,&EBO_quad);


    m_window->resetOpenGLState();
}
