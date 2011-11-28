#ifndef INDEX_H
#define INDEX_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QDebug>
#include <math.h>


struct Posting {
    const int docId;
    int tf; // Term Frequency
    Posting(int _docId, int _tf) : docId(_docId), tf(_tf) {}
    QString toString() {
        return QString("Posting(docId=%1, tf=%2)")
                .arg(docId).arg(tf);
    }
};

class PostingListIterator
{
public:
    PostingListIterator() {}
    virtual bool hasNext() = 0;
    virtual Posting next() = 0;
    virtual void jumpTo(int docId) = 0;
    QList<Posting> toList() {
        QList<Posting> postingList;
        while(hasNext()) {
            postingList.prepend(next());
        }
        return postingList;
    }
};

class Index
{
public:
    Index() : docIdCounter(0) {}
    virtual PostingListIterator* getPostingList(const QString &term) const = 0;
    virtual float getIdf(const QString &term) const {
        return log((float)(docIdCounter + 1) / (float)getDocFreq(term)); // + 1 to avoid idf = 0
    }
    virtual float getNorm(int docId) const {
        return norms.value(docId);
    }

    virtual int addDocument(QList<QString> terms) {
        int docId = nextDocId();
        foreach (const QString term, terms) {
            addPosting(term, docId);
        }
        // Simple doc length norm. Extracted from Lucene's DefaultSimilarity
        norms.insert(docId, 1.0 / sqrt((float)terms.size()));
        return docId;
    }
    virtual int getDocFreq(const QString &term) const = 0;


    virtual QString doc(int docId) const { return docs[docId]; }
    virtual void storeDoc(int docId, const QString &docContent) {
        docs.insert(docId, docContent);
    }

    // Utility methods

    float* getIdfs(const QStringList &terms) const
    {
        float *idfs = new float[terms.size()];
        for (int i = 0; i < terms.size(); ++i) {
            idfs[i] = getIdf(terms.at(i));
        }
        return idfs;
    }

    QList<PostingListIterator*> getPostingIterators(const QStringList &terms) const {
        QList<PostingListIterator*> lists;
        foreach(QString term, terms) {
            lists.append(getPostingList(term));
        }
        return lists;
    }

protected:
    virtual void addPosting(const QString &term, int docId) = 0;
private:
    int nextDocId() { return docIdCounter++; }
    int docIdCounter;
    QMap<int, QString> docs;
    QMap<int, float> norms;
};

#endif // INDEX_H
