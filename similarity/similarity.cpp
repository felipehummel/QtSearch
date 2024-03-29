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
