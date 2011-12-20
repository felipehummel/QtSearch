QtSearch Library
===============

Open Source Lib implementing a simple search engine (vector space model) in C++ using the Qt framework. The code is intendend to be extremely simple to use, understand and still be flexible to extensions.
This lib has some large inspiration on Lucene library design.

Usage
===========

    Similarity sim;
    Index *index = new MemoryIndex(sim);
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
    
    // index single string documents
    indexer.indexDocument("example document");
    indexer.indexDocument("example document two");
    
    // index a whole dir, each file as a document
    indexer.indexDir("documentsDir", "UTF-8");
        
    // index a whole file as one document per line
    indexer.indexFileDocPerLine("documents.txt", "UTF-8");
    
    QueryProcessor processor(index, analyzer, sim);

    QTextStream stream(stdin);
    stream.setCodec("UTF-8");
    QString line;

    
    // read queries from stdin and perform a boolean AND search
    do {
        qDebug() << "Input you query\n";
        line = stream.readLine();
        
        QList<Result> results = processor.searchAND(line); // searchOR(query) also available
        
        foreach (Result result, results) {
            qDebug () << result.score << " :: " << index->doc(result.docId);
        }
    } while (!line.isNull());


Simple Document Recommendation
============

QtSearch has a simple approach for document recommendation. You provide a string document representing the user profile and it provides the documents most suited for that profile. It uses a simple approach of selecting the top-K most meaningful terms (greatest IDF) in the profile. 

    qDebug() << "Input your document\n";
    line = stream.readLine();
    qDebug() << "Documents recommended\n";
    QList<Result> recommendedDocs = processor.recommendedDocuments(line);
    foreach (Result result, recommendedDocs) {
        qDebug () << result.score << " :: " << index->doc(result.docId);
    }