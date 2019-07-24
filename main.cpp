#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include <QQuickWindow>
#include <QQmlContext>
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

    // get root object ( QQuickWindow )
    QList<QObject*> root_objects = engine.rootObjects();
    QQuickWindow *ui_window = qobject_cast<QQuickWindow*>(root_objects.at(0));
    qDebug() << ui_window->objectName();

    ui_window->setClearBeforeRendering(false);

    // get root context
    QQmlContext *root_context = engine.rootContext();

    OpenGlRenderer renderer;
    renderer.setWindow( ui_window );

    root_context->setContextProperty("_renderer", &renderer);


    QObject::connect( ui_window, SIGNAL(sceneGraphInitialized()), &renderer, SLOT(initialize()), Qt::DirectConnection );
    QObject::connect( ui_window, SIGNAL(sceneGraphInvalidated()), &renderer, SLOT(cleanup()), Qt::DirectConnection );
    QObject::connect( ui_window, SIGNAL(beforeRendering()), &renderer, SLOT(paint()), Qt::DirectConnection );

    return app.exec();
}
