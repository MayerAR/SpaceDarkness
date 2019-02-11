#include "readfile.h"
#include <QFile>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>
#include <memory>
#include <QDebug>

xmlElem::~xmlElem()
{
    //deletet nextLevel and delete shared_ptr<xmlElem>
}

xmlHandler::xmlHandler(std::shared_ptr<xmlElem> newElem): curElem(newElem)
{
    allSteps.clear();
}

xmlHandler::~xmlHandler(){}

bool xmlHandler::startElement(const QString &namespaceURI,
                           const QString &localName,
                           const QString &qName,
                           const QXmlAttributes &attribs)

{
    Q_UNUSED(namespaceURI); //we dont use namespase
    Q_UNUSED(localName);    //we dont use namespace
    std::shared_ptr<xmlElem> newElem(new xmlElem);
    newElem->attributes = attribs;
    newElem->name = qName;
    curElem->nextLevel.insert(newElem->name, newElem);
    allSteps.push_back(curElem);
    curElem = newElem;
    return true;
}

bool xmlHandler::endElement(const QString &namespaceURI,
                         const QString &localName,
                         const QString &qName)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(localName);
    if(curElem->name != qName)
    {
        qWarning("Error! Wrong curElem or wrong xmlFile");
        return false;
    }
    curElem = allSteps.back();
    allSteps.pop_back();
    return true;
}

bool xmlHandler::characters(const QString &str)
{
    curElem->value = str;
    return true;
}

bool xmlHandler::fatalError(const QXmlParseException &exception)
{
    qWarning("Line %d, column %d: ", exception.lineNumber(),
             exception.columnNumber());
    qDebug()<<"Message: "<<exception.message();
    return false;
}

bool xmlHandler::isAllStepsEmpty() //if we finished reading the file and allSteps is not empty --> throw error
{
    return allSteps.empty();
}


std::shared_ptr<xmlElem> readXmlFile(QString filePath)
{
    std::shared_ptr<xmlElem> parentElem(new xmlElem);
    parentElem->name = filePath;

    QFile* file = new QFile(filePath);
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw filePath;
    }

    QXmlSimpleReader reader;
    QXmlInputSource *source = new QXmlInputSource(file);
    xmlHandler handler(parentElem);
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);

    if(!reader.parse(source) && !handler.isAllStepsEmpty())
        qDebug()<<"Couldn't parse xmlFile: "<<filePath;

    file->close();
    delete file;
    delete source;
    return parentElem;
}


QXmlStreamWriter writeXmlFile::xmlWriter;

bool writeXmlFile::writeFile(const QString &filePath, std::shared_ptr<xmlElem> parentElem)
{
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"Couldn't open xmlFile: "<<filePath;
        return false;
    }
    xmlWriter.setDevice(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    for(auto elem: parentElem->nextLevel)
        writeXmlFile::writeElem(elem);
    xmlWriter.writeEndDocument();
    file.close();
    return true;
}

bool writeXmlFile::writeElem(std::shared_ptr<xmlElem> curElem)
{
    xmlWriter.writeStartElement(curElem->name);
    for(int i = 0, length = curElem->attributes.length(); i < length; i++)
    {
        xmlWriter.writeAttribute(curElem->attributes.localName(i), curElem->attributes.value(i));
    }
    if(curElem->value != "")
        xmlWriter.writeCharacters(curElem->value);

    for(auto elem: curElem->nextLevel)
        writeElem(elem);

    xmlWriter.writeEndElement();
    return true;
}

bool findInXmlMap(const std::shared_ptr<xmlElem> elem, const QString &key)
{
    auto iter = elem->nextLevel.find(key);
    if(iter == elem->nextLevel.end())
    {
        qDebug()<<"Error! I don't find "<<key<<" in xml file, with name: "<<elem->name;
        return false;
    }
    return true;
}
