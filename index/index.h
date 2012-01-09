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

#ifndef INDEX_H
#define INDEX_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QDebug>
#include <math.h>
#include <similarity/similarity.h>


struct Posting {
    int docId;
    int tf; // Term Frequency
    Posting(int _docId, int _tf) : docId(_docId), tf(_tf) {}
    Posting() : docId(-1), tf(-1) {}
    QString toString() {
        return QString("Posting(docId=%1, tf=%2)")
                .arg(docId).arg(tf);
    }
};


class PostingListIterator
{
public:
    PostingListIterator() {}
    virtual bool hasNext() = 0;
    virtual Posting next() = 0;
    virtual Posting current() = 0;
    virtual int size() = 0;
    virtual bool jumpTo(int docId) = 0;
    QList<Posting> toList() {
        // TODO - do not exaust the iterator (or reset it)
        QList<Posting> postingList;
        while(hasNext()) {
            postingList.prepend(next());
        }
        return postingList;
    }
};

class Index
{
public:
    Index(Similarity _similarity = Similarity()) : similarity(_similarity), docIdCounter(0) {}

    virtual PostingListIterator* getPostingList(const QString &term) const = 0;
    float getIdf(const QString &term) const {
        return similarity.idf(getDocFreq(term), docIdCounter, term);
    }
    virtual float norm(int docId) const = 0;


    virtual int addDocument(QList<QString> terms) {
        int docId = nextDocId();
        foreach (const QString term, terms) {
            addPosting(term, docId);
        }
        // Simple doc length norm. Extracted from Lucene's DefaultSimilarity
        setNorm(docId, similarity.norm(terms.size()));
        return docId;
    }
    virtual int getDocFreq(const QString &term) const = 0;


    virtual QString doc(int docId) const = 0;
    virtual void storeDoc(int docId, const QString &docContent) = 0;

    // Utility methods

    float* getIdfs(const QStringList &terms) const
    {
        float *idfs = new float[terms.size()];
        for (int i = 0; i < terms.size(); ++i) {
            idfs[i] = getIdf(terms.at(i));
        }
        return idfs;
    }

    QList<PostingListIterator*> getPostingIterators(const QStringList &terms) const {
        QList<PostingListIterator*> lists;
        foreach(QString term, terms) {
            lists.append(getPostingList(term));
        }
        return lists;
    }

protected:
    const Similarity similarity;
    virtual void addPosting(const QString &term, int docId) = 0;
    virtual void setNorm(int docId, float norm) = 0;
private:
    int nextDocId() { return docIdCounter++; }
    int docIdCounter;
};

#endif // INDEX_H
