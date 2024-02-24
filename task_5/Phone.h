//
// Created by lama on 23.02.24.
//

#ifndef TASK_5_2_PHONE_H
#define TASK_5_2_PHONE_H


#include <string>

struct Phone {
private:
    int id;
    std::string phone;
public:
    Phone();
    Phone(int id, const std::string &phone);
    int get_id() const;
    std::string get_phone() const;
    void print() const;
};


#endif //TASK_5_2_PHONE_H
