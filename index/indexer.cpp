#include <QRegExp>

#include "index/indexer.h"

Indexer::Indexer(Index *_index, Tokenizer _tokenizer)
    : index(_index), tokenizer(_tokenizer){}

int Indexer::indexDocument(QString docContent) {
    QStringList docTerms = tokenizer.tokenize(docContent);
    int docId = index->addDocument(docTerms);
    index->storeDoc(docId, docContent);
    return docId;
}

QStringList Tokenizer::tokenize(const QString &docContent) const {
    return Tokenizer::cleanAccents(docContent.toLower()).split(QRegExp("[^\\w0-9]+"), QString::SkipEmptyParts);
}

QString Tokenizer::cleanAccents(const QString &text) {
    QString str(text);
    const char tA[4] = {225,226,227,228}; //á,â,ã,à
    const char tE[2] = {233,234}; //é,ê
    const char tI[1] = {237}; //í
    const char tO[3] = {243,244,245}; //ó,ô,õ
    const char tU[2] = {250,252}; //ú,ü
    const char tC[1] = {231}; //ç
    for (unsigned int j = 0; j < sizeof(tA); j++) str.replace(tA[j],"a");
    for (unsigned int j = 0; j < sizeof(tE); j++) str.replace(tE[j],"e");
    for (unsigned int j = 0; j < sizeof(tI); j++) str.replace(tI[j],"i");
    for (unsigned int j = 0; j < sizeof(tO); j++) str.replace(tO[j],"o");
    for (unsigned int j = 0; j < sizeof(tU); j++) str.replace(tU[j],"u");
    for (unsigned int j = 0; j < sizeof(tC); j++) str.replace(tC[j],"c");
    return str;
}

bool Indexer::indexFileDocPerLine(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        indexDocument(line);
        line = in.readLine();
    }
    return true;
}
