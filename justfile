default:
    @just run

build:
    @clang++ main.cpp -std=c++20 -o build/a.out

run:
    @just build
    @./build/a.out
