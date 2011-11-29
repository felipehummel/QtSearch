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
