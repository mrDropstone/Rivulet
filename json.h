#pragma once
#include <initializer_list>
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
    json& operator->();
    ~JsonPtr();
};




class JsonArray {
private:
    std::vector<JsonPtr> _data;

public:
    JsonArray(std::vector<json*>& array);
    int size();
    json& at(int index);
    void print(int indent);
};

class JsonObject {
private:
    std::unordered_map<JsonString, JsonPtr> _data;

public:
    JsonObject() {}
    JsonObject(std::initializer_list<std::pair<const char*, json>> map);
    JsonObject(const std::unordered_map<const char*, json>& map);
    JsonObject(const std::unordered_map<std::string, JsonPtr>& map);
    void emplace(const std::string& key, JsonPtr value);
    json& operator[](const char* key);
    void swap(JsonObject& other);
    void operator=(std::unordered_map<const char*, json>& json_object);
    void print(int indent);
};


class json {
private:
    std::variant<JsonInt, JsonFloat, JsonString, JsonObject, JsonArray, JsonUndefined> _json_data;

private:
    void print(int indent);

public:
    json();
    json(JsonUndefined undefined);
    json(int value);
    json(const char* value);
    json(const json& other);
    json(std::initializer_list<std::pair<const char*, json>> json_object);

    json& operator=(json& other);
    json& operator=(int number);
    json& operator=(float number);
    json& operator=(std::string& string);
    json& operator=(const char* string);
    void operator=(std::initializer_list<std::pair<const char*, json>> json_object);
    void operator=(const std::unordered_map<const char*, json>& json_object);
    json& operator[](const char* key);
    void emplace(const char* key, const json &value);


    std::string get_string();
    int get_int();
    float get_float();
    friend std::ostream& operator<<(std::ostream& out, json& json_node);
    friend void JsonObject::print(int indent);
    friend void JsonArray::print(int indent);
};



