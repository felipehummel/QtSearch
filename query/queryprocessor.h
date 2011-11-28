#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H

#include "index/index.h"
#include "index/indexer.h"

struct Result {
    int docId;
    float score;
    Result(int _docId, float _score) : docId(_docId), score(_score) {}
};

class QueryProcessor
{
public:
    QueryProcessor(Index *_index, Tokenizer _tokenizer = Tokenizer());

    QList<Result> searchAND(const QString &query) const;

    const Index* getIndex() { return index; }
private:
    float calculateScore(float *idfs, const Posting *postings, int numTerms) const;

    const Index *index;
    const Tokenizer tokenizer;
};

#endif // QUERYPROCESSOR_H
