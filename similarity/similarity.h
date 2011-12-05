#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <QString>

class Similarity
{
public:
    Similarity();
    virtual float norm(int docNumberTokens) const;
    virtual float tf(int termFrequency) const;
    virtual float idf(int docFrequency, int totalNumDocs, const QString &term) const;
};


class ConstantTFSimilarity : public Similarity
{
public:
    ConstantTFSimilarity() : Similarity() {}

    float tf(int termFrequency) const {
        Q_UNUSED(termFrequency);
        return 1.0;
    }
};



#endif // SIMILARITY_H
