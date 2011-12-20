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

    virtual QString doc(int docId) const { return docs[docId]; }
    virtual void storeDoc(int docId, const QString &docContent) {
        docs.insert(docId, docContent);
    }
    virtual float norm(int docId) const {
        return norms.value(docId);
    }
    virtual float setNorm(int docId, float norm) {
        norms.insert(docId, norm);
    }

private:
    QMap<QString, QList<Posting>* > invertedIndex;
    QMap<int, QString> docs;
    QMap<int, float> norms;
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
    int size();
private:
    int currentPos;
    const QList<Posting> postingList;
};

#endif // MEMORYINDEX_H
