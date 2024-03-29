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

#include <QStringList>
#include <QDebug>

#include "queryprocessor.h"

QueryProcessor::QueryProcessor(Index *_index, const Analyzer &_analyzer, const Similarity _similarity)
    : index(_index), analyzer(_analyzer), similarity(_similarity)
{
}

bool resultLessThan(Result &a, Result &b)
{
    return a.score > b.score;
}

bool pairLessThan(QPair<QString, float> &a, QPair<QString, float> &b)
{
    return a.second > b.second;
}

bool postingItPointerLessThan(PostingListIterator *a, PostingListIterator *b)
{
    return a->size() < b->size();
}

QList<Result> QueryProcessor::searchAND(const QString &query) const
{
    QStringList terms = analyzer.analyze(query);
    if (terms.empty())
        return QList<Result>();
    QList<PostingListIterator*> postingLists = index->getPostingIterators(terms);
    qSort(postingLists.begin(), postingLists.end(), postingItPointerLessThan);

    foreach (PostingListIterator *it, postingLists) {
        if (!it->hasNext())
            return QList<Result>();
    }
    QList<Result> results;
    float *idfs = index->getIdfs(terms);
    Posting *matchedPostings = new Posting[postingLists.size()];

    forever {
        if (!postingLists[0]->hasNext())
            break;
        matchedPostings[0] = postingLists[0]->next();
        int pivotDocId = matchedPostings[0].docId;
        bool match = true;
        for (int i = 1; i < postingLists.size() && match; ++i) {
            bool found = postingLists[i]->jumpTo(pivotDocId);
            if (found && postingLists[i]->current().docId == pivotDocId)
                matchedPostings[i] = postingLists[i]->current();
            else
                match = false;
        }
        if (match) {
            float score = calculateScore(idfs, matchedPostings, postingLists.size());
            results.append(Result(pivotDocId, score));
        }
    }

    delete[] matchedPostings;
    delete[] idfs;
    foreach(PostingListIterator *it, postingLists) {
        delete it;
    }
    qSort(results.begin(), results.end(), resultLessThan);
    return results;
}

QList<Result> QueryProcessor::searchOR(const QString &query) const
{

    QHash <int, float> accumHash;
    float score;

    QStringList terms = analyzer.analyze(query);
    float *idfs = index->getIdfs(terms);
    if (terms.empty())
        return QList<Result>();
    QList<PostingListIterator*> postingLists = index->getPostingIterators(terms);

    int termIndex = 0;


    foreach (PostingListIterator *it, postingLists) {
        while(it->hasNext()) {
            Posting posting = it->next();
               accumHash[posting.docId] = accumHash[posting.docId]
                    + (idfs[termIndex] * similarity.tf(posting.tf));
        }
        termIndex++;
    }

    QList<Result> results;

    QHash<int, float>::const_iterator i = accumHash.constBegin();
    while (i != accumHash.constEnd()) {
        score = i.value() * index->norm(i.key());
        results.append(Result(i.key(),score));
        i++;
    }

    foreach(PostingListIterator *it, postingLists)
        delete it;
    delete[] idfs;
    qSort(results.begin(), results.end(), resultLessThan);
    return results;
}

QList <QPair <QString, float> > QueryProcessor::selectKeywords(const QString &doc) const
{
    QStringList terms = analyzer.analyze(doc);
    QList < QPair<QString, float> > rankedListTerms;
    QPair <QString, float> pair;

    for (int i = 0; i < terms.size(); ++i) {
        pair.first = terms.at(i);
        pair.second = index->getIdf(terms.at(i));
        rankedListTerms << pair;
    }

    qSort(rankedListTerms.begin(), rankedListTerms.end(), pairLessThan);
    return rankedListTerms;
}

QList<Result> QueryProcessor::recommendedDocuments(const QString &doc) const
{
    // Select and rank keywords according to idf
    QList <QPair <QString, float> > keywords = selectKeywords(doc);

    // Query will be the 10 most relevant words (according to idf)
    QString query;
    for (int i = 0; i < keywords.size() && i < 10; i++) {
        if (keywords.at(i).second != INFINITY)
            query = query + " " + keywords.at(i).first;
    }

    // Return the recommended documents
    QList<Result> results;
    results = searchOR(query);
    return results;
}


float QueryProcessor::calculateScore(float *idfs, const Posting *postings, int numTerms) const
{
    float accum = 0.0;
    for (int i = 0; i < numTerms; ++i) {
        accum += idfs[i] * similarity.tf(postings[i].tf);
    }
    return accum * index->norm(postings[0].docId);
}
