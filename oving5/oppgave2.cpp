#include <iostream>
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>


#include "account.hpp"
#include "account_odb.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;

int main() {
    odb::mysql::database db("test", "", "datacom5");
    {
        account john("john");
        john.balance(50.2);
        account jane("jane");

        odb::transaction t(db.begin());

        db.persist(john);
        db.persist(jane);

        t.commit();
    }

    string waiter;
    cin >> waiter;
    {
        odb::transaction t(db.begin());

        for(account &e : db.query<account>(odb::query<account>::balance > 25)){
            e.balance(25.3);
            db.update(e);
        }

        t.commit();

    }

}