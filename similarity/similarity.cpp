#include "similarity.h"
#include <math.h>

Similarity::Similarity()
{
}


float Similarity::norm(int docNumberTokens) const
{
    return 1.0 / sqrt((float)docNumberTokens);
}

float Similarity::tf(int termFrequency) const
{
    return (float)termFrequency;
}

float Similarity::idf(int docFrequency, int totalNumDocs, const QString &term) const
{
    Q_UNUSED(term);
    return log((float)(totalNumDocs + 1) / (float)docFrequency); // + 1 to avoid idf = 0
}
