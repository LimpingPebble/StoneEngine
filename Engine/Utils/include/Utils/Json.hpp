// Copyright 2024 Stone-Engine

#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Stone::Json {

struct Value;

using Object = std::unordered_map<std::string, std::shared_ptr<Value>>;
using Array = std::vector<std::shared_ptr<Value>>;

struct Value {

	std::variant<Object, Array, std::string, double, bool, std::nullptr_t> value;

	explicit Value(Object obj);
	explicit Value(Array arr);
	explicit Value(std::string str);
	explicit Value(double num);
	explicit Value(bool b);
	explicit Value(std::nullptr_t n = nullptr);

	template <typename T>
	bool is() const {
		return std::holds_alternative<T>(value);
	}

	bool isNull() const {
		return std::holds_alternative<std::nullptr_t>(value);
	}

	template <typename T>
	T &get() {
		return std::get<T>(value);
	}

	template <typename T>
	const T &get() const {
		return std::get<T>(value);
	}

	static std::shared_ptr<Value> parseString(const std::string &input);
	static std::shared_ptr<Value> parseFile(const std::string &path);
	std::string serialize() const;
};

std::shared_ptr<Value> object(const Object &obj = {});
std::shared_ptr<Value> array(const Array &arr = {});
std::shared_ptr<Value> string(const std::string &str = "");
std::shared_ptr<Value> number(double num = 0.0);
std::shared_ptr<Value> boolean(bool b = false);
std::shared_ptr<Value> null();


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

class Serializer {

public:
	std::string serialize(const Value &value);

	void operator()(const Object &obj);
	void operator()(const Array &arr);
	void operator()(const std::string &str);
	void operator()(double num);
	void operator()(bool b);
	void operator()(std::nullptr_t);

private:
	std::stringstream _ss;
};

} // namespace Stone::Json
