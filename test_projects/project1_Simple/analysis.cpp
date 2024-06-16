#include <iostream>
#include <fstream>

int main() {
    std::ofstream file("123.txt", std::ios::out);
    
    if (file.is_open()) {
        std::cout << "File 123.txt created successfully." << std::endl;
        file.close();
    } else {
        std::cerr << "Unable to create file 123.txt." << std::endl;
    }
    
    return 0;
}