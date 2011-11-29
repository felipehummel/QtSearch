#include "tokenizer.h"

Tokenizer::Tokenizer()
{
}

QStringList Tokenizer::tokenize(const QString &docContent) const {
    return docContent.split(QRegExp("[^\\w0-9]+"), QString::SkipEmptyParts);
}

