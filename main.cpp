#include<iostream>
#include <unordered_map>
#include"json.hpp"

int main() {
    json j;
    j = 5;
    j = "hell";
    json h = {
        {"u", 4},
        {"e", 5},
        {"ee", 55},
    };
    json e = {
        {"u", 4},
        {"e", 5},
        {"ee", 55},
        {"ea", 67},
        {
            "aaa", {
                {"aeeea", 99}, 
                {"aa",
                    {{"aa", 67}}
                }
            }
        }
    };
    json& man = h["u"];
    man = {
        {
            "e", 5
        }
    };
    //std::cout <<  h["u"]["e"].get_int() << std::endl;
    //std::cout <<  e["aaa"]["aa"]["aa"].get_int() << std::endl;
    //std::cout << e;
    h.emplace("los", e);
    //h["e"]["aaa"]["aa"]["aa"] = h;
    //h["e"]["aaa"]["aa"]["aa"]["e"]["aaa"]["aa"]["aa"] = h;
    std::cout <<  h << std::endl;
    return 0;
}
