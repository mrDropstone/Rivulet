#pragma once
#include <initializer_list>
#include <ostream>
#include<stdexcept>
#include<stdint.h>
#include<variant>
#include<unordered_map>
#include<string>
#include<vector>
#include<iostream>
#include<stdint.h>


using JsonInt = int64_t;
using JsonFloat = double;
using JsonString = std::string;
class JsonUndefined {};


class json;

class JsonPtr {
private:
    json* _ptr = nullptr;

public:
    JsonPtr() {}
    JsonPtr(const JsonPtr& other);
    void operator=(const JsonPtr& other);
    JsonPtr(JsonPtr&& other) {
        _ptr = other._ptr;
        other._ptr = nullptr;
    }
    JsonPtr(const json& json_ref);
    void operator=(json& json_ref);
    json& ref();
    ~JsonPtr();
};




class JsonArray {
private:
    std::vector<JsonPtr> _data;

public:
    JsonArray(std::vector<json*>& array);
    int size();
    json& at(int index);
};

class JsonObject {
private:
    std::unordered_map<JsonString, JsonPtr> _data;

public:
    JsonObject() {}
    JsonObject(std::initializer_list<std::pair<const char*, json>> map);
    JsonObject(const std::unordered_map<const char*, json>& map);
    JsonObject(const std::unordered_map<std::string, JsonPtr>& map);
    void emplace(const std::string& key, JsonPtr value) {
        _data.emplace(key, value);
    }
    json& operator[](const char* key);
    void swap(JsonObject& other);
    void operator=(std::unordered_map<const char*, json>& json_object);
    friend std::ostream& operator<<(std::ostream& out, JsonObject json_object);
};


class json {
private:
    std::variant<JsonInt, JsonFloat, JsonString, JsonObject, JsonArray, JsonUndefined> _json_data;

public:
    json() {}

    json(JsonUndefined undefined) {
        _json_data = undefined;
    }
    json(int value) {
        _json_data = value;
    }
    json(const char* value) {
        _json_data = value;
    }
    json(const json& other) {
        _json_data = other._json_data;
    }
    json(std::initializer_list<std::pair<const char*, json>> json_object) {
        _json_data = JsonObject(json_object);
    }

    json& operator=(json& other) {
        _json_data = other._json_data;
        return *this;
    }
    json& operator=(int number) {
        _json_data = (JsonInt)number;
        return *this;
    }
    json& operator=(float number) {
        _json_data = (JsonFloat)number;
        return *this;
    }
    json& operator=(std::string& string) {
        _json_data = string;
        return *this;
    }
    json& operator=(const char* string) {
        _json_data = string;
        return *this;
    }
    void operator=(std::initializer_list<std::pair<const char*, json>> json_object) {
        _json_data = JsonObject(json_object);
    }
    void operator=(const std::unordered_map<const char*, json>& json_object) {
        _json_data = JsonObject(json_object);
    }
    json& operator[](const char* key) {
        if (std::holds_alternative<JsonObject>(_json_data))
            return std::get<JsonObject>(_json_data)[key];
        throw std::runtime_error("Wrong type.");
    }


    std::string get_string();
    int get_int();
    friend std::ostream& operator<<(std::ostream& out, json& json_node);
};



std::ostream& operator<<(std::ostream& out, json& json_node);







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
    _ptr = new json(*other._ptr);
}
json& JsonPtr::ref() {
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
json& JsonObject::operator[](const char* key) {
    return _data[key].ref();
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

std::ostream& operator<<(std::ostream& out, json& json_node) {
    if (std::holds_alternative<JsonInt>(json_node._json_data)) {
        std::cout << std::get<JsonInt>(json_node._json_data);
    }
    if (std::holds_alternative<std::string>(json_node._json_data)) {
        std::cout << std::get<std::string>(json_node._json_data);
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, JsonObject json_object) {
    
}
