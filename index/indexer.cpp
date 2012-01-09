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

#include <QRegExp>
#include <QDir>

#include "index/indexer.h"
#include "analyzer/tokenstream.h"

Indexer::Indexer(Index *_index, const Analyzer &_analyzer)
    : index(_index), analyzer(_analyzer){}

int Indexer::indexDocument(QString docContent) {
    QStringList docTerms = analyzer.analyze(docContent);
    int docId = index->addDocument(docTerms);
    index->storeDoc(docId, docContent);
    return docId;
}

bool Indexer::indexFileDocPerLine(const QString &filePath, const char *codec)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    in.setCodec(codec);
    QString line = in.readLine();
    while (!line.isNull()) {
        indexDocument(line);
        line = in.readLine();
    }
    return true;
}

bool Indexer::indexDir(const QString &dirPath, const char *codec)
{
    QDir dir = QDir(dirPath);

    if (!dir.exists()) {
        qDebug() << "Directory does not exist.";
        return false;
    }

    QList<QFileInfo> entryInfoList;
    entryInfoList = dir.entryInfoList();

    for (int i = 0; i < entryInfoList.size(); i++) {

        if (entryInfoList.at(i).baseName() == "")
            continue;

        QFile file(entryInfoList.at(i).absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            indexDir(entryInfoList.at(i).absoluteFilePath(), "UTF-8");
            continue;
        }

        QTextStream in(&file);
        in.setCodec(codec);
        QString text = in.readAll();
        indexDocument(text);
        file.close();
    }

    return true;
}
