#pragma once

#include "types.h"

#include <cstdint>
#include <list>
#include <unordered_map>

enum class NumberKind {
  Decimal,
  Float,
  Binary,
  Octal,
  Hexadecimal,
};

enum class TokenKind {
  TEndfile,
  TLBrace,    // {
  TRBrace,    // }
  TLParen,    // (
  TRParen,    // )
  TLBrack,    // [
  TRBrack,    // ]
  TComma,     // ,
  TSemiColon, // ;
  TDot,       // .

  // Arithmetic Operators
  TPlus,       // +
  TMinus,      // -
  TAsterisk,   // *
  TSlash,      // /
  TPercent,    // %
  TMinusMinus, // --
  TPlusPlus,   // ++

  // Bitwise Operators
  TTilde, // ~
  TOr,    // |
  TAnd,   // &
  TXor,   // ^
  TShl,   // <<
  TShr,   // >>

  // Logical Operators
  TOrOr,       // ||
  TAndAnd,     // &&
  TEqualEqual, // ==
  TBangEqual,  // !=
  TLess,       // <
  TLessEqual,  // <=
  TGreat,      // >
  TGreatEqual, // >=
  TBang,       // !

  // Assignment Operators
  TEqual,         // =
  TPlusEqual,     // +=
  TMinusEqual,    // -=
  TAsteriskEqual, // *=
  TSlashEqual,    // /=
  TPercentEqual,  // %=
  TOrEqual,       // |=
  TAndEqual,      // &=
  TXorEqual,      // ^=
  TTildeEqual,    // ~=

  // Keywords
  TIf,       // if
  TElse,     // else
  TLoop,     // loop
  TFunction, // function
  TReturn,   // return
  TYield,    // yield
  TFalse,    // false
  TTrue,     // true
  TNull,     // null
  TVar,      // var
  TFinal,    // final
  TStatic,   // static

  // Literals
  TString,
  TInteger,
  TRational,
  TIdentifier
};

typedef struct TokenData {
  TokenKind kind;
  size_t start;
  size_t length;
} TokenData;

typedef std::unordered_map<String, TokenKind> KeywordMap;
typedef std::list<TokenData> TokenList;

class Lexer {
private:
  const KeywordMap keywords = {
      {"if", TokenKind::TIf},         {"else", TokenKind::TElse},
      {"loop", TokenKind::TLoop},     {"function", TokenKind::TFunction},
      {"return", TokenKind::TReturn}, {"yield", TokenKind::TYield},
      {"false", TokenKind::TFalse},   {"true", TokenKind::TTrue},
      {"null", TokenKind::TNull},     {"var", TokenKind::TVar},
      {"final", TokenKind::TFinal},   {"static", TokenKind::TStatic},
  };
  String source;
  size_t source_length;
  size_t position{0};
  size_t line{0};
  size_t column{0};
  bool isDone{false};
  TokenList tokens;
  bool eof();
  char peek();
  char peek(size_t next);
  void advance();
  void advance(size_t next);
  void parseString(const char term);
  void parseNumber(size_t start, NumberKind kind);

public:
  void lexLine(String line);
  void lex(String source);
  TokenList getTokens();

  Lexer();
  ~Lexer();
};
