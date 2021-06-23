#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QString>
#include "httplistener.h"
#include "httprequesthandler.h"
#include "requestmapper.h"
#include "global.h"
#include "cookietestcontroller.h"

#include "xmlcontroller.h"
#include <QtCore>
#include <QtXml>
#include <QDebug>

using namespace stefanfrings;

/**
 * Search the configuration file.
 * Aborts the application if not found.
 * @return The valid filename
 */
QString searchConfigFile() {
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QFile file;
    file.setFileName(binDir+"/webapp1.ini");
    if (!file.exists()) {
        file.setFileName(binDir+"/etc/webapp1.ini");
        if (!file.exists()) {
            file.setFileName(binDir+"/../etc/webapp1.ini");
            if (!file.exists()) {
                file.setFileName(binDir+"/../"+appName+"/etc/webapp1.ini");
                if (!file.exists()) {
                    file.setFileName(binDir+"/../../"+appName+"/etc/webapp1.ini");
                    if (!file.exists()) {
                        file.setFileName(binDir+"/../../../../../"+appName+"/etc/webapp1.ini");
                        if (!file.exists()) {
                            file.setFileName(QDir::rootPath()+"etc/webapp1.ini");
                        }
                    }
                }
            }
        }
    }
    if (file.exists()) {
        QString configFileName=QDir(file.fileName()).canonicalPath();
        qDebug("using config file %s", qPrintable(configFileName));
        return configFileName;
    }
    else {
        qFatal("config file not found");
    }
}


//so first of all let's write our recutsive DOM function which
//will traverse all the nodes and output what we need

void traverse(const QDomNode &node) {
    QDomNode domNode = node.firstChild();

    //while loop to go and go till hit empty
    while (!domNode.isNull()) {
        if (domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if (!domElement.isNull()) {
                if (domElement.tagName() == "orderperson")
                    qDebug() << "Name: " << domElement.attribute("name", "");
                else
                    qDebug() << "\t" << domElement.tagName() << ": " << domElement.text();
            }
        }
        traverse(domNode);
        domNode = domNode.nextSibling();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Load the configuration file
    QString configFileName=searchConfigFile();
    QSettings* listenerSettings=new QSettings(configFileName, QSettings::IniFormat, &app);
    listenerSettings->beginGroup("listener");

    // Configure logging
    QSettings* logSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    logSettings->beginGroup("logging");
    logger=new FileLogger(logSettings,10000,&app);
    logger->installMsgHandler();

    // Log the library version
    qDebug("QtWebApp has version %s",getQtWebAppLibVersion());

    // Session store
    QSettings* sessionSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    sessionSettings->beginGroup("sessions");
    sessionStore=new HttpSessionStore(sessionSettings,&app);

    // Static file controller
    QSettings* fileSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    fileSettings->beginGroup("files");
    staticFileController=new StaticFileController(fileSettings,&app);

    // Configure template cache
    QSettings* templateSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    templateSettings->beginGroup("templates");
    templateCache=new TemplateCache(templateSettings,&app);

    QDomDocument doc;
    QFile file("xmlFile.xml");  //let's open it and read into console
    if (file.open(QIODevice::ReadOnly)) {
        if (doc.setContent(&file))  //if successfull then proceed
        {
            QDomElement elem = doc.documentElement();
            traverse(elem);
        }
        file.close();
    }
    return 0;






    // Start the HTTP server
    new HttpListener(listenerSettings,new RequestMapper(&app),&app);

  //  return app.exec();


}
