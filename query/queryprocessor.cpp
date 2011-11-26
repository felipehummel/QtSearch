#include "queryprocessor.h"
#include <QStringList>
#include <QDebug>

QueryProcessor::QueryProcessor(Index *_index, Tokenizer _tokenizer)
    : index(_index), tokenizer(_tokenizer)
{
}
bool resultLessThan(Result &a, Result &b) {
    return a.score < b.score;
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
    QList<float> idfs = index->getIdfs(terms);
    QList<Result> results;
    forever {
        if (!postingLists[0]->hasNext())
            break;
        Posting pivotPosting = postingLists[0]->next();
        int pivotDocId = pivotPosting.docId;
        bool match = true;
        QList<Posting> matchedPostings;
        matchedPostings.reserve(postingLists.size());
        matchedPostings.append(pivotPosting);
        for (int i = 1; i < postingLists.size() && match; ++i) {
            postingLists[i]->jumpTo(pivotDocId);
            if (postingLists[i]->hasNext()) {
                Posting posting = postingLists[i]->next();
                if (posting.docId == pivotDocId)
                    matchedPostings.append(posting);
                else
                    match = false;
            }
            else
                match = false;
        }
        if (match) {
            float score = calculateScore(idfs, matchedPostings);
            results.append(Result(pivotDocId, score));
        }
    }

    foreach(PostingListIterator *it, postingLists) {
        delete it;
    }
    qSort(results.begin(), results.end(), resultLessThan);
    return results;
}


float QueryProcessor::calculateScore(const QList<float> &idfs, const QList<Posting> postings) const
{
    float accum = 0.0;
    for (int i = 0; i < idfs.size(); ++i) {
        accum += idfs.at(i) * postings.at(i).tf;
    }
    return accum;
}
