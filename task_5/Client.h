//
// Created by lama on 23.02.24.
//

#ifndef TASK_5_2_CLIENT_H
#define TASK_5_2_CLIENT_H


#include <string>
#include <vector>
#include "Phone.h"

struct Client {
private:
    int id;
    std::string first_name;
    std::string last_name;
    std::string email;
    std::vector<Phone> phones;
public:
    Client();
    Client(int id, const std::string &first_name, const std::string &last_name, const std::string &email, const std::vector<Phone> &phones);
    Client(int id, const std::string &first_name, const std::string &last_name, const std::string &email);
    void print() const;
    int get_id() const;
    std::string get_first_name() const;
    std::string get_last_name() const;
    std::string get_email() const;
    std::vector<Phone> get_phones() const;
};




#endif //TASK_5_2_CLIENT_H
