//
// Created by lama on 23.02.24.
//
#include <iostream>
#include "Client.h"

void Client::print() const {
    std::cout << "\nClient: {" << '\n';
    std::cout << "\tid: " << id << '\n';
    std::cout << "\tfirst_name: " << first_name << '\n';
    std::cout << "\tlast_name: " << last_name << '\n';
    std::cout << "\temail: " << email << '\n';
    std::cout << "Phones: \n";
    for (const auto &item: phones) {
        std::cout << "\t" + item.get_phone() << '\n';
    }
    std::cout << "}\n";
}

int Client::get_id() const {
    return this->id;
}

std::string Client::get_first_name() const {
    return this->first_name;
}

std::string Client::get_last_name() const {
    return this->last_name;
}

std::string Client::get_email() const {
    return this->email;
}

std::vector<Phone> Client::get_phones() const {
    return this->phones;
}

Client::Client(int id, const std::string &first_name, const std::string &last_name, const std::string &email,
               const std::vector<Phone> &phones) {
    this->id = id;
    this->first_name = first_name;
    this->last_name = last_name;
    this->email = email;
    for (const auto &item: phones) {
        this->phones.emplace_back(item);
    }
}

Client::Client() {
    this->id = 0;
}

Client::Client(int id, const std::string &first_name, const std::string &last_name, const std::string &email) {
    this->id = id;
    this->first_name = first_name;
    this->last_name = last_name;
    this->email = email;
}
