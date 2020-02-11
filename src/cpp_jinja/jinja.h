#ifndef JINJA_H
#define JINJA_H

#include <queue>
#include <string>
#include <vector>

#include "context.h"
#include "token.h"

namespace jinja
{
    std::queue<Token*> tokenize(std::string text);
    std::vector<Token*> generate_tree(std::queue<Token*>& tokens, TokenType until);
    std::string parse(std::string text, Context context);
}

#endif /* ! JINJA_H */
