#include <QRegExp>
#include <QDir>

#include "index/indexer.h"
#include "analyzer/tokenstream.h"

Indexer::Indexer(Index *_index, const Analyzer &_analyzer)
    : index(_index), analyzer(_analyzer){}

int Indexer::indexDocument(QString docContent) {
    QStringList docTerms = analyzer.analyze(docContent);
    int docId = index->addDocument(docTerms);
    index->storeDoc(docId, docContent);
    return docId;
}

bool Indexer::indexFileDocPerLine(const QString &filePath, const char *codec)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    in.setCodec(codec);
    QString line = in.readLine();
    while (!line.isNull()) {
        indexDocument(line);
        line = in.readLine();
    }
    return true;
}

bool Indexer::indexDir(const QString &dirPath, const char *codec)
{
    QDir dir = QDir(dirPath);

    if (!dir.exists()) {
        qDebug() << "Directory does not exist.";
        return false;
    }

    QList<QFileInfo> entryInfoList;
    entryInfoList = dir.entryInfoList();
    QFile file;
    QString text;

    for (int i = 0; i < entryInfoList.size(); i++) {

        if (entryInfoList.at(i).baseName() == "")
            continue;

        file.setFileName(entryInfoList.at(i).absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            indexDir(entryInfoList.at(i).absoluteFilePath(), "UTF-8");
            continue;
        }

        QTextStream in(&file);
        in.setCodec(codec);
        text = in.readAll();
        indexDocument(text);
        file.close();
    }

    return true;
}
