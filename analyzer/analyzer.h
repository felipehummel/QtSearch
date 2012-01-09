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

#ifndef ANALYZER_H
#define ANALYZER_H

#include <QString>
#include <QSet>
#include <QStringList>
#include "analyzer/tokenizer.h"

class TokenFilter
{
public:
    TokenFilter() {}
    virtual QStringList filter(const QString &token) const = 0;
};


class Analyzer
{
public:
    Analyzer(const Tokenizer &_tokenizer);

    // Basic Filters
    void withLowerCaseFilter();
    void withAccentFilter();
    void withStopWordFilter(const QSet<QString> &stopWords);

    QStringList analyze(const QString &docContent) const;
    void useFilter(TokenFilter *filter);
    ~Analyzer();

private:
    QList<TokenFilter*> filters;
    Tokenizer tokenizer;
};

class StandardAnalyzer : public Analyzer
{
public:
    StandardAnalyzer() : Analyzer(Tokenizer()) {
        withLowerCaseFilter();
        withAccentFilter();
    }
};



class StopWordFilter : public TokenFilter
{
public:
    StopWordFilter(const QSet<QString> &stopWords);
    QStringList filter(const QString &token) const;
private:
    QSet<QString> stopWords;
};

class LowerCaseFilter : public TokenFilter
{
public:
    LowerCaseFilter() {}
    QStringList filter(const QString &token) const;
};

class AccentFilter : public TokenFilter
{
public:
    AccentFilter() {}
    QStringList filter(const QString &token) const;
private:
    QString clean(const QString &text) const;
};



#endif // ANALYZER_H
