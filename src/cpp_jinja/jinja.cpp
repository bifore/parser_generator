#include "jinja.h"

#include "util.h"

using namespace std;

namespace jinja
{
    size_t get_pos(int lvl, string text)
    {
        size_t pos = 1;
        for(; pos < text.size() && lvl; ++pos)
        {
            if(text[pos - 1] == '{' && text[pos] == '{')
            {
                ++lvl;
                if(pos - 1 < text.size() && text[pos + 1] == '{')
                    ++pos;
            }
            else if(text[pos - 1] == '}' && text[pos] == '}')
            {
                --lvl;
                if(lvl && pos - 1 < text.size() && text[pos + 1] == '}')
                    ++pos;
            }
        }
        return pos;
    }

    queue<Token*> tokenize_expr(string text)
    {
        queue<Token*> tokens;
        size_t pos = text.find("{{");
        if(pos == string::npos)
            tokens.push(new Token(text, EXPR));
        else
        {
            if(pos)
                tokens.push(new Token(text.substr(0, pos), EXPR));
            text = text.substr(pos + 2);
            pos = get_pos(1, text);
            if(pos != string::npos)
            {
                queue<Token*> sub = tokenize_expr(text.substr(1, pos - 3));
                while(!sub.empty())
                {
                    if(tokens.empty())
                        tokens.push(new Token("", EXPR));
                    tokens.back()->children.push_back(sub.front());
                    sub.pop();
                }
                text = text.substr(pos);
                if(text.size())
                {
                    sub = tokenize_expr(text);
                    while(!sub.empty())
                    {
                        tokens.push(sub.front());
                        sub.pop();
                    }
                }
            }
        }
        return tokens;
    }

    queue<Token*> tokenize(string text)
    {
        queue<Token*> tokens;
        while(!text.empty())
        {
            size_t pos = text.find('{');
            if(pos == string::npos)
            {
                if(!text.empty())
                    tokens.push(new Token(text, TEXT));
                return tokens;
            }
            if(pos)
                tokens.push(new Token(text.substr(0, pos), TEXT));
            text = text.substr(pos + 1);
            if(text.empty())
            {
                tokens.push(new Token("{", TEXT));
                return tokens;
            }
            if(text[0] == '{')
            {
                pos = get_pos(1, text);
                if(pos != string::npos)
                {
                    queue<Token*> sub = tokenize_expr(text.substr(1, pos - 3));
                    Token* expr = new Token("", EXPR);
                    while(!sub.empty())
                    {
                        expr->children.push_back(sub.front());
                        sub.pop();
                    }
                    tokens.push(expr);
                    text = text.substr(pos);
                }
            }
            else if(text[0] == '%')
            {
                pos = text.find("%}");
                if(pos != string::npos)
                {
                    string expr = util::trim(text.substr(1, pos - 2));
                    text = text.substr(pos + 2);
                    if(util::starts_with(expr, "for"))
                        tokens.push(new Token(expr.substr(3), FOR));
                    else if(util::starts_with(expr, "if"))
                        tokens.push(new Token(expr.substr(2), IF));
                    else
                    {
                        expr = util::trim(expr);
                        TokenType type = expr == "endif" ? ENDIF : ENDFOR;
                        tokens.push(new Token("", type));
                    }
                }
            }
            else
                tokens.push(new Token("{", TEXT));
        }
        return tokens;
    }

    vector<Token*> generate_tree(queue<Token*>& tokens, TokenType until)
    {
        vector<Token*> tree;
        while(!tokens.empty())
        {
            Token* token = tokens.front();
            tokens.pop();
            if(token->type == FOR)
                token->children = generate_tree(tokens, ENDFOR);
            else if(token->type == IF)
                token->children = generate_tree(tokens, ENDIF);
            else if(token->type == until)
                return tree;
            tree.push_back(token);
        }
        return tree;
    }

    string parse(string text, Context context)
    {
        queue<Token*> tokens = tokenize(text);
        vector<Token*> tree = generate_tree(tokens, EOS);
        string output;
        for(Token* token : tree)
            output += token->to_text(&context);
        return output;
    }
}
