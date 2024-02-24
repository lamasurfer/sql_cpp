//
// Created by lama on 24.02.24.
//
#include <iostream>
#include "Phone.h"

int Phone::get_id() const {
    return this->id;
}

std::string Phone::get_phone() const {
    return this->phone;
}

Phone::Phone() {
    this->id = 0;
}

Phone::Phone(int id, const std::string &phone) {
    this->id = id;
    this->phone = phone;
}

void Phone::print() const {
    std::cout << "\nPhone: {" << '\n';
    std::cout << "\tid: " << this->id << '\n';
    std::cout << "\tphone: " << this->phone << '\n';
    std::cout << "}\n";
}

