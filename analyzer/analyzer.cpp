#include "analyzer.h"
#include "tokenstream.h"
#include <QDebug>

Analyzer::Analyzer(const Tokenizer &_tokenizer)
    : tokenizer(_tokenizer)
{}

void Analyzer::withLowerCaseFilter()
{
    filters.append(new LowerCaseFilter());
}

void Analyzer::withAccentFilter()
{
    filters.append(new AccentFilter());
}

void Analyzer::withStopWordFilter(const QSet<QString> &stopWords)
{
    filters.append(new StopWordFilter(stopWords));
}

void Analyzer::useFilter(TokenFilter *filter)
{
    filters.append(filter);
}

QStringList Analyzer::analyze(const QString &docContent) const {
    TokenStream stream(tokenizer.tokenize(docContent));
    QStringList analyzedTokens;
    while (stream.hasNext()) {
        QString token = stream.next();
        foreach (TokenFilter *filter, filters) {
            QStringList result = filter->filter(token);
            if (result.empty()) { // filter deleted token
                token = "";
                break;
            }
            if (result.size() != 1) { // filter added tokens
                stream.insertTokens(result); //insert into stream to be evaluated
            }
            token = result.at(0); //update token
        }
        if (token != "")
            analyzedTokens.append(token);
    }
    return analyzedTokens;
}

Analyzer::~Analyzer() {
    foreach (TokenFilter *filter, filters) {
        delete filter;
    }
}



// TokenFilters

StopWordFilter::StopWordFilter(const QSet<QString> &_stopWords)
    : stopWords(_stopWords)
{
}

QStringList StopWordFilter::filter(const QString &token) const
{
    return stopWords.contains(token) ?  QStringList() : QStringList(token);
}

QStringList LowerCaseFilter::filter(const QString &token) const
{
    return QStringList(token.toLower());
}

QStringList AccentFilter::filter(const QString &token) const
{
    return QStringList(clean(token));
}

//TODO only replaces lowercase
QString AccentFilter::clean(const QString &text) const
{
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
