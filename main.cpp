#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include <QWindow>
#include <QDebug>

#include "openglrenderer.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
                     [url](QObject *obj, const QUrl &objUrl) { if (!obj && url == objUrl) QCoreApplication::exit(-1); },
                    Qt::QueuedConnection);
    engine.load(url);


    QList<QObject*> root_objects = engine.rootObjects();
    QWindow *ui_window = qobject_cast<QWindow*>(root_objects.at(0));
    qDebug() << ui_window->objectName();

    OpenGlRenderer renderer;
    renderer.setWindow( ui_window );


    QObject::connect( ui_window, SIGNAL(afterRendering()), &renderer, SLOT(paint()), Qt::DirectConnection );


    return app.exec();
}
