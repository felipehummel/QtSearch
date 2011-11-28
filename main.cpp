#include <QDebug>
#include <QStringList>
#include <QTextStream>

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

    QTextStream stream(stdin);
    QString line;
    do {
        qDebug() << "Input you query: ---------------\n";
        line = stream.readLine();
        QList<Result> results = processor.searchAND(line);
        foreach (Result result, results) {
            qDebug () << result.score << " :: " << index->doc(result.docId);
        }

    } while (!line.isNull());
    return 1;
}
