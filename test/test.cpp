#include <iostream>
#include <fstream>
#include <string>

int main(){
    std::ifstream fin("cqvie.jpg", std::ios::binary);
    std::ofstream fout("cqvie2.jpg", std::ios::binary | std::ios::trunc);
    std::string line;
    while(std::getline(fin, line))
        fout << line;
    

    return 0;
}