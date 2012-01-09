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

#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

#include <QStringList>

class TokenStream
{
public:
    // TODO - make this receive any 'iterable' string container?
    TokenStream(const QStringList &tokens);

    bool hasNext();
    QString next();
    void insertTokens(const QStringList &tokens, int startFrom = 1);

private:
    QStringList stream;
    int currentPos;
};

#endif // TOKENSTREAM_H
