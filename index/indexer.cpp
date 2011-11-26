#include <QRegExp>

#include "index/indexer.h"

Indexer::Indexer(Index *_index, Tokenizer _tokenizer)
    : index(_index), tokenizer(_tokenizer){}

void Indexer::indexDocument(QString docContent) {
    QStringList docTerms = tokenizer.tokenize(docContent);
    int docId = index->addDocument(docTerms);
    index->calculateNorm(docId);
    index->storeDoc(docId, docContent);
}

QStringList Tokenizer::tokenize(QString docContent) {
    return docContent.split(QRegExp("[^\\w0-9]+"), QString::SkipEmptyParts);
}
