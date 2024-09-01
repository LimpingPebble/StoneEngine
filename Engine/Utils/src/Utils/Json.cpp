// Copyright 2024 Stone-Engine

#include "Utils/Json.hpp"

#include "Utils/StringExt.hpp"

#include <sstream>


namespace Stone::Json {

Value::Value(Object obj) : value(std::move(obj)) {
}

Value::Value(Array arr) : value(std::move(arr)) {
}

Value::Value(std::string str) : value(std::move(str)) {
}

Value::Value(double num) : value(num) {
}

Value::Value(bool b) : value(b) {
}

Value::Value(std::nullptr_t n) : value(n) {
}

std::shared_ptr<Value> Value::parse(const std::string &input) {
	Parser parser(input);
	return parser.parse();
}

std::string Value::serialize() const {
	Serializer serializer;
	return serializer.serialize(*this);
}

std::shared_ptr<Value> object(const Object &obj) {
	return std::make_shared<Value>(obj);
}
std::shared_ptr<Value> array(const Array &arr) {
	return std::make_shared<Value>(arr);
}
std::shared_ptr<Value> string(const std::string &str) {
	return std::make_shared<Value>(str);
}
std::shared_ptr<Value> number(double num) {
	return std::make_shared<Value>(num);
}
std::shared_ptr<Value> boolean(bool b) {
	return std::make_shared<Value>(b);
}
std::shared_ptr<Value> null() {
	return std::make_shared<Value>();
}


Lexer::Lexer(const std::string &input) : _input(input) {
}

Token Lexer::nextToken() {
	while (_pos < _input.size() && std::isspace(_input[_pos]))
		_pos++;

	if (_pos >= _input.size())
		return {TokenType::EndOfFile, ""};

	char current = _input[_pos];

	switch (current) {
	case '{': _pos++; return {TokenType::LeftBrace, "{"};
	case '}': _pos++; return {TokenType::RightBrace, "}"};
	case '[': _pos++; return {TokenType::LeftBracket, "["};
	case ']': _pos++; return {TokenType::RightBracket, "]"};
	case ':': _pos++; return {TokenType::Colon, ":"};
	case ',': _pos++; return {TokenType::Comma, ","};
	case '"': return _stringToken();
	default: return _otherTokens();
	}
}

Token Lexer::_stringToken() {
	std::stringstream ss;
	_pos++; // Skip the opening quote
	while (_pos < _input.size() && _input[_pos] != '"') {
		if (_input[_pos] == '\\')
			_pos++; // Handle escape sequences
		ss << _input[_pos++];
	}
	_pos++; // Skip the closing quote
	return {TokenType::String, ss.str()};
}

Token Lexer::_otherTokens() {
	if (std::isdigit(_input[_pos]) || _input[_pos] == '-') {
		return _numberToken();
	} else if (_input.substr(_pos, 4) == "true") {
		_pos += 4;
		return {TokenType::True, "true"};
	} else if (_input.substr(_pos, 5) == "false") {
		_pos += 5;
		return {TokenType::False, "false"};
	} else if (_input.substr(_pos, 4) == "null") {
		_pos += 4;
		return {TokenType::Null, "null"};
	} else {
		throw std::runtime_error("Unexpected character in input");
	}
}

Token Lexer::_numberToken() {
	std::stringstream ss;
	if (_input[_pos] == '-')
		ss << _input[_pos++];
	while (_pos < _input.size() && std::isdigit(_input[_pos])) {
		ss << _input[_pos++];
	}
	if (_pos < _input.size() && _input[_pos] == '.') {
		ss << _input[_pos++];
		while (_pos < _input.size() && std::isdigit(_input[_pos])) {
			ss << _input[_pos++];
		}
	}
	return {TokenType::Number, ss.str()};
}


Parser::Parser(const std::string &input) : _lexer(input) {
	_currentToken = _lexer.nextToken();
}

std::shared_ptr<Value> Parser::parse() {
	return _parseValue();
}

std::shared_ptr<Value> Parser::_parseValue() {
	switch (_currentToken.type) {
	case TokenType::LeftBrace: return _parseObject();
	case TokenType::LeftBracket: return _parseArray();
	case TokenType::String:
	case TokenType::Number:
	case TokenType::True:
	case TokenType::False:
	case TokenType::Null: return _parsePrimitive();
	default: throw std::runtime_error("Unexpected token in input");
	}
}

std::shared_ptr<Value> Parser::_parseObject() {
	Object object;
	_consume(TokenType::LeftBrace);
	while (_currentToken.type != TokenType::RightBrace) {
		std::string key = _currentToken.value;
		_consume(TokenType::String);
		_consume(TokenType::Colon);
		object[key] = _parseValue();
		if (_currentToken.type == TokenType::Comma) {
			_consume(TokenType::Comma);
		} else {
			break;
		}
	}
	_consume(TokenType::RightBrace);
	return std::make_shared<Value>(object);
}

std::shared_ptr<Value> Parser::_parseArray() {
	Array array;
	_consume(TokenType::LeftBracket);
	while (_currentToken.type != TokenType::RightBracket) {
		array.push_back(_parseValue());
		if (_currentToken.type == TokenType::Comma) {
			_consume(TokenType::Comma);
		} else {
			break;
		}
	}
	_consume(TokenType::RightBracket);
	return std::make_shared<Value>(array);
}

std::shared_ptr<Value> Parser::_parsePrimitive() {
	std::shared_ptr<Value> value;
	switch (_currentToken.type) {
	case TokenType::String: value = std::make_shared<Value>(_currentToken.value); break;
	case TokenType::Number: value = std::make_shared<Value>(std::stod(_currentToken.value)); break;
	case TokenType::True: value = std::make_shared<Value>(true); break;
	case TokenType::False: value = std::make_shared<Value>(false); break;
	case TokenType::Null: value = std::make_shared<Value>(); break;
	default: throw std::runtime_error("Unexpected token in input");
	}
	_currentToken = _lexer.nextToken();
	return value;
}

void Parser::_consume(TokenType expected) {
	if (_currentToken.type != expected) {
		throw std::runtime_error("Unexpected token in input");
	}
	_currentToken = _lexer.nextToken();
}


std::string Serializer::serialize(const Value &value) {
	std::visit(*this, value.value);
	return _ss.str();
}

void Serializer::operator()(const Object &object) {
	_ss << "{";
	bool first = true;
	for (const auto &pair : object) {
		if (!first)
			_ss << ",";
		_ss << "\"" << pair.first << "\":";
		std::visit(*this, pair.second->value);
		first = false;
	}
	_ss << "}";
}

void Serializer::operator()(const Array &array) {
	_ss << "[";
	bool first = true;
	for (const auto &item : array) {
		if (!first)
			_ss << ",";
		std::visit(*this, item->value);
		first = false;
	}
	_ss << "]";
}

void Serializer::operator()(const std::string &str) {
	_ss << "\"" << escape_string(str) << "\"";
}

void Serializer::operator()(double num) {
	_ss << num;
}

void Serializer::operator()(bool b) {
	_ss << (b ? "true" : "false");
}

void Serializer::operator()(std::nullptr_t) {
	_ss << "null";
}

} // namespace Stone::Json
