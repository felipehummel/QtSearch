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
    
Customizing the Index
==========

All data to perform searches is stored on a `Index`. This includes vocabulary, inverted index (term's posting lists), documents content and norm. Currently QtSearch provides only `MemoryIndex`, a completely in memory index with no persistent storage.
To implement a custom `Index` one must inherit it or `MemoryIndex` and implement/reimplement the following methods:

    // method that register that a term occured in a document. This method is called at every occurrence of the term
    void addPosting(const QString &term, int docId);
    
    // returns the posting list iterator for that term. Used by the QueryProcessor to perform searches
    PostingListIterator* getPostingList(const QString &term) const;
    
    // return the doc frequency of a term. Also known as the size of its posting list
    int getDocFreq(const QString &term) const;

    // return the document textual content
    QString doc(int docId) const;
    
    // store the document textual content
    void storeDoc(int docId, const QString &docContent);
    
    // return the document norm 
    float norm(int docId) const;
    
    // store the document norm. It is already calculated by a Similarity implementation
    void setNorm(int docId, float norm)
    

When creating a custom `Index` you need a `PostingListIterator`implementation. If your index is already on memory or you can load to memory the whole posting list at once you can reuse `MemoryPostingListIterator`.
Otherwise, you can extend `PostingListIterator` and implement the following methods:

    bool hasNext();
    Posting next();
    // move the iterator to the FIRST document with id EQUAL or GREATER than the provided docId
    bool jumpTo(int docId);
    Posting current();
    int size();

Aside from the `jumpTo` method the iterator is very java-like so it is not that much trouble. `jumpTo` is used to speed up and simplify boolean AND queries.


Customizing document tokenization and analysis
==========

When the document is sent to the `Indexer` to be indexed two processes occur: 
(1) the document textual content is tokenized (splitted) according to a `Tokenizer`;
(2) the tokens emitted in the previous step are passed through a series of `TokenFilter` objects that can perform transformations on the terms.

These steps are wrapped up in a `Analyzer` object which receives a `Tokenizer` as constructor parameter
and `TokenFilter` objects to be used:

    analyzer.useFilter(new LowerCaseFilter);
    analyzer.useFilter(new AccentFilter);
    analyzer.useFilter(new BadWordsFilter);
    analyzer.useFilter(new SynonymFilter);
    
The `Analyzer` class has basic filters bultin, you can use the by calling the following methods:

    analyzer.withAccentFilter();
    analyzer.withLowerCaseFilter();
    analyzer.withStopWordFilter(stopWords); //stopwords is a QSet<QString>
    
To customize the `Analyzer` you can create a new `TokenFilter` by extending and implementing the following method:


    QStringList filter(const QString &token) const;
    
It returns a `QStringList` so a filter can remove a given token or return more than one token.

You can also customize the tokenization process by extending `Tokenizer` and implementing:

    QStringList tokenize(const QString &docContent) const;
    