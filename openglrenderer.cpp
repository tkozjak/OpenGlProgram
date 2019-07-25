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

// PROPERTY SETTERS
// time
void OpenGlRenderer::setTime(qreal in_time)
{
    if (in_time == m_time)
        return;
    m_time = in_time;

    emit timeChanged();
    if( m_window )
        m_window->update();
}
// geometry switch
void OpenGlRenderer::setGswitch( bool in_bool)
{
    m_gswitch = in_bool;
}

// RENDER FUNCTION
void OpenGlRenderer::paint()
{
    //    qDebug() << "paint called";

    float greenValue = ( static_cast<float>(sin(m_time)) / 1.0f ) + 0.0f;

    int window_width = m_window->width();
    int window_height = m_window->height();

    // VIEWPORT
    glViewport(0,0,window_width,window_height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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
    // model matrix aka world matrix
    glm::mat4 model = glm::mat4(1.0f);
    float deg = -55 * static_cast<float>(m_time);
    model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    // view matrix aka camera matrix
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    // projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_width) / static_cast<float>(window_height), 0.1f, 100.0f);

    // we render many cubes
    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
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
    // set matrices
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // bind correct VAO and RENDER
    if( m_gswitch ){
        glBindVertexArray(VAO_quad);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
//    else{
//        glBindVertexArray(VAO_tri);
//        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
//    }
    else{
        glBindVertexArray(VAO_box);
        for(unsigned int i = 0; i < 10; i++)
        {
          glm::mat4 model = glm::mat4(1.0f);
          model = glm::translate(model, cubePositions[i]);
          float angle = 20.0f * (i+1)  * static_cast<float>(m_time);
          model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
          glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // CLEANUP
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture_0);
    glDeleteTextures(1, &texture_1);


    m_window->resetOpenGLState();
}

// BUFFER INITIALIZATNIOS
void OpenGlRenderer::initialize()
{
    qDebug() << "Initalize called.";

    initializeOpenGLFunctions();

    QOpenGLContext* context = m_window->openglContext();
    QSurfaceFormat def_fb_surface_format = context->format();
    int major = def_fb_surface_format.majorVersion();
    int minor = def_fb_surface_format.minorVersion();
    QSurfaceFormat::FormatOptions format_options = def_fb_surface_format.options();
    QSurfaceFormat::ColorSpace  color_space = def_fb_surface_format.colorSpace();
    QSurfaceFormat::OpenGLContextProfile context_profile = def_fb_surface_format.profile();
    QSurfaceFormat::RenderableType renderable_type = def_fb_surface_format.renderableType();
    qDebug() << "Default framebuffer surface version. Major: "<< major<<". Minor: "<<minor;
    qDebug() << "Default framebuffer surface version. Major: "<< major<<". Minor: "<<minor;
    qDebug() << "Default framebuffer surface. Format options: "<< format_options;
    qDebug() << "Default framebuffer surface. Color space: "<< color_space;
    qDebug() << "Default framebuffer surface. Context profile: "<< context_profile;
    qDebug() << "Default framebuffer surface. Renderable type: "<< renderable_type;

    //COMPUTE SHADER INFO
    int maxsizeX;
    uint dim = 0;
    glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, dim, &maxsizeX );
    int maxsizeY;
    dim = 1;
    glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, dim, &maxsizeY );
    int maxsizeZ;
    dim = 2;
    glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, dim, &maxsizeZ );
    qDebug() << "Max (local) work group size (x,y,z): "<< maxsizeX <<", "<< maxsizeY <<", "<< maxsizeZ;

    int maxInvoc;
    glGetIntegerv( GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &maxInvoc );
    qDebug() << "Max invocations in (local) work group: "<< maxInvoc;


    // GEOMETRY
    // vertices
    float vertices[] = {
        // position             // color               // uv coordiantes
        -0.5f, -0.5f, 0.0f,     1.0, 0.0, 0.0, 1.0,    0.0, 0.0,
         0.5f, -0.5f, 0.0f,     0.0, 1.0, 0.0, 1.0,    1.0, 0.0,
        -0.5f,  0.5f, 0.0f,     0.0, 0.0, 1.0, 1.0,    0.0, 1.0,
         0.5f,  0.5f, 0.0f,     1.0, 1.0, 0.0, 1.0,    1.0, 1.0,
         0.0f,  0.5f, 0.0f,     1.0, 0.0, 1.0, 1.0,    0.5, 1.0
    };
    // quad indices
    unsigned int indices_quad[]{
        0, 1, 2,
        2, 1, 3
    };
    // triangle indices
    unsigned int indices_tri[]{
        0, 1, 4
    };

    // SETUP VAO for triangle
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

    // diagnostic
    GLint size = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    qDebug() << "Vertex buffer size (triangle): " << size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    qDebug() << "Index buffer size (triangle): " << size;

    glBindVertexArray(0);


    // SETUP VAO for quad
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
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // tex coordinate
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7* sizeof(float)));
    glEnableVertexAttribArray(2);

    // diagnostic
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    qDebug() << "Vertex buffer size (quad): " << size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    qDebug() << "Index buffer size (quad): " << size;

    glBindVertexArray(0);


    // VAO for box
    float box_vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     1.0, 0.0, 0.0, 1.0,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     1.0, 0.0, 0.0, 1.0,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     1.0, 0.0, 0.0, 1.0,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     1.0, 1.0, 0.0, 1.0,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,     1.0, 1.0, 0.0, 1.0,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,     1.0, 1.0, 0.0, 1.0,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,     1.0, 1.0, 0.0, 1.0,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,     1.0, 1.0, 0.0, 1.0,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     1.0, 1.0, 0.0, 1.0,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     1.0, 0.0, 1.0, 1.0,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     1.0, 0.0, 1.0, 1.0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     1.0, 0.0, 1.0, 1.0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     1.0, 0.0, 1.0, 1.0,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     1.0, 0.0, 1.0, 1.0,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     1.0, 0.0, 1.0, 1.0,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     1.0, 1.0, 1.0, 1.0,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     1.0, 1.0, 1.0, 1.0,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     1.0, 1.0, 1.0, 1.0,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     1.0, 1.0, 1.0, 1.0,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     1.0, 1.0, 1.0, 1.0,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     1.0, 1.0, 1.0, 1.0,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     0.0, 1.0, 1.0, 1.0,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,     0.0, 1.0, 1.0, 1.0,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,     0.0, 1.0, 1.0, 1.0,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,     0.0, 1.0, 1.0, 1.0,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     0.0, 1.0, 1.0, 1.0,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     0.0, 1.0, 1.0, 1.0,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     1.0, 0.0, 0.0, 1.0,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     1.0, 0.0, 0.0, 1.0,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,     1.0, 0.0, 0.0, 1.0,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     1.0, 0.0, 0.0, 1.0,
    };

    glGenVertexArrays(1, &VAO_box);
    glBindVertexArray(VAO_box);
    // vertex buffer
    unsigned int VBO_box;
    glGenBuffers(1, &VBO_box);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // vertex attributes
    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // color
    // tex coordinates
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5* sizeof(float)));
    glEnableVertexAttribArray(1);
    // tex coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(2);

    // diagnostic
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    qDebug() << "Vertex buffer size (box): " << size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    qDebug() << "Index buffer size (box): " << size;

    glBindVertexArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // not sure about this
    // does not work on HP!
//    glDeleteBuffers(1,&VBO);
//    glDeleteBuffers(1,&EBO_tri);
//    glDeleteBuffers(1,&VBO_quad);
//    glDeleteBuffers(1,&EBO_quad);

}

// CLEANUP
void OpenGlRenderer::cleanup()
{
    //    qDebug() << "Cleanup called.";
}

void OpenGlRenderer::contextInfo()
{

}
