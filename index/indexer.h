#ifndef INDEXER_H
#define INDEXER_H

#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "index/index.h"

class Tokenizer {
public:
    virtual QStringList tokenize(const QString &docContent) const;
};

class Indexer
{
public:
    Indexer(Index *_index, Tokenizer tokenizer = Tokenizer());
    virtual int indexDocument(QString docContent);
    const Index* getIndex() { return index; }
    bool indexFileDocPerLine(const QString &filePath);

private:
    Index *index;
    Tokenizer tokenizer;
};
#endif // INDEXER_H
