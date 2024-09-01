// Copyright 2024 Stone-Engine

#include "Utils/Json.hpp"

#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Stone::Json {

Value::Value(Object obj) : type(Type::Object), value(std::move(obj)) {
}

Value::Value(Array arr) : type(Type::Array), value(std::move(arr)) {
}

Value::Value(Primitive prim) : type(Type::Primitive), value(std::move(prim)) {
}

Object &Value::asObject() {
	return std::get<Object>(value);
}

const Object &Value::asObject() const {
	return std::get<Object>(value);
}

Array &Value::asArray() {
	return std::get<Array>(value);
}

const Array &Value::asArray() const {
	return std::get<Array>(value);
}

Primitive &Value::asPrimitive() {
	return std::get<Primitive>(value);
}

const Primitive &Value::asPrimitive() const {
	return std::get<Primitive>(value);
}

std::string &Value::asString() {
	return std::get<std::string>(asPrimitive());
}

const std::string &Value::asString() const {
	return std::get<std::string>(asPrimitive());
}

double &Value::asNumber() {
	return std::get<double>(asPrimitive());
}

const double &Value::asNumber() const {
	return std::get<double>(asPrimitive());
}

bool &Value::asBool() {
	return std::get<bool>(asPrimitive());
}

const bool &Value::asBool() const {
	return std::get<bool>(asPrimitive());
}

std::nullptr_t &Value::asNull() {
	return std::get<std::nullptr_t>(asPrimitive());
}

const std::nullptr_t &Value::asNull() const {
	return std::get<std::nullptr_t>(asPrimitive());
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
	Primitive value;
	switch (_currentToken.type) {
	case TokenType::String: value = _currentToken.value; break;
	case TokenType::Number: value = std::stod(_currentToken.value); break;
	case TokenType::True: value = true; break;
	case TokenType::False: value = false; break;
	case TokenType::Null: value = nullptr; break;
	default: throw std::runtime_error("Unexpected token in input");
	}
	auto result = std::make_shared<Value>(value);
	_currentToken = _lexer.nextToken();
	return result;
}

void Parser::_consume(TokenType expected) {
	if (_currentToken.type != expected) {
		throw std::runtime_error("Unexpected token in input");
	}
	_currentToken = _lexer.nextToken();
}


} // namespace Stone::Json
