#include <QDebug>
#include <QStringList>
#include <QTextStream>
#include <QTime>
#include <QDir>

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
    indexer.indexDocument("example document document two lala");
    if (argc > 1) {
        QDir dir = QDir(QString(argv[1]));
        if (dir.exists())
            indexer.indexDir(QString(argv[1]), "UTF-8");
        else
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

        qDebug() << "======= RESPONSE FOR QUERY USING OR";
        x = QTime::currentTime().msec();
        results = processor.searchOR(line);
        y = QTime::currentTime().msec();
        qDebug () << "Took " << (y-x) << "ms";
        foreach (Result result, results) {
            qDebug () << result.score << " :: " << index->doc(result.docId);
        }


    } while (!line.isNull());
    return 1;
}
