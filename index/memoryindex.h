#ifndef MEMORYINDEX_H
#define MEMORYINDEX_H

#include <QMap>
#include "index.h"
#include "similarity/similarity.h"

class MemoryIndex : public Index
{
public:
    MemoryIndex(Similarity _sim);
    void addPosting(const QString &term, int docId);
    PostingListIterator* getPostingList(const QString &term) const;
    int getDocFreq(const QString &term) const;
    Posting current();

private:
    QMap<QString, QList<Posting>* > invertedIndex;
};


class MemoryPostingListIterator : public PostingListIterator
{
public:
    // QList has copy-on-write semantics, so its alright to copy
    MemoryPostingListIterator(const QList<Posting> postingList);
    bool hasNext();
    Posting next();
    bool jumpTo(int docId);
    Posting current();
private:
    int currentPos;
    const QList<Posting> postingList;
};

#endif // MEMORYINDEX_H
