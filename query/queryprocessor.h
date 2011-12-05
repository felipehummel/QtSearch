#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H

#include "index/index.h"
#include "index/indexer.h"
#include "analyzer/analyzer.h"

struct Result {
    int docId;
    float score;
    Result(int _docId, float _score) : docId(_docId), score(_score) {}
};

class QueryProcessor
{
public:
    QueryProcessor(Index *_index,
                   const Analyzer &_analyzer = StandardAnalyzer(),
                   const Similarity _similarity = Similarity()) ;

    QList<Result> searchAND(const QString &query) const;
    QList<Result> searchOR(const QString &query) const;
    QList<Result> recommendedDocuments(const QString &doc) const;

    const Index* getIndex() { return index; }
private:
    float calculateScore(float *idfs, const Posting *postings, int numTerms) const;
    QList <QPair <QString, float> > selectKeywords(const QString &doc) const;

    const Index *index;
    const Analyzer analyzer;
    const Similarity similarity;
};

#endif // QUERYPROCESSOR_H
