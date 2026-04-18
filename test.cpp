#include<fcntl.h>
#include<vector>
#include<unistd.h>
#include<termios.h>
#include<iostream>

class Test {
private:
    struct termios original_termios;
    struct termios current_termios;

public:
    Test() {
        tcgetattr(STDIN_FILENO, &original_termios);
        current_termios = original_termios;
        current_termios.c_lflag &= ~ECHO ;
        current_termios.c_lflag &= ~ICANON;
        current_termios.c_cc[VMIN] = 1;    // Read at least 1 character
        current_termios.c_cc[VTIME] = 0;  
        if(tcsetattr(STDIN_FILENO, TCSANOW, &current_termios) == -1) {
            std::cout << "Error loading configuration." << std::endl; std::exit(1);
        }
    }
    ~Test() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    }
    void get_key() {
        std::vector<char> key_data(256);
        int bytes_read = read(STDIN_FILENO, key_data.data() , 4);
        for (char c : key_data)
            std::cout << c;
    }
};

int main() {
    Test man;
    man.get_key();
    return 0;
}
