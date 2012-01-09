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

#include "tokenstream.h"
#include <QDebug>

TokenStream::TokenStream(const QStringList &tokens)
    : stream(tokens), currentPos(0)
{
}

bool TokenStream::hasNext()
{
    return currentPos < stream.size();
}

QString TokenStream::next()
{
    return stream.at(currentPos++);
}


void TokenStream::insertTokens(const QStringList &tokens, int startFrom) {
    int j = currentPos+1;
    for (int i = startFrom; i < tokens.size(); ++i, ++j) {
        stream.insert(j, tokens.at(startFrom));
    }
}
