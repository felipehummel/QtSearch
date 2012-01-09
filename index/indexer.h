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

#ifndef INDEXER_H
#define INDEXER_H


#include <QFile>
#include <QTextStream>

#include "index/index.h"
#include "analyzer/analyzer.h"

class Indexer
{
public:
    Indexer(Index *_index, const Analyzer &_analyzer = StandardAnalyzer());
    virtual int indexDocument(QString docContent);
    const Index* getIndex() { return index; }
    bool indexFileDocPerLine(const QString &filePath, const char *codec);
    bool indexDir(const QString &dirPath, const char *codec);

private:
    Index *index;
    Analyzer analyzer;
};
#endif // INDEXER_H
