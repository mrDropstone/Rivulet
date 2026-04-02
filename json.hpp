#pragma once
#include <climits>
#include <initializer_list>
#include<stdexcept>
#include<stdint.h>
#include"utils.hpp"
#include<variant>
#include"json.h"


JsonPtr::JsonPtr(const json& json_ref) {
    _ptr = new json(json_ref);
}
void JsonPtr::operator=(json& json_ref) {
    delete _ptr;
    _ptr = new json(json_ref);
}
JsonPtr::JsonPtr(const JsonPtr& other) {
    _ptr = new json(*other._ptr);
}
void JsonPtr::operator=(const JsonPtr& other) {
    delete _ptr;
    _ptr = new json(*other._ptr);
}
json& JsonPtr::ref() {
    return *_ptr;
}
json& JsonPtr::operator->() {
    return *_ptr;
}
JsonPtr::~JsonPtr() {
    delete _ptr;
    _ptr = nullptr;
}



JsonArray::JsonArray(std::vector<json*>& array) {
    for (json* node : array)
        _data.emplace_back(*node);
}
int JsonArray::size() {
    return _data.size();
}
json& JsonArray::at(int index) {
    return _data[index].ref();
}
void JsonArray::print(int indent) {
    for (JsonPtr json_ptr : _data) {
        json_ptr.ref().print(indent);
    }
}


JsonObject::JsonObject(std::initializer_list<std::pair<const char*, json>> map) {
    _data.clear();
    for (auto [key, data]: map) {
        _data.emplace(JsonString(key), JsonPtr(std::move(data)));
    }
}
JsonObject::JsonObject(const std::unordered_map<const char*, json>& map) {
    _data.clear();
    for (auto [key, data]: map) {
        _data.emplace(JsonString(key), JsonPtr(data));
    }
}
void JsonObject::swap(JsonObject& other) {
    other._data.swap(_data);
}
void JsonObject::emplace(const std::string& key, JsonPtr value) {
    _data.emplace(key, value);
}
json& JsonObject::operator[](const char* key) {
    return _data[key].ref();
}
void JsonObject::print(int indent) {
    std::cout << "{\n";
    indent += 4;
    int index = 0;
    for (auto [key, value] : _data) {
        index++;
        print_space(indent);
        std::cout << '"' << key << "\" : ";
        value.ref().print(indent);
        if (index < _data.size())
            std::cout << ",\n";
        else
            std::cout << '\n';
    }
    indent -= 4;
    print_space(indent);
    std::cout << "}";
}



json::json(JsonUndefined undefined) {
    _json_data = undefined;
}
json::json() {
    _json_data = JsonUndefined();
}
json::json(int value) {
    _json_data = value;
}
json::json(const char* value) {
    _json_data = value;
}
json::json(const json& other) {
    _json_data = other._json_data;
}
json::json(std::initializer_list<std::pair<const char*, json>> json_object) {
    _json_data = JsonObject(json_object);
}

json& json::operator=(json& other) {
    _json_data = other._json_data;
    return *this;
}
json& json::operator=(int number) {
    _json_data = (JsonInt)number;
    return *this;
}
json& json::operator=(float number) {
    _json_data = (JsonFloat)number;
    return *this;
}
json& json::operator=(std::string& string) {
    _json_data = string;
    return *this;
}
json& json::operator=(const char* string) {
    _json_data = string;
    return *this;
}
void json::operator=(std::initializer_list<std::pair<const char*, json>> json_object) {
    _json_data = JsonObject(json_object);
}
void json::operator=(const std::unordered_map<const char*, json>& json_object) {
    _json_data = JsonObject(json_object);
}


json& json::operator[](const char* key) {
    if (std::holds_alternative<JsonObject>(_json_data))
        return std::get<JsonObject>(_json_data)[key];
    throw std::runtime_error("Wrong type.");
}


std::string json::get_string() {
    if (std::holds_alternative<JsonString>(_json_data))
        return std::get<JsonString>(_json_data);
    throw std::runtime_error("Wrong type.");
}
int json::get_int() {
    if (std::holds_alternative<JsonInt>(_json_data))
        return std::get<JsonInt>(_json_data);
    throw std::runtime_error("Wrong type.");
}
float json::get_float() {
    if (std::holds_alternative<JsonFloat>(_json_data))
        return std::get<JsonFloat>(_json_data);
    throw std::runtime_error("Wrong type.");
}
void json::emplace(const char* key, const json &value) {
    if (std::holds_alternative<JsonObject>(_json_data)) {
        std::get<JsonObject>(_json_data).emplace(key, value);
        return;
    }
    throw std::runtime_error("Wrong type.");
}
void json::print(int indent) {
    if (std::holds_alternative<JsonInt>(_json_data))
        std::cout << std::get<JsonInt>(_json_data);
    else if (std::holds_alternative<JsonString>(_json_data))
        std::cout << std::get<JsonString>(_json_data);
    else if (std::holds_alternative<JsonFloat>(_json_data))
        std::cout << std::get<JsonFloat>(_json_data);
    else if (std::holds_alternative<JsonObject>(_json_data))
        std::get<JsonObject>(_json_data).print(indent);
}


std::ostream& operator<<(std::ostream& out, json& json_node) {
    json_node.print(0);
    return out;
}

