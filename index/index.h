#ifndef INDEX_H
#define INDEX_H

#include <QString>
#include <QList>

struct Posting {
    int docId;
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
        return getDocFreq(term) / docIdCounter;
    }
    virtual float getNorm(int docId) const = 0;
    virtual int addDocument(QList<QString> terms) {
        int docId = nextDocId();
        foreach (const QString term, terms) {
            addPosting(term, docId);
        }
        return docId;
    }
    virtual void calculateNorm(int docId) = 0;
    virtual int getDocFreq(const QString &term) const = 0;


    virtual QString doc(int docId) { return doc[docId]; }
    virtual void storeDoc(docId, docContent) {
        docs.insert(docId, docContent)
    }

protected:
    virtual void addPosting(const QString &term, int docId) = 0;
private:
    int nextDocId() { return docIdCounter++; }
    int docIdCounter;
    QMap<int, QString> docs;
};



#endif // INDEX_H
