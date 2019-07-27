#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <QObject>
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLContext>
#include <QDebug>
#include <QQuickWindow>
#include <QFile>
#include <QByteArray>
#include <QRandomGenerator>
#include <QtMath>

#include "stb_image.h"
#include "glm_pch.h"


// ALLOCATE COMPUTE SHADER BUFFERS
#define NUM_PARTICLES 256*256
#define WORK_GROUP_SIZE 128


class OpenGlRenderer : public QObject, protected QOpenGLFunctions_4_4_Core
{
    Q_OBJECT

    Q_PROPERTY(qreal time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(bool gswitch READ gswitch WRITE setGswitch )

public:
    OpenGlRenderer( QObject *parent  = nullptr );
    void setWindow( QQuickWindow* const window );
    const QString stringFromShaderFile( QString url );

    // time property
    qreal time() const { return m_time; }
    void setTime(qreal in_time );

    // geometry switch property
    bool gswitch() const { return m_gswitch; }
    void setGswitch( bool in_bool );

private:
    QQuickWindow* m_window = nullptr;
    qreal m_time;
    bool m_gswitch = false;

    unsigned int VAO_tri;
    unsigned int VAO_quad;
    unsigned int VAO_box;
    unsigned int VAO_points;

public slots:
    void paint();
    void initialize();
    void cleanup();
    void contextInfo();

signals:
    void timeChanged();

};

#endif // OPENGLRENDERER_H
