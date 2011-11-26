#include "memoryindex.h"
#include <QDebug>

MemoryIndex::MemoryIndex() {}

float MemoryIndex::getIdf(const QString &term) const
{
    Q_UNUSED(term)
    return 1.0;// TODO
}
float MemoryIndex::getNorm(int docId) const
{
    Q_UNUSED(docId)
    return 1.0;// TODO
}
void MemoryIndex::addPosting(const QString &term, int docId)
{
    QList<Posting> *postingList;
    if (invertedIndex.contains(term))
        postingList = invertedIndex.value(term);
    else {
        postingList = new QList<Posting>;
        invertedIndex.insert(term, postingList);
    }

    //if last insertion was on the same doc, increment frequency
    if (!postingList->empty() && postingList->at(0).docId == docId)
        (*postingList)[0].tf++;
    else
        postingList->prepend(Posting(docId, 1));
}

PostingListIterator* MemoryIndex::getPostingList(const QString &term) const
{
    if (invertedIndex.contains(term))
        return new MemoryPostingListIterator(*(invertedIndex[term]));
    else {
        const QList<Posting> emptyPosting;
        return new MemoryPostingListIterator(emptyPosting);
    }
}

void MemoryIndex::calculateNorm(int docId)
{
    Q_UNUSED(docId)
    // TODO
}

MemoryPostingListIterator::MemoryPostingListIterator(const QList<Posting> _postingList)
    : currentPos(0), postingList(_postingList)
{}

bool MemoryPostingListIterator::hasNext()
{
    return currentPos < postingList.size();
}

Posting MemoryPostingListIterator::next()
{
    return postingList.at(currentPos++);
}
