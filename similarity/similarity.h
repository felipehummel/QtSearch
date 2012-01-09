/*******************************************************************************
**
** This file is part of QtSearch **
**
**
** Name: Qt Search
** Version: Beta
** Contact: Felipe da Costa Hummel <felipehummel@gmail.com>
**          Maisa Vidal Teixeira <maisa.vidal@openbossa.org>
**
** Qt Search is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** any later version.
**
** Qt Search is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
*******************************************************************************/

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
