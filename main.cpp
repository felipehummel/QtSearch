#include <QDebug>
#include <QStringList>

#include "index/memoryindex.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc); Q_UNUSED(argv)
    Index *index = new MemoryIndex();
    QStringList list;
    list << "teste";
    list << "teste1";
    list << "teste1";
    list << "teste1";
    list << "teste1";
    list << "teste2";
    index->addDocument(list);
    PostingListIterator *it = index->getPostingList(QString("teste1"));
    qDebug() << "AKI: " << it->hasNext();
    while (it->hasNext()) {
        Posting posting = it->next();
        qDebug() << posting.toString();
    }
    return 1;
}
