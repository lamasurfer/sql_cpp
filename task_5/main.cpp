#include <iostream>
#include <pqxx/pqxx>
#include "ClientRepo.h"


int main() {

    ClientRepo db("host=localhost "
                  "port=5432 "
                  "dbname=sqlcpp5 "
                  "user=postgres "
                  "password=postgres");
    bool result = db.clean_tables();
    if (!result) {
        return 1;
    }
    std::cout << "DB is clean\n";

    result = db.create_tables(); // Метод, создающий структуру БД (таблицы).
    if (!result) {
        return 1;
    }
    std::cout << "DB structure is ready\n";

    std::cout << "\n=========================\n";

    // Метод, позволяющий добавить нового клиента.
    Client clientToInsert{0, "Scott", "Stevens", "stevens@mail.de"};

    int insertedClientId = db.insert_client(clientToInsert);
    std::cout << "\nInserted client, id = " << insertedClientId << '\n';

    Client clientById = db.get_client_by_id(insertedClientId);
    clientById.print();

    std::cout << "\n=========================\n";

    // Метод, позволяющий добавить телефон.
    Phone phoneToInsert1{0, "+79163332211"};
    int insertedPhoneId1 = db.insert_phone(phoneToInsert1);
    std::cout << "\nInserted phone, id = " << insertedPhoneId1 << '\n';

    phoneToInsert1 = db.get_phone_by_id(insertedPhoneId1);
    phoneToInsert1.print();

    Phone phoneToInsert2{0, "+79653221122"};
    int insertedPhoneId2 = db.insert_phone(phoneToInsert2);
    std::cout << "\nInserted phone, id = " << insertedPhoneId2 << '\n';

    phoneToInsert2 = db.get_phone_by_id(insertedPhoneId2);
    phoneToInsert2.print();

    std::cout << "\n=========================\n";

    //Метод, позволяющий добавить телефон для существующего клиента.
    int clientToPhoneId1 = db.insert_client_to_phone(insertedClientId, insertedPhoneId1);
    std::cout << "\nLinked client with phone, link id = " << clientToPhoneId1 << '\n';

    int clientToPhoneId2 = db.insert_client_to_phone(insertedClientId, insertedPhoneId2);
    std::cout << "\nLinked client with phone, link id = " << clientToPhoneId2 << '\n';

    clientById = db.get_client_by_id(insertedClientId);
    clientById.print();

    std::cout << "\n=========================\n";

    //Метод, позволяющий удалить телефон у существующего клиента
    bool deleted = db.delete_client_to_phone_by_client_and_phone_id(insertedClientId, insertedPhoneId2);
    std::cout << "\nDeleted client to phone link, result = " << std::boolalpha << deleted << '\n';

    clientById = db.get_client_by_id(insertedClientId);
    clientById.print();

    std::cout << "\n=========================\n";

    // Удаление отдельного телефона, у которого связей нет
    Phone phoneToDelete{0, "+79292221133"};
    int phoneToDeleteId = db.insert_phone(phoneToDelete);
    std::cout << "\nInserted next phone, id = " << phoneToDeleteId << '\n';

    phoneToDelete = db.get_phone_by_id(phoneToDeleteId);
    phoneToDelete.print();

    deleted = db.delete_phone_by_id(phoneToDeleteId);
    std::cout << "\nDeleted phone, result = " << std::boolalpha << deleted << '\n';

    std::cout << "\n=========================\n";

    //Метод, позволяющий найти клиента по его данным — name.
    Client clientByName = db.get_client_by_first_name("Scott");
    clientByName.print();

    std::cout << "\n=========================\n";

    //Метод, позволяющий изменить данные о клиенте
    std::string newName = "Jeff";
    bool update = db.update_client_name(clientByName.get_id(), newName);
    std::cout << "\nUpdated client name, result = " << std::boolalpha << update << '\n';

    clientByName = db.get_client_by_first_name(newName);
    clientByName.print();

    std::cout << "\n=========================\n";

    deleted = db.delete_client_by_id(clientByName.get_id());
    //violates foreign key constraint "client_to_phone_client_id_fkey" on table "client_to_phone"
    std::cout << "\nTrying to delete client, result must be false = " << std::boolalpha << deleted << '\n';

    std::cout << "\n=========================\n";

    //Метод, позволяющий удалить существующего клиента
    Client clientToDelete{0, "To", "Delete", "Client"};
    int clientToDeleteId = db.insert_client(clientToDelete);

    clientToDelete = db.get_client_by_id(clientToDeleteId);
    clientToDelete.print();
    deleted = db.delete_client_by_id(clientToDelete.get_id());
    std::cout << "\nTrying to delete client, result = " << std::boolalpha << deleted << '\n';

    clientToDelete = db.get_client_by_id(clientToDeleteId);
    std::cout << "\nEmpty deleted client:" << '\n';
    clientToDelete.print();

    return 0;
}
