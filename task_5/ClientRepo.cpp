//
// Created by lama on 23.02.24.
//

#include <iostream>
#include "ClientRepo.h"
#include <pqxx/pqxx>

ClientRepo::ClientRepo(const char *options) {
    this->c = new pqxx::connection(options);
}

ClientRepo::~ClientRepo() {
    if (this->c != nullptr) {
        c->close();
    }
}

bool ClientRepo::clean_tables() {
    if (this->c == nullptr) {
        throw std::runtime_error("no connection!");
    }
    pqxx::work tx{*c};

    std::string clean =
            "DROP TABLE IF EXISTS client_to_phone;"
            "DROP TABLE IF EXISTS phone;"
            "DROP TABLE IF EXISTS client;";

    try {
        tx.exec(clean);
    } catch (pqxx::sql_error &e) {
        std::cout << "error in clean_tables() " << e.what();
        tx.abort();
        return false;
    }
    tx.commit();
    return true;
}

bool ClientRepo::create_tables() {
    if (this->c == nullptr) {
        throw std::runtime_error("no connection!");
    }
    pqxx::work tx{*c};

    std::string phone =
            "CREATE TABLE IF NOT EXISTS phone ("
            "id serial NOT NULL PRIMARY KEY,"
            "phone text NOT NULL UNIQUE"
            ");";

    std::string client =
            "CREATE TABLE IF NOT EXISTS client ("
            "id serial NOT NULL PRIMARY KEY,"
            "first_name text NOT NULL,"
            "last_name text NOT NULL,"
            "email text NOT NULL UNIQUE"
            ");";

    std::string client_to_phone =
            "CREATE TABLE IF NOT EXISTS client_to_phone ("
            "id serial NOT NULL PRIMARY KEY,"
            "client_id integer NOT NULL REFERENCES client (id),"
            "phone_id integer NOT NULL REFERENCES phone (id)"
            ");";

    try {
        tx.exec(phone);
        tx.exec(client);
        tx.exec(client_to_phone);
    } catch (pqxx::sql_error &e) {
        std::cout << "error in create_tables() " << e.what();
        tx.abort();
        return false;
    }
    tx.commit();
    return true;
}

int ClientRepo::insert_client(const Client &client) {
    if (this->c == nullptr) {
        throw std::runtime_error("no connection!");
    }
    pqxx::work tx{*c};

    std::string query =
            "INSERT INTO client (first_name, last_name, email)"
            "VALUES ( '" +
            tx.esc(client.get_first_name()) + "', '" +
            tx.esc(client.get_last_name()) + "', '" +
            tx.esc(client.get_email()) + "' ) RETURNING id;";

    try {
        auto result = tx.exec(query);

        auto row = result.at(0);
        auto field = row.at(0);
        auto opt = field.get<int>();
        if (opt.has_value()) {
            tx.commit();
            return opt.value();
        }
        tx.abort();
        return 0;

    } catch (pqxx::sql_error &e) {
        std::cout << "error in insert_client() " << e.what();
        tx.abort();
        return 0;
    }
}

int ClientRepo::insert_phone(const Phone &phone) {
    if (this->c == nullptr) {
        throw std::runtime_error("no connection!");
    }
    pqxx::work tx{*c};

    std::string query =
            "INSERT INTO phone (phone)"
            "VALUES ( '" + tx.esc(phone.get_phone()) + "' ) RETURNING id;";

    try {
        auto result = tx.exec(query);
        auto row = result.at(0);
        auto field = row.at(0);
        auto opt = field.get<int>();
        if (opt.has_value()) {
            tx.commit();
            return opt.value();
        }
        tx.abort();
        return 0;
    } catch (pqxx::sql_error &e) {
        std::cout << "error in insert_phone() " << e.what();
        tx.abort();
        return 0;
    }
}

int ClientRepo::insert_client_to_phone(int clientId, int phoneId) {
    pqxx::work tx{*c};

    std::string query =
            "INSERT INTO client_to_phone (client_id, phone_id)"
            "VALUES ( " +
            tx.esc(std::to_string(clientId)) + ", " +
            tx.esc(std::to_string(phoneId)) + " ) RETURNING id;";

    try {
        auto result = tx.exec(query);
        auto row = result.at(0);
        auto field = row.at(0);
        auto opt = field.get<int>();
        if (opt.has_value()) {
            tx.commit();
            return opt.value();
        }
        tx.abort();
        return 0;
    } catch (pqxx::sql_error &e) {
        std::cout << "insert_client_to_phone() " << e.what();
        tx.abort();
        return 0;
    }
}

Client ClientRepo::get_client_by_id(int clientId) {
    pqxx::work tx{*c};

    std::string client_query =
            " SELECT id, first_name, last_name, email "
            " FROM client "
            " WHERE id = " + tx.esc(std::to_string(clientId)) + ";";

    auto clientOptional = tx.query01<int, std::string, std::string, std::string>(client_query);
    if (clientOptional.has_value()) {
        auto value = clientOptional.value();
        int id = std::get<0>(value);
        std::string first_name = std::get<1>(value);
        std::string last_name = std::get<2>(value);
        std::string email = std::get<3>(value);
        tx.commit();

        std::vector<Phone> phones = get_client_phones(clientId);

        return {id, first_name, last_name, email, phones};

    } else {
        tx.abort();
        return {};
    }
}

Client ClientRepo::get_client_by_first_name(const std::string &name) {
    pqxx::work tx{*c};

    std::string client_query =
            " SELECT id, first_name, last_name, email "
            " FROM client "
            " WHERE LOWER(first_name) = LOWER('" + tx.esc(name) + "');";


    auto clientOptional = tx.query01<int, std::string, std::string, std::string>(client_query);
    if (clientOptional.has_value()) {
        auto value = clientOptional.value();
        int id = std::get<0>(value);
        std::string first_name = std::get<1>(value);
        std::string last_name = std::get<2>(value);
        std::string email = std::get<3>(value);
        tx.commit();

        std::vector<Phone> phones = get_client_phones(id);

        return {id, first_name, last_name, email, phones};

    } else {
        tx.abort();
        return {};
    }
}

Phone ClientRepo::get_phone_by_id(int phoneId) {
    pqxx::work tx{*c};

    std::string phone_query =
            " SELECT id, phone "
            " FROM phone "
            " WHERE id = " + tx.esc(std::to_string(phoneId)) + ";";

    auto phoneOptional = tx.query01<int, std::string>(phone_query);
    if (phoneOptional.has_value()) {
        auto value = phoneOptional.value();
        int id = std::get<0>(value);
        std::string phone = std::get<1>(value);

        tx.commit();
        return {id, phone};
    } else {
        tx.abort();
        return {};
    }
}

std::vector<Phone> ClientRepo::get_client_phones(int clientId) {
    pqxx::work tx{*c};

    std::string phones_query =
            " SELECT p.id, p.phone "
            " FROM client_to_phone ctp "
            " JOIN phone p ON ctp.phone_id = p.id "
            " WHERE ctp.client_id = " + tx.esc(std::to_string(clientId)) + ";";

    std::vector<Phone> phones;
    for (auto a: tx.query<int, std::string>(phones_query)) {
        phones.emplace_back(std::get<0>(a), std::get<1>(a));
    }
    tx.commit();
    return phones;
}

bool ClientRepo::delete_client_by_id(int id) {
    pqxx::work tx{*c};

    std::string delete_client_by_id_query =
            " DELETE FROM client "
            " WHERE id = " + tx.esc(std::to_string(id)) + ";";

    try {
        tx.exec(delete_client_by_id_query);
        tx.commit();
    } catch (pqxx::sql_error &e) {
        std::cout << "error in delete_client_by_id() " << e.what();
        tx.abort();
        return false;
    }
    return true;
}

bool ClientRepo::delete_phone_by_id(int id) {
    pqxx::work tx{*c};

    std::string delete_phone_by_id_query =
            " DELETE FROM phone "
            " WHERE id = " + tx.esc(std::to_string(id)) + ";";
    try {
        tx.exec(delete_phone_by_id_query);
        tx.commit();
    } catch (pqxx::sql_error &e) {
        std::cout << "error in delete_phone_by_id() " << e.what();
        tx.abort();
        return false;
    }
    return true;
}

bool ClientRepo::delete_client_to_phone_by_client_and_phone_id(int clientId, int phoneId) {
    pqxx::work tx{*c};

    std::string delete_client_to_phone_by_client_and_phone_id_query =
            " DELETE FROM client_to_phone "
            " WHERE client_id = '" +
            tx.esc(std::to_string(clientId)) + "' " +
            " AND phone_id = '" +
            tx.esc(std::to_string(phoneId)) + "';";

    try {
        tx.exec(delete_client_to_phone_by_client_and_phone_id_query);
        tx.commit();
    } catch (pqxx::sql_error &e) {
        std::cout << "error in delete_client_to_phone_by_client_and_phone_id() " << e.what();
        tx.abort();
        return false;
    }
    return true;
}

bool ClientRepo::delete_client_to_phone_by_id(int id) {
    pqxx::work tx{*c};

    std::string delete_client_to_phone_by_id_query =
            " DELETE FROM client_to_phone "
            " WHERE id = '" + tx.esc(std::to_string(id)) + "';";

    try {
        tx.exec(delete_client_to_phone_by_id_query);
        tx.commit();
    } catch (pqxx::sql_error &e) {
        std::cout << "error in delete_client_to_phone_by_id() " << e.what();
        tx.abort();
        return false;
    }
    return true;
}

bool ClientRepo::update_client_name(int clientId, const std::string &name) {
    pqxx::work tx{*c};

    std::string update_client_name_query =
            " UPDATE client "
            " SET first_name = '" +
            tx.esc(name) + "' "
            " WHERE id = '" + tx.esc(std::to_string(clientId)) + "';";

    try {
        tx.exec(update_client_name_query);
        tx.commit();
    } catch (pqxx::sql_error &e) {
        std::cout << "error in update_client_name() " << e.what();
        tx.abort();
        return false;
    }
    return true;
}

