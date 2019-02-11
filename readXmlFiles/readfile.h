#ifndef READFILE_H
#define READFILE_H

#include <QXmlDefaultHandler>
#include <QXmlStreamWriter>
#include <QMultiMap>
#include <memory>
#include <QString>

struct xmlElem
{
public:
  QXmlAttributes attributes;
  QString name;
  QString value;
  QMultiMap<QString, std::shared_ptr<xmlElem>> nextLevel;
  ~xmlElem();
};


class xmlHandler : public QXmlDefaultHandler
{ 
public: 
  xmlHandler(std::shared_ptr<xmlElem> newElem);
  ~xmlHandler();
  bool startElement(const QString &namespaceURI, 
                    const QString &localName, 
                    const QString &qName, 
                    const QXmlAttributes &attribs); 

  bool endElement(const QString &namespaceURI,
                  const QString &localName, 
                  const QString &qName);

  bool characters(const QString &str);
  bool fatalError(const QXmlParseException &exception);
  bool isAllStepsEmpty();
  
private: 
    std::shared_ptr<xmlElem> curElem;
    QVector<std::shared_ptr<xmlElem>> allSteps; // all of steps, that we do in xmlFile
};

class writeXmlFile
{
public:
    static bool writeFile(const QString& filePath, std::shared_ptr<xmlElem> parentElem);
private:
    static bool writeElem(std::shared_ptr<xmlElem> curElem);
    static QXmlStreamWriter xmlWriter;
};


std::shared_ptr<xmlElem> readXmlFile(QString filePath);
bool findInXmlMap(const std::shared_ptr<xmlElem> elem, const QString &key);


#endif // READFILE_H
