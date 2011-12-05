#ifndef INDEX_H
#define INDEX_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QDebug>
#include <math.h>
#include <similarity/similarity.h>


struct Posting {
    int docId;
    int tf; // Term Frequency
    Posting(int _docId, int _tf) : docId(_docId), tf(_tf) {}
    Posting() : docId(-1), tf(-1) {}
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
    virtual Posting current() = 0;
    virtual bool jumpTo(int docId) = 0;
    QList<Posting> toList() {
        // TODO - do not exaust the iterator (or reset it)
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
    Index(Similarity _similarity = Similarity()) : similarity(_similarity), docIdCounter(0) {}

    virtual PostingListIterator* getPostingList(const QString &term) const = 0;
    float getIdf(const QString &term) const {
        return similarity.idf(getDocFreq(term), docIdCounter, term);
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
        norms.insert(docId, similarity.norm(terms.size()));
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
    const Similarity similarity;
    virtual void addPosting(const QString &term, int docId) = 0;
private:
    int nextDocId() { return docIdCounter++; }
    int docIdCounter;
    QMap<int, QString> docs;
    QMap<int, float> norms;
};

#endif // INDEX_H
