#include <QDebug>
#include <QStringList>

#include "index/memoryindex.h"
#include "index/indexer.h"
#include "query/queryprocessor.h"

int main(int argc, char *argv[])
{
    Index *index = new MemoryIndex();
    Indexer indexer(index, Tokenizer());
    indexer.indexDocument("example document");
    indexer.indexDocument("example document two");
    if (argc > 1) {
        indexer.indexFileDocPerLine(QString(argv[1]));
    }

    QueryProcessor processor(index, Tokenizer());
    QList<Result> results = processor.searchAND("example");
    foreach (Result result, results) {
        qDebug () << result.score << " :: " << index->doc(result.docId);
    }

    return 1;
}
