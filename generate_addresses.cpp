#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <bitcoin/bitcoin.hpp>

std::vector<std::pair<std::string, std::string>> generateAddresses(const std::string& startHex, const std::string& endHex) {
    std::vector<std::pair<std::string, std::string>> addresses;
    double startTime = bitcoin::timestamp();
    
    std::ofstream file("addresses.txt");
    for (int i = std::stoi(startHex, 0, 16); i <= std::stoi(endHex, 0, 16); ++i) {
        std::stringstream private_key_hex_stream;
        private_key_hex_stream << std::hex << std::setw(64) << std::setfill('0') << i;
        std::string private_key_hex = private_key_hex_stream.str();
        bc::data_chunk private_key_data = bc::decode_hex(private_key_hex);
        bc::ec_secret private_key(private_key_data.begin(), private_key_data.end());
        
        bc::ec_compressed public_key;
        bc::secret_to_public(public_key, private_key);
        
        std::string address = bc::payment_address(public_key).encoded();
        addresses.push_back(std::make_pair(address, private_key_hex));
        
        double timeRemaining = (bitcoin::timestamp() - startTime) / (i - std::stoi(startHex, 0, 16) + 1) * (std::stoi(endHex, 0, 16) - i);
        std::cout << "Time remaining: " << std::fixed << std::setprecision(2) << timeRemaining << " seconds" << std::endl;
        
        file << "Address: " << address << "\tPrivate Key Hex: " << private_key_hex << std::endl;
        file.flush();
    }
    
    return addresses;
}

int main() {
    std::string startHex, endHex;
    std::cout << "Enter the starting hex value: ";
    std::cin >> startHex;
    std::cout << "Enter the ending hex value: ";
    std::cin >> endHex;
    
    try {
        std::vector<std::pair<std::string, std::string>> addresses = generateAddresses(startHex, endHex);
        std::cout << "Addresses generated successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
    
    return 0;
}
