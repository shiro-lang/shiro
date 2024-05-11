#include "lexer.h"

bool isLower(const char c) { return 96 < c && c < 123; }

bool isUpper(const char c) { return 64 < c && c < 91; }

bool isNum(const char c) { return 47 < c && c < 58; }

bool isBinary(const char c) { return 47 < c && c < 50; }

bool isOctal(const char c) { return 47 < c && c < 50; }

bool isHex(const char c) {
  return isNum(c) || (96 < c && c < 103) || (64 < c && c < 71);
}

bool isID(const char c) { return c == '$' || c == '_' || isalpha(c); }

Lexer::Lexer() {}

bool Lexer::eof() { return this->source_length <= this->position; }

char Lexer::peek() { return peek(0); }

char Lexer::peek(size_t next) {
  if (this->position + next < this->source_length) {
    return this->source[this->position + next];
  }
  return 0;
}

void Lexer::advance() { advance(1); }

void Lexer::advance(size_t next) { this->position += next; }

void Lexer::parseNumber(size_t start, NumberKind kind) {
  switch (kind) {
  case NumberKind::Decimal: {
    while (isNum(peek(0)) || peek(0) == '.') {
      advance(1);
      if (peek(0) == '.') {
        advance(1);
        parseNumber(start, NumberKind::Float);
        break;
      }
    }
  } break;
  case NumberKind::Float: {
    while (isNum(peek(0))) {
      advance(1);
    }
  } break;
  case NumberKind::Binary: {
    while (isBinary(peek(0))) {
      advance(1);
    }
  } break;
  case NumberKind::Octal: {
    while (isOctal(peek(0))) {
      advance(1);
    }
  } break;
  case NumberKind::Hexadecimal: {
    while (isHex(peek(0))) {
      advance(1);
    }
  } break;
  default:
    break;
  }
  if (kind == NumberKind::Float) {
    this->tokens.push_back(
        {TokenKind::TRational, start + 1, this->position - start - 1});
  } else {
    this->tokens.push_back(
        {TokenKind::TInteger, start + 1, this->position - start - 1});
  }
}

void Lexer::parseString(const char term) {
  size_t start = this->position;
  while (peek() != term) {
    advance(1);
  }
  tokens.push_back({TokenKind::TString, start, this->position - start});
  advance(1);
  return;
}

void Lexer::lexLine(String line) {
  bool endOfLine = false;
  while (!endOfLine) {
    switch (peek()) {
    case '\n': {
      endOfLine = true;
      break;
    }
    case '{': {
      this->tokens.push_back({TokenKind::TLBrace, this->position, 1});
      advance(1);
      break;
    }
    case '}': {
      this->tokens.push_back({TokenKind::TRBrace, this->position, 1});
      advance(1);
      break;
    }

    case '(': {
      this->tokens.push_back({TokenKind::TLParen, this->position, 1});
      advance(1);
      break;
    }
    case ')': {
      this->tokens.push_back({TokenKind::TRParen, this->position, 1});
      advance(1);
      break;
    }

    case '[': {
      this->tokens.push_back({TokenKind::TLBrack, this->position, 1});
      advance(1);
      break;
    }
    case ']': {
      this->tokens.push_back({TokenKind::TRBrack, this->position, 1});
      advance(1);
      break;
    }

    case ',': {
      this->tokens.push_back({TokenKind::TComma, this->position, 1});
      advance(1);
      break;
    }

    case ';': {
      this->tokens.push_back({TokenKind::TSemiColon, this->position, 1});
      advance(1);
      break;
    }

    case '.': {
      this->tokens.push_back({TokenKind::TDot, this->position, 1});
      advance(1);
      break;
    }

    case '+': {
      const char next = peek(1);
      if (next == '+') {
        this->tokens.push_back({TokenKind::TPlusPlus, this->position, 2});
        advance(2);
        break;
      } else if (next == '=') {
        this->tokens.push_back({TokenKind::TPlusEqual, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TPlus, this->position, 1});
      advance(1);
      break;
    }
    case '-': {
      const char next = peek(1);
      if (next == '-') {
        this->tokens.push_back({TokenKind::TMinusMinus, this->position, 2});
        advance(2);
        break;
      } else if (next == '=') {
        this->tokens.push_back({TokenKind::TMinusEqual, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TMinus, this->position, 1});
      advance(1);
      break;
    }
    case '*': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TAsteriskEqual, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TAsterisk, this->position, 1});
      advance(1);
      break;
    }
    case '/': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TSlashEqual, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TSlash, this->position, 1});
      advance(1);
      break;
    }
    case '%': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TPercentEqual, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TPercent, this->position, 1});
      advance(1);
      break;
    }
    case '|': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TOrEqual, this->position, 2});
        advance(2);
        break;
      } else if (peek(1) == '|') {
        this->tokens.push_back({TokenKind::TOrOr, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TOr, this->position, 1});
      advance(1);
      break;
    }
    case '&': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TAndEqual, this->position, 2});
        advance(2);
        break;
      } else if (peek(1) == '&') {
        this->tokens.push_back({TokenKind::TAndAnd, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TAnd, this->position, 1});
      advance(1);
      break;
    }
    case '~': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TTildeEqual, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TTilde, this->position, 1});
      advance(1);
      break;
    }
    case '^': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TXorEqual, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TXor, this->position, 1});
      advance(1);
      break;
    }
    case '<': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TLessEqual, this->position, 2});
        advance(2);
        break;
      } else if (peek(1) == '<') {
        this->tokens.push_back({TokenKind::TShl, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TLess, this->position, 1});
      advance(1);
      break;
    }
    case '>': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TGreatEqual, this->position, 2});
        advance(2);
        break;
      } else if (peek(1) == '>') {
        this->tokens.push_back({TokenKind::TShr, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TGreat, this->position, 1});
      advance(1);
      break;
    }
    case '=': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TEqualEqual, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TEqual, this->position, 1});
      advance(1);
      break;
    }
    case '!': {
      if (peek(1) == '=') {
        this->tokens.push_back({TokenKind::TBangEqual, this->position, 2});
        advance(2);
        break;
      }
      this->tokens.push_back({TokenKind::TBang, this->position, 1});
      advance(1);
      break;
    }
    case '"': {
      advance();
      parseString('"');
      break;
    }
    case '`': {
      advance();
      parseString('`');
      break;
    }
    case '\'': {
      advance();
      parseString('\'');
      break;
    }
    default:
      char c = peek();
      if (-1 < c && c < 33) {
        advance(1);
        break;
      }
      // Number Parsing
      {
        size_t start = this->position;
        NumberKind kind = NumberKind::Decimal;
        bool num = false;
        if (peek(0) == '0') {
          const char next = peek(1);
          if (next == 'x') {
            advance(2);
            kind = NumberKind::Hexadecimal;
          } else if (next == 'o') {
            advance(2);
            kind = NumberKind::Octal;
          } else if (next == 'b') {
            advance(2);
            kind = NumberKind::Binary;
          }
          num = true;
        } else if (isNum(peek(0))) {
          num = true;
        }

        if (num) {
          parseNumber(start, kind);
        }
      }
      // Keywords and indetifier
      {
        size_t start = this->position;
        while (isID(peek(0))) {
          advance();
        }

        if (this->position - start == 0) {
          continue;
        }
        size_t length = this->position - start;
        std::string string = source.substr(start, length);
        if (keywords.find(string) == keywords.end()) {
          tokens.push_back({TokenKind::TIdentifier, start, length});
          break;
        }
        tokens.push_back({keywords.at(string), start, length});
      }
      break;
    }
  }
}

void Lexer::lex(String source) {
  this->source = source;
  this->source_length = this->source.length();
}

Lexer::~Lexer() {}
