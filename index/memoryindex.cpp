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

#include "memoryindex.h"
#include <QDebug>

MemoryIndex::MemoryIndex(Similarity _sim) : Index(_sim) {}

int MemoryIndex::getDocFreq(const QString &term) const
{
    if (!invertedIndex.contains(term))
        return 0;
    else
        return invertedIndex.value(term)->size();
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

    //if last insertion (last element) was on the same doc, increment frequency
    if (!postingList->empty() && postingList->last().docId == docId)
        (*postingList)[0].tf++;
    else
        postingList->append(Posting(docId, 1));
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

Posting MemoryPostingListIterator::current() {
    return postingList.at(currentPos);
}

bool MemoryPostingListIterator::jumpTo(int docId)
{
    // TODO - don't use next(). Instead walk the postingList directly
    if (currentPos < postingList.size()) { // check if already exausted or empty
        int current = postingList[currentPos].docId;
        while (hasNext() && current < docId) {
            current = next().docId;
        }
        if (currentPos > 0)
            currentPos--;
        return current == docId;
    }
    return false;
}

int MemoryPostingListIterator::size()
{
    return postingList.size();
}
