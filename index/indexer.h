#ifndef INDEXER_H
#define INDEXER_H


#include <QFile>
#include <QTextStream>

#include "index/index.h"
#include "analyzer/analyzer.h"

class Indexer
{
public:
    Indexer(Index *_index, const Analyzer &_analyzer = StandardAnalyzer());
    virtual int indexDocument(QString docContent);
    const Index* getIndex() { return index; }
    bool indexFileDocPerLine(const QString &filePath, const char *codec);

private:
    Index *index;
    Analyzer analyzer;
};
#endif // INDEXER_H
