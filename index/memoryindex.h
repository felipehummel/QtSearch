#ifndef MEMORYINDEX_H
#define MEMORYINDEX_H

#include <QMap>
#include "index.h"

class MemoryIndex : public Index
{
public:
    MemoryIndex();
    float getNorm(int docId) const;
    void addPosting(const QString &term, int docId);
    PostingListIterator* getPostingList(const QString &term) const;
    void calculateNorm(int docId);
    int getDocFreq(const QString &term) const;

private:
    QMap<QString, QList<Posting>* > invertedIndex;
};


class MemoryPostingListIterator : public PostingListIterator
{
public:
    MemoryPostingListIterator(const QList<Posting> postingList);
    bool hasNext();
    Posting next();
private:
    int currentPos;
    const QList<Posting> postingList;
};

#endif // MEMORYINDEX_H
