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
