#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

#include "context.h"

namespace jinja
{
    enum TokenType
    {
        TEXT, EXPR, FOR, ENDFOR, IF, ENDIF, EOS
    };

    struct Token
    {
        std::string token;
        TokenType type;
        std::vector<Token*> children;

        Token(std::string token, TokenType type);
        std::string to_text(Context* context);
    };
}

#endif /* ! TOKEN_H */
