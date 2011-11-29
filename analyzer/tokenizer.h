#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QStringList>

class Tokenizer {
public:
    Tokenizer();
    virtual QStringList tokenize(const QString &docContent) const;
};

#endif // TOKENIZER_H
