/*
To compile and install this program on Debian, you can use the g++ compiler and make utility. Here are the steps:

1. Install g++ if it's not already installed:
   sudo apt-get install g++

2. Compile the program:
   g++ -o base58 source.cpp

3. The executable 'base58' will be created in the current directory. You can run it with:
   ./base58 --string "your string here"

Note: This program uses the getopt library, which is a part of the GNU C Library (glibc). It should be available on any standard Linux distribution.
*/

#include <string>
#include <vector>
#include <algorithm>
#include <getopt.h>
#include <iostream>

std::string base58_chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

std::string to_base58(std::string input) {
    std::vector<unsigned char> v(input.begin(), input.end());
    std::string result;
    unsigned int carry;
    while (!v.empty() && v.back() == 0) {
        v.pop_back();
    }
    int size = v.size() * 138 / 100 + 1;
    std::vector<unsigned char> b58(size);
    for (auto& big : v) {
        carry = big;
        for (auto it = b58.rbegin(); it != b58.rend(); it++) {
            carry += 256 * (*it);
            *it = carry % 58;
            carry /= 58;
        }
    }
    auto it = std::find_if(b58.begin(), b58.end(), [](unsigned char x) { return x != 0; });
    for (; it != b58.end(); it++) {
        result += base58_chars[*it];
    }
    return result;
}

int main(int argc, char *argv[]) {
    int c;
    std::string input;
    while (1) {
        static struct option long_options[] = {
            {"string", required_argument, 0, 's'},
            {0, 0, 0, 0}
        };
        int option_index = 0;
        c = getopt_long(argc, argv, "s:", long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
            case 's':
                input = optarg;
                break;
            default:
                return 1;
        }
    }
    std::cout << to_base58(input);
    return 0;
}