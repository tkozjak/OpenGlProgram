#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QDebug>
#include <QQuickWindow>
#include <QFile>
//#include <QString>
#include <QByteArray>

class OpenGlRenderer : public QObject, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY timeChanged)

public:
    OpenGlRenderer( QObject *parent  = nullptr );
    void setWindow( QQuickWindow* const window );
    const QString stringFromShaderFile( QString url );

    // time property
    qreal t() const { return m_time; }
    void setT(qreal t);

private:
    QQuickWindow* m_window = nullptr;
    qreal m_time;

public slots:
    void paint();

signals:
    void timeChanged();

};

#endif // OPENGLRENDERER_H
