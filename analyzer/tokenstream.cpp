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
