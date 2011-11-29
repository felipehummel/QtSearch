#include <QDebug>
#include <QStringList>
#include <QTextStream>
#include <QTime>

#include "index/memoryindex.h"
#include "index/indexer.h"
#include "query/queryprocessor.h"
#include "analyzer/analyzer.h"

int main(int argc, char *argv[])
{
    Index *index = new MemoryIndex();
    Tokenizer tokenizer;
    Analyzer analyzer(tokenizer);
    QSet<QString> stopWords;
    stopWords.insert("the");
    stopWords.insert("this");
    stopWords.insert("that");

    analyzer.withAccentFilter();
    analyzer.withLowerCaseFilter();
    analyzer.withStopWordFilter(stopWords);

    Indexer indexer(index, analyzer);
    indexer.indexDocument("example document");
    indexer.indexDocument("example document two");
    if (argc > 1) {
        indexer.indexFileDocPerLine(QString(argv[1]), "UTF-8");
    }

    QueryProcessor processor(index, analyzer);

    QTextStream stream(stdin);
    stream.setCodec("UTF-8");
    QString line;
    do {
        qDebug() << "Input you query: ---------------\n";
        line = stream.readLine();
        int x = QTime::currentTime().msec();
        QList<Result> results = processor.searchAND(line);
        int y = QTime::currentTime().msec();
        qDebug () << "Took " << (y-x) << "ms";
        foreach (Result result, results) {
            qDebug () << result.score << " :: " << index->doc(result.docId);
        }

    } while (!line.isNull());
    return 1;
}
