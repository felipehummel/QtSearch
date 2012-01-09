/*******************************************************************************
**
** This file is part of QtSearch **
**
**
** Name: Qt Search
** Version: Beta
** Contact: Felipe da Costa Hummel <felipehummel@gmail.com>
**          Maisa Vidal Teixeira <maisa.vidal@openbossa.org>
**
** Qt Search is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** any later version.
**
** Qt Search is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
*******************************************************************************/

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
