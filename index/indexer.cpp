#include <QRegExp>

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
