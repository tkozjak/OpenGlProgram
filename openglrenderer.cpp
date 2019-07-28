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

    float time = static_cast<float>(sin(m_time));

    int window_width = m_window->width();
    int window_height = m_window->height();


    // VIEWPORT
    glViewport(0,0, window_width, window_height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // RUN FILL TEXTURE COMPUTE SHADER PROGRAM
    int tex_w = 512, tex_h = 512;
    int group_size = 32;
    glUseProgram(m_shaderPrograms["fill_texture_compute_prg"]);
    glDispatchCompute((GLuint)tex_w/group_size, (GLuint)tex_h/group_size, 1);
    // put up memory barrier
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


    // experiment: read texture
    GLfloat* pixels = new GLfloat[tex_w*tex_h*4];
    glActiveTexture(GL_TEXTURE2);
    glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA,  GL_FLOAT, pixels);
    qDebug() << "pixels get texture: " << pixels[3] << pixels[7] << pixels[11] << pixels[15];
    delete [] pixels;



    // RUN PARTICLES COMPUTE SHADER PROGRAM
    glUseProgram(m_shaderPrograms["particles_compute_prg"]);
    glUniform1f( glGetUniformLocation(m_shaderPrograms["particles_compute_prg"], "time_1"), time);
    glUniform1f( glGetUniformLocation(m_shaderPrograms["particles_compute_prg"], "time_2"), QDateTime::currentDateTime().time().second());
    glDispatchCompute( NUM_PARTICLES/WORK_GROUP_SIZE, 1, 1);
    // put up memory barrier
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


    // TRANSFORMATIONS
    // model matrix aka world matrix
    glm::mat4 model = glm::mat4(1.0f);
    float deg = -55 * static_cast<float>(m_time);
    model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    // view matrix aka camera matrix
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -15.0f));
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

    glUseProgram(m_shaderPrograms["standard_prg"]);

    // GET/SET UNIFORMS
    // set global time
    glUniform1f( glGetUniformLocation(m_shaderPrograms["standard_prg"], "ourTime"), time);

    // set textures
    glUniform1i( glGetUniformLocation(m_shaderPrograms["standard_prg"], "ourTexture_0"), 0 );
    glUniform1i( glGetUniformLocation(m_shaderPrograms["standard_prg"], "ourTexture_1"), 1 );
    // set compute shader result
    glUniform1i( glGetUniformLocation(m_shaderPrograms["standard_prg"], "ourComputeShaderTexture"), 2 );

    // set matrices
    glUniformMatrix4fv(glGetUniformLocation(m_shaderPrograms["standard_prg"], "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderPrograms["standard_prg"], "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderPrograms["standard_prg"], "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // set program execution switch
    glUniform1i( glGetUniformLocation(m_shaderPrograms["standard_prg"], "render_box"), 1 );


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
          glUniformMatrix4fv(glGetUniformLocation(m_shaderPrograms["standard_prg"], "model"), 1, GL_FALSE, glm::value_ptr(model));

          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // draw SSBO particles as points
    glUniform1i( glGetUniformLocation(m_shaderPrograms["standard_prg"], "render_box"), 0 );
    glBindVertexArray( VAO_points );
    glUniformMatrix4fv(glGetUniformLocation(m_shaderPrograms["standard_prg"], "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glDrawArrays( GL_POINTS, 0, 1024*1024 );

    // CLEANUP
    glBindVertexArray(0);

    // strange behaviour for this
//    m_window->resetOpenGLState();
}

// BUFFER INITIALIZATNIOS
void OpenGlRenderer::initialize()
{
    qDebug() << "Initalize called.";

    initializeOpenGLFunctions();
    QRandomGenerator rd;

    // INIT STATES
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);

    // diagnostic items
    int  success;
    char infoLog[512];

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

    //COMPUTE SHADER INFORMATION
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

    // COMPUTE SHADER - FILL TEXTURE
    //  ** create texture **
    int tex_w = 512, tex_h = 512;
//    int group_size = 32;
    GLuint tex_output;
//    m_textures.append( tex_output );
    m_imageTextures.insert("compute_texture", tex_output );
    glGenTextures(1, &m_imageTextures["compute_texture"]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_imageTextures["compute_texture"]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(0, m_imageTextures["compute_texture"], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    // ** create shader and program **
    // create compute shader
    QByteArray data = stringFromShaderFile(":/Shaders/simpleCompute").toLocal8Bit();
    const char* computeShaderSource = data.data();
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeShaderSource, nullptr);
    glCompileShader(computeShader);
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    // check compilation errors
    if(!success)
    {
        glGetShaderInfoLog(computeShader, 512, nullptr, infoLog);
        qDebug() << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED" << infoLog;
    }
    // create program object
    m_shaderPrograms.insert( "fill_texture_compute_prg", glCreateProgram() );
    glAttachShader(m_shaderPrograms["fill_texture_compute_prg"], computeShader);
    // link program
    glLinkProgram(m_shaderPrograms["fill_texture_compute_prg"]);
    // check link errors
    glGetProgramiv(m_shaderPrograms["fill_texture_compute_prg"], GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_shaderPrograms["fill_texture_compute_prg"], 512, nullptr, infoLog);
        qDebug() << "ERROR::COMPUTE::PROGRAM::COMPILATION_FAILED" << infoLog;
    }
    // clean up shader object
    glDeleteShader(computeShader);


    // POINTS COMPUTE SHADER
    // structure for particle position
    struct pos{
        float x, y, z , lifespan;
    };
    // create and bind shader storage buffer object
    GLuint posSSbo;
    glGenBuffers( 1, &posSSbo);
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, posSSbo );
    // allocate buffer storage to correct size
    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct pos), nullptr, GL_STATIC_DRAW );
    // usage bits for buffer mapping
    GLbitfield bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
    // get pointer to buffer storage, cast it as a pointer to our struct
    struct pos* points = (struct pos*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof( struct pos ), bufMask);
    // fill our buffer
    for( int i = 0; i < NUM_PARTICLES; i++){

        qreal px, py, pz;

        do{
            px = (rd.generateDouble()-0.5) * 2;
            py = (rd.generateDouble()-0.5) * 2;
            pz = (rd.generateDouble()-0.5) * 2;
        }while( qSqrt( qPow(px, 2.0) + qPow(py,2.0) + qPow(pz, 2.0) ) > 1.0 );

        points[i].x = static_cast<float>(px);
        points[i].y = static_cast<float>(py);
        points[i].z = static_cast<float>(pz);
        points[i].lifespan = static_cast<float>(1.0);
    }
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    // structure for particle velocity
    struct vel{
        float vx, vy, vz , life;
    };
    // create and bind shader storage buffer object
    GLuint velSSbo;
    glGenBuffers( 1, &velSSbo);
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, velSSbo );
    // allocate buffer storage to correct size
    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct vel), nullptr, GL_STATIC_DRAW );
    // get pointer to buffer storage, cast it as a pointer to our struct
    struct vel* velocities = (struct vel*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof( struct vel ), bufMask);
    // fill our buffer
    for( int i = 0; i < NUM_PARTICLES; i++){

        qreal vx, vy, vz;

        do{
            vx = (rd.generateDouble()-0.5) * 2;
            vy = (rd.generateDouble()-0.5) * 2;
            vz = (rd.generateDouble()-0.5) * 2;
        }while( qSqrt( qPow(vx, 2.0) + qPow(vy,2.0) + qPow(vz, 2.0) ) > 1.0 );

        velocities[i].vx = static_cast<float>(vx);
        velocities[i].vy = static_cast<float>(vy);
        velocities[i].vz = static_cast<float>(vz);
        velocities[i].life = static_cast<float>(0.0);
    }
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    // structure for particle color
    struct col{
        float r, g, b, a;
    };
    // create and bind shader storage buffer object
    GLuint colSSbo;
    glGenBuffers( 1, &colSSbo);
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, colSSbo );
    // allocate buffer storage to correct size
    glBufferData( GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(struct col), nullptr, GL_STATIC_DRAW );
    // get pointer to buffer storage, cast it as a pointer to our struct
    struct col* colors = (struct col*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof( struct col ), bufMask);
    // fill our buffer
    for( int i = 0; i < NUM_PARTICLES; i++){
        colors[i].r = static_cast<float>(rd.generateDouble());
        colors[i].g = static_cast<float>(rd.generateDouble());
        colors[i].b = static_cast<float>(rd.generateDouble());
        colors[i].a = 1.0;
    }
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

    glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  4,  posSSbo );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  5,  velSSbo );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  6,  colSSbo );

    // CREATE PARTICLES COMPUTE PROGRAM
    // create compute shader
    data = stringFromShaderFile(":/Shaders/particleCompute").toLocal8Bit();
    const char* computeParticlesSource = data.data();
    GLuint computeParticlesShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeParticlesShader, 1, &computeParticlesSource, nullptr);
    glCompileShader(computeParticlesShader);
    glGetShaderiv(computeParticlesShader, GL_COMPILE_STATUS, &success);
    // check compilation errors
    if(!success)
    {
        glGetShaderInfoLog(computeParticlesShader, 512, nullptr, infoLog);
        qDebug() << "ERROR::SHADER::PARTICLE::COMPUTE::COMPILATION_FAILED" << infoLog;
    }
    // create program onject
    m_shaderPrograms.insert("particles_compute_prg", glCreateProgram());
    glAttachShader(m_shaderPrograms["particles_compute_prg"], computeParticlesShader);
    // link program
    glLinkProgram(m_shaderPrograms["particles_compute_prg"]);
    // check link errors
    glGetProgramiv(m_shaderPrograms["particles_compute_prg"], GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_shaderPrograms["particles_compute_prg"], 512, nullptr, infoLog);
        qDebug() << "ERROR::::PARTICLE::PROGRAM::COMPILATION_FAILED" << infoLog;
    }
    // clean up shader object
    glDeleteShader(computeParticlesShader);

    // CREATE VAO FOR RENDERING POINTS
    glGenVertexArrays(1, &VAO_points);
    glBindVertexArray(VAO_points);
    // bind SSBO buffer
    glBindBuffer(GL_ARRAY_BUFFER, posSSbo);
    // set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // bind SSBO buffer
    glBindBuffer( GL_ARRAY_BUFFER, colSSbo );
    // set vertex attributes
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void*>(8* sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    // IMAGES and TEXTURES
    int image_width, image_height, image_nrChannels;
    unsigned char *image_data;
    stbi_set_flip_vertically_on_load(true);

    // texture_0
    image_data = stbi_load("container.jpg", &image_width, &image_height, &image_nrChannels, 0);
    unsigned int texture_0;
    m_imageTextures.insert("container_tex", texture_0 );
    glGenTextures(1, &m_imageTextures["container_tex"]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_imageTextures["container_tex"]);
    // filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // copy data
    if( image_data ){
        qDebug() << "pixels image data: " << image_data[0] << image_data[1] << image_data[2];
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    // free source data
    stbi_image_free(image_data);

    // experiment: read texture
    GLubyte* pixels = new GLubyte[image_width*image_height*image_nrChannels];
    glActiveTexture(GL_TEXTURE0);
    glGetTexImage( GL_TEXTURE_2D, 0, GL_RGB,  GL_UNSIGNED_BYTE, pixels);
    qDebug() << "pixels get texture: " << pixels[0] << pixels[1] << pixels[2];
    delete [] pixels;

    // texture_1
    image_data = stbi_load("awesomeface.png", &image_width, &image_height, &image_nrChannels, 0);
    unsigned int texture_1;
    m_imageTextures.insert("awesomeface_tex", texture_1 );
    glGenTextures(1, &m_imageTextures["awesomeface_tex"]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_imageTextures["awesomeface_tex"]);
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
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_MAP_READ_BIT); // testing buffer storage
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


    // GENERAL PROGRAM and SHADERS
    // SETUP VERTEX SHADER
    data = stringFromShaderFile(":/Shaders/simple.vert").toLocal8Bit();
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
    m_shaderPrograms.insert("standard_prg", glCreateProgram());
    glAttachShader(m_shaderPrograms["standard_prg"], vertexShader);
    glAttachShader(m_shaderPrograms["standard_prg"], fragmentShader);
    glLinkProgram(m_shaderPrograms["standard_prg"]);
    glGetProgramiv(m_shaderPrograms["standard_prg"], GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_shaderPrograms["standard_prg"], 512, nullptr, infoLog);
        qDebug() << "ERROR::PROGRAM::COMPILATION_FAILED" << infoLog;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


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
