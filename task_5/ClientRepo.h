//
// Created by lama on 23.02.24.
//

#ifndef TASK_5_2_CLIENTREPO_H
#define TASK_5_2_CLIENTREPO_H


#include <tuple>
#include "pqxx/transaction"
#include "Phone.h"
#include "Client.h"

class ClientRepo {
public:
    explicit ClientRepo(const char options[]);

    ~ClientRepo();

    bool clean_tables();

    bool create_tables();

    int insert_client(const Client &client);

    int insert_phone(const Phone &phone);

    int insert_client_to_phone(int clientId, int phoneId);

    Phone get_phone_by_id(int phoneId);

    Client get_client_by_id(int clientId);

    bool update_client_name(int clientId, const std::string &name);

    Client get_client_by_first_name(const std::string &name);

    std::vector<Phone> get_client_phones(int clientId);

    bool delete_client_by_id(int id);

    bool delete_phone_by_id(int id);

    bool delete_client_to_phone_by_client_and_phone_id(int clientId, int phone_id);

    bool delete_client_to_phone_by_id(int id);


private:
    pqxx::connection *c = nullptr;
};


#endif //TASK_5_2_CLIENTREPO_H
