#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QDebug>
#include <QWindow>

class OpenGlRenderer : public QObject, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    OpenGlRenderer( QObject *parent  = nullptr );
    void setWindow( QWindow* const window );

private:
    QWindow* m_window = nullptr;

public slots:
    void paint();
};

#endif // OPENGLRENDERER_H
