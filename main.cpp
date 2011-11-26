#include <QDebug>
#include <QStringList>

#include "index/memoryindex.h"
#include "index/indexer.h"
#include "query/queryprocessor.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc); Q_UNUSED(argv)
    Index *index = new MemoryIndex();
    Indexer indexer(index, Tokenizer());
    indexer.indexDocument("a casa do cachorro foi parar lá distante longe pra caramba");
    indexer.indexDocument("teste blablabla cara caramba caraô casa do caramba");
    indexer.indexDocument("eu quis dizer você não quis escutar não quero promessas");
    indexer.indexDocument("quero te dizer que você vai se lascar");

    QueryProcessor processor(index, Tokenizer());
    QList<Result> results = processor.searchAND("casa");
    foreach (Result result, results) {
        qDebug () << result.score << " :: " << index->doc(result.docId);
    }

    return 1;
}
