#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctype.h>
#include <vector>

enum TokenType {
    Identifier = 0,
    Keyword,
    Op,
    Literal,
    EndOfFile
};

typedef struct {
    TokenType type;
    std::string value;
    std::string filepath;
    int line, colum;
}Token;

std::vector<std::string> keywords = {
    "const",
    "fn",
    "->",
    "start",
    "end",
    "if",
    "else",
    "elif",
    "i32",
    "void",
    "print",
};

void printToken(Token& t) {
    std::string result = "";
    switch (t.type)
    {
    case Identifier:
        result += "identifier";
        break;
    case Keyword:
        result += "keyword";
        break;
    case Op:
        result += "op";
        break;
    case Literal:
        result += "literal";
        break;
    default:
        break;
    }
    result += ": " + t.value;
    std::cout << result << std::endl;
}

typedef struct {
    char* start;
    char* current;
    bool endOfFile;
}Tokenizer;

Token makeToken(Tokenizer& tokenizer, TokenType type, bool change) {
    Token t;
    t.type = type;
    t.value = std::string(tokenizer.start, tokenizer.current);
    
    if (change) {
        tokenizer.start = tokenizer.current;
    }
    else {
        tokenizer.current = tokenizer.start;
    }
    if (*(tokenizer.current + 1) == '\0') {
        tokenizer.endOfFile = true;
    }
    return t;
}

Token tokenNumber(Tokenizer& tokenizer, bool change) {
    while (isdigit(*tokenizer.current)) tokenizer.current += 1;
    return makeToken(tokenizer, TokenType::Literal, change);
}

bool isKeyword(Tokenizer& tokenizer) {
    for (auto& keyword : keywords) {
        std::string token = std::string(tokenizer.start, tokenizer.current);
        if (token == keyword) {
            return true;
        }
    }

    return false;
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

Token tokenWord(Tokenizer& tokenizer, bool change) {
    while (!is_whitespace(*tokenizer.current) && *tokenizer.current != '\0') tokenizer.current += 1;
    if (*tokenizer.current == '0') {
        tokenizer.endOfFile = true;
    }
    if (isKeyword(tokenizer)) {
        return makeToken(tokenizer, TokenType::Keyword, change);
    }

    return makeToken(tokenizer, TokenType::Identifier, change);
}

Token getNextToken(Tokenizer& tokenizer, bool change) {
    while (is_whitespace(*tokenizer.start)) tokenizer.start += 1;
    tokenizer.current = tokenizer.start;
    if (*tokenizer.current == '\0') {
        tokenizer.endOfFile = true;
        return makeToken(tokenizer, TokenType::EndOfFile, change);
    }
    //tokenizer.current += 1;
    switch (*tokenizer.current)
    {
    case '-':
        if (*(tokenizer.current + 1) == '>') {
            return tokenWord(tokenizer, change);
        }
        else {
            tokenizer.current += 1;
            return makeToken(tokenizer, TokenType::Op, change);
        }
    case '+':
    case '*':
    case '/':
    case '=':
        tokenizer.current += 1;
        return makeToken(tokenizer, TokenType::Op, change);
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return tokenNumber(tokenizer, change);
    default:
        if (isalpha(*tokenizer.current)) return tokenWord(tokenizer, change);
    }
}

Tokenizer initTokenizer(const std::string& fileContent) {
    Tokenizer t;
    t.start = (char*)fileContent.data();
    t.current = t.start;
    return t;
}