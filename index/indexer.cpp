#include <QRegExp>

#include "index/indexer.h"

Indexer::Indexer(Index *_index, Tokenizer _tokenizer)
    : index(_index), tokenizer(_tokenizer){}

int Indexer::indexDocument(QString docContent) {
    QStringList docTerms = tokenizer.tokenize(docContent);
    int docId = index->addDocument(docTerms);
    index->storeDoc(docId, docContent);
    return docId;
}

QStringList Tokenizer::tokenize(const QString &docContent) const {
    return docContent.split(QRegExp("[^\\w0-9]+"), QString::SkipEmptyParts);
}

bool Indexer::indexFileDocPerLine(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        indexDocument(line);
        line = in.readLine();
    }
    return true;
}
