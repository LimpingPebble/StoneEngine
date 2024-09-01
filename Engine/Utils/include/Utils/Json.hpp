// Copyright 2024 Stone-Engine

#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Stone::Json {

struct Value;

using Object = std::unordered_map<std::string, std::shared_ptr<Value>>;
using Array = std::vector<std::shared_ptr<Value>>;
using Primitive = std::variant<std::string, double, bool, std::nullptr_t>;

struct Value {
	enum class Type {
		Object,
		Array,
		Primitive
	};

	Type type;
	std::variant<Object, Array, Primitive> value;

	explicit Value(Object obj);
	explicit Value(Array arr);
	explicit Value(Primitive prim);

	Object &asObject();
	const Object &asObject() const;
	Array &asArray();
	const Array &asArray() const;
	Primitive &asPrimitive();
	const Primitive &asPrimitive() const;
	std::string &asString();
	const std::string &asString() const;
	double &asNumber();
	const double &asNumber() const;
	bool &asBool();
	const bool &asBool() const;
	std::nullptr_t &asNull();
	const std::nullptr_t &asNull() const;
};

enum class TokenType {
	LeftBrace,
	RightBrace,
	LeftBracket,
	RightBracket,
	Comma,
	Colon,
	String,
	Number,
	True,
	False,
	Null,
	EndOfFile
};

struct Token {
	TokenType type;
	std::string value;
};

class Lexer {
public:
	explicit Lexer(const std::string &input);

	Token nextToken();

private:
	const std::string &_input;
	std::size_t _pos = 0;

	Token _stringToken();
	Token _otherTokens();
	Token _numberToken();
};

class Parser {
public:
	explicit Parser(const std::string &input);

	std::shared_ptr<Value> parse();

private:
	Lexer _lexer;
	Token _currentToken;

	std::shared_ptr<Value> _parseValue();
	std::shared_ptr<Value> _parseObject();
	std::shared_ptr<Value> _parseArray();
	std::shared_ptr<Value> _parsePrimitive();
	void _consume(TokenType expected);
};

} // namespace Stone::Json
