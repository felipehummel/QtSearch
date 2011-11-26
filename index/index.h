#ifndef INDEX_H
#define INDEX_H

#include <QString>

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
};

class Index
{
public:
    Index() : docIdCounter(0) {}
    virtual PostingListIterator* getPostingList(const QString &term) const = 0;
    virtual float getIdf(const QString &term) const = 0;
    virtual float getNorm(int docId) const = 0;
    virtual void addDocument(QList<QString> terms) {
        int docId = nextDocId();
        foreach (const QString term, terms) {
            addPosting(term, docId);
        }
    }
    virtual void addPosting(const QString &term, int docId) = 0;
    virtual void calculateNorm(int docId) = 0;
    int nextDocId() { return docIdCounter++; }

private:
    int docIdCounter;

};



#endif // INDEX_H
