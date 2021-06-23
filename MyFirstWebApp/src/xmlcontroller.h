#ifndef XMLCONTROLLER_H
#define XMLCONTROLLER_H

#include <httprequesthandler.h>

using namespace stefanfrings;

class XmlController : public HttpRequestHandler {
        Q_OBJECT

public:
     XmlController(QObject *parent=0);
     void service(HttpRequest& request, HttpResponse& response);
private:
    QList<QString> xml;
};

#endif // XMLCONTROLLER_H

