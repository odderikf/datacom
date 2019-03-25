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

    odb::transaction t(db.begin());

    odb::result<account> r = db.query<account>(odb::query<account>::name == "john");
    cout << r.size() << " results found for john" << endl;
    account john = *r.begin();
    odb::result<account> r2 = db.query<account>(odb::query<account>::name == "jane");
    cout << r2.size() << " results found for jane" << endl;
    account jane = *r2.begin();

    t.commit();

    string waiter;
    cout << "waiting";
    cin >> waiter;

    odb::transaction t2(db.begin());

    cout << "changing john" << endl;
    john.balance(john.balance() + 413.612);
    db.update(john);

    t2.commit();

    cout << "waiting";
    cin >> waiter;

    try {
        odb::transaction t2(db.begin());

        cout << "changing jane";
        jane.balance(jane.balance() - 413.612);
        db.update(jane);

        t2.commit();
    } catch (odb::object_changed &e) {
        odb::transaction t2(db.begin());
        odb::result<account> r3 = db.query<account>(odb::query<account>::account_number == john.account_number());
        account john2 = *r3.begin();
        john2.balance(john2.balance() - 413.612);
        db.update(john2);
        t2.commit();
        throw e;
    }



}