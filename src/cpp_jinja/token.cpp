#include "token.h"

#include "util.h"

using namespace std;

namespace jinja
{
    Token::Token(string token, TokenType type)
    {
        if(type != TEXT)
            token = util::trim(token);
        this->token = token;
        this->type = type;
    }

    string expr_to_text(Context* context, Token* token)
    {
        string key = token->token;
        for(Token* child: token->children)
            key += expr_to_text(context, child);
        return token->token.size() ? context->get(key)->get() : key;
    }

    string for_to_text(Context* context, Token* token)
    {
        vector<string> vec = util::split(token->token, " ");
        vector<Context*> array = context->get(vec[2])->get_array();
        string output;
        Context* loop = new Context();
        context->set("loop", loop);
        int i = 0;
        int revi = array.size();
        for(Context* value : array)
        {
            loop->set("index", to_string(i++));
            loop->set("revindex", to_string(--revi));
            context->set(vec[0], value);
            for(Token* child : token->children)
                output += child->to_text(context);
        }
        context->erase(vec[0]);
        context->erase("loop");
        return output;
    }

    bool is_true(Context* context, Token* token)
    {
        vector<string> vec = util::split(token->token, " ");
        string left = vec[0][0] == '"'
            ? vec[0].substr(1, vec[0].size() - 2)
            : context->get(vec[0])->get();
        string right = vec[2][0] == '"'
            ? vec[2].substr(1, vec[2].size() - 2)
            : context->get(vec[2])->get();
        if(vec[1] == "==")
            return left == right;
        if(vec[1] == "!=")
            return left != right;
        return false;
    }

    string if_to_text(Context* context, Token* token)
    {
        string output;
        if(is_true(context, token))
            for(Token* child : token->children)
                output += child->to_text(context);
        return output;
    }

    string Token::to_text(Context* context)
    {
        switch(type)
        {
            case TEXT:
                return token;
            case EXPR:
                return expr_to_text(context, this);
            case FOR:
                return for_to_text(context, this);
            case IF:
                return if_to_text(context, this);
            default:
                return token;
        }
    }
}
