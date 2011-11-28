#include <QStringList>
#include <QDebug>

#include "queryprocessor.h"

QueryProcessor::QueryProcessor(Index *_index, Tokenizer _tokenizer)
    : index(_index), tokenizer(_tokenizer)
{
}
bool resultLessThan(Result &a, Result &b) {
    return a.score > b.score;
}

QList<Result> QueryProcessor::searchAND(const QString &query) const {
    QStringList terms = tokenizer.tokenize(query);
    if (terms.empty())
        return QList<Result>();
    QList<PostingListIterator*> postingLists = index->getPostingIterators(terms);
    foreach (PostingListIterator *it, postingLists) {
        if (!it->hasNext())
            return QList<Result>();
    }
    QList<Result> results;
    float *idfs = index->getIdfs(terms);
    Posting *matchedPostings = new Posting[postingLists.size()];
    // TODO - sort postingListsIterator by size. Use smaller as pivot
    forever {
        if (!postingLists[0]->hasNext())
            break;
        Posting pivotPosting = postingLists[0]->next();
        int pivotDocId = pivotPosting.docId;
        bool match = true;
        matchedPostings[0] = pivotPosting;
        for (int i = 1; i < postingLists.size() && match; ++i) {
            postingLists[i]->jumpTo(pivotDocId);
            if (postingLists[i]->hasNext()) {
                Posting posting = postingLists[i]->next();
                if (posting.docId == pivotDocId)
                    matchedPostings[i] = posting;
                else
                    match = false;
            }
            else
                match = false;
        }
        if (match) {
            float score = calculateScore(idfs, matchedPostings, postingLists.size());
            results.append(Result(pivotDocId, score));
        }
    }

    delete matchedPostings;
    delete idfs;
    foreach(PostingListIterator *it, postingLists) {
        delete it;
    }
    qSort(results.begin(), results.end(), resultLessThan);
    return results;
}


float QueryProcessor::calculateScore(float *idfs, const Posting *postings, int numTerms) const
{
    float accum = 0.0;
    for (int i = 0; i < numTerms; ++i) {
        accum += idfs[i] * postings[i].tf;
    }
    return accum * index->getNorm(postings[0].docId);
}
