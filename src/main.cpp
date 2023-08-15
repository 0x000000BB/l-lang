#include "Tokenizer.h"

#include <memory>

enum VaribleType {
    U32 = 0,
    U64,
    I32,
    I64,
    VOID,
};

VaribleType tokenToType(const Token& t) {
    if (t.value == "u32") {
        return VaribleType::U32;
    }
    else if (t.value == "u64") {
        return VaribleType::U64;
    }
    else if (t.value == "i32") {
        return VaribleType::I32;
    }
    else if (t.value == "i64") {
        return VaribleType::I64;
    }
    else if (t.value == "void") {
        return VaribleType::VOID;
    }
    else {
        std::cout << "ERROR: " << t.value << " is not a valid tpye" << std::endl;
        exit(-1);
    }
}

bool isVaribleType(Token& t) {
    std::string type = t.value;
    if (t.value == "u32") {
        return true;
    }
    else if (t.value == "u64") {
        return true;
    }
    else if (t.value == "i32") {
        return true;
    }
    else if (t.value == "i64") {
        return true;
    }
    else if (t.value == "void") {
        return true;
    }
    return false;
}

class ASTNode {
public:
    virtual void generate(std::ofstream& out) {

    };
};

class VaribleDefinition : public ASTNode {
public:

    void generate(std::ofstream& out) override {
        
        std::cout << "ERROR: not implemented VaribleDefinition" << std::endl;
        exit(-1);
    }

public:
    bool isConst;
    Token name;
    VaribleType type;
    ASTNode* defaultValue;
};

class Parameter : public ASTNode {
public:
    void generate(std::ofstream& out) override {
        std::cout << "ERROR: not implemented Parameter" << std::endl;
        exit(-1);
    }

public:
    Token name;
    VaribleType tpye;
};

class FunctionDefinition : public ASTNode {
public:
    void generate(std::ofstream& out) override {
        std::cout << "ERROR: not implemented FunctionDefinition" << std::endl;
        exit(-1);
    }

public:
    Token name;
    std::vector<Parameter> parameters;
    VaribleType returnType;
    std::vector<ASTNode*> content;
};

class LiteralNode : public ASTNode {
public:
    void generate(std::ofstream& out) override {
        std::cout << "ERROR: not implemented LiteralNode" << std::endl;
        exit(-1);
    }
public:
    Token value;
};

class Plus : public ASTNode {
public:
    void generate(std::ofstream& out) override {
        std::cout << "ERROR: not implemented Plus" << std::endl;
        exit(-1);
    }
public:
    ASTNode* left;
    ASTNode* right;
};

class Print : public ASTNode {
public:
    void generate(std::ofstream& out) override {
        std::cout << "ERROR: not implemented Print" << std::endl;
        exit(-1);
    }
public:
    ASTNode* parameter;
};

class IdentifierNode : public ASTNode {
public:
    void generate(std::ofstream& out) override {
        std::cout << "ERROR: not implemented IdentifierNode" << std::endl;
        exit(-1);
    }
public:
    Token value;
};

class ASTTree {
public:
    void init(Tokenizer& tokenizer) {
        while (!tokenizer.endOfFile) {
            nodes.push_back(evaluateExpression(tokenizer));
        }
    }
    void generate(std::ofstream& out) {
        for(ASTNode* node : nodes){
            node->generate(out);
        }
    }

private:
    ASTNode* parseOperation(ASTNode* left, Token& op, Tokenizer& tokenizer) {
        if (op.value == "+") {
            Plus* plus = new Plus();
            plus->left = left;
            Token next = getNextToken(tokenizer, false);
            if (next.type == TokenType::Identifier) {
                IdentifierNode* identifier = new IdentifierNode();
                identifier->value = getNextToken(tokenizer, true);
                plus->right = identifier;
            }
            return plus;
        }
    }
    ASTNode* evaluateExpression(Tokenizer& tokenizer) {
        Token token = getNextToken(tokenizer, true);
        Token nextToken;
        printToken(token);
        switch (token.type)
        {
        case TokenType::Keyword:
            if (token.value == "const") {
                VaribleDefinition* vDefinition = new VaribleDefinition();
                vDefinition->isConst = true;
                vDefinition->name = getNextToken(tokenizer, true);
                vDefinition->type = tokenToType(getNextToken(tokenizer, true));
                if (getNextToken(tokenizer, true).value == "=") {
                    vDefinition->defaultValue = evaluateExpression(tokenizer);
                }
                else {
                    std::cout << "ERROR: varible must have default value" << std::endl;
                    exit(-1);
                }
                return vDefinition;
            }
            if (token.value == "fn") {
                FunctionDefinition* fDevinition = new FunctionDefinition();
                fDevinition->name = getNextToken(tokenizer, true);
                if (getNextToken(tokenizer, true).value != "->") {
                    fDevinition->parameters;
                }
                fDevinition->returnType = tokenToType(getNextToken(tokenizer, true));
                nextToken = getNextToken(tokenizer, false);
                while (nextToken.value != "end") {
                    if (nextToken.type != TokenType::EndOfFile) {
                         fDevinition->content.emplace_back(evaluateExpression(tokenizer));
                    }
                    nextToken = getNextToken(tokenizer, false);
                }
                std::cout << getNextToken(tokenizer, true).value << std::endl;
                std::cout << getNextToken(tokenizer, true).value << std::endl;
                return fDevinition;
            }
            if (token.value == "print") {
                Print* print = new Print();
                print->parameter = evaluateExpression(tokenizer);
                return print;
            }
            break;
        case TokenType::Literal:
            nextToken = getNextToken(tokenizer, true);
            if (nextToken.value == "end") {
                LiteralNode* literal = new LiteralNode();
                literal->value = token;
                return literal;
            }
            else if (nextToken.type == TokenType::Op) {
                LiteralNode* literal = new LiteralNode();
                literal->value = token;
                return parseOperation(literal, nextToken, tokenizer);
            }
            break;
        case TokenType::Identifier:
            nextToken = getNextToken(tokenizer, true);
            if (isVaribleType(nextToken)) {
                VaribleDefinition* vDefinition = new VaribleDefinition();
                vDefinition->isConst = false;
                vDefinition->name = token;
                vDefinition->type = tokenToType(nextToken);
                if (getNextToken(tokenizer, true).value == "=") {
                    vDefinition->defaultValue = evaluateExpression(tokenizer);
                }
                else {
                    std::cout << "ERROR: varible must have default value" << std::endl;
                    exit(-1);
                }
                return vDefinition;
            }
            else if (nextToken.type == TokenType::Op) {
                IdentifierNode* identifier = new IdentifierNode();
                identifier->value = token;
                return parseOperation(identifier, nextToken, tokenizer);
            }
        default:
            std::cout << "ERROR: not implementet" << std::endl;
            exit(-1);
            break;
        }
        std::cout << "ERROR: not implementet" << std::endl;
        exit(-1);
    }

private:
    std::vector<ASTNode*> nodes;
    std::vector<std::string> varibleNames;
    std::vector<std::string> functionNames;
};

int main() {
    std::ifstream f("tests/main.l");
    std::stringstream buffer;
    buffer << f.rdbuf();
    std::string fileContent = buffer.str();
    fileContent += '\0';

    //std::cout << buffer.str() << std::endl;

    Tokenizer tokenizer = initTokenizer(fileContent);
    ASTTree tree;
    tree.init(tokenizer);
    std::ofstream o("tests/out.asm");
    tree.generate(o);
    return 0;
}