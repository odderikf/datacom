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
    john.balance( john.balance()+413.612 );
    db.update(john);

    cout << "waiting";
    cin >> waiter;

    cout << "changing jane";
    jane.balance(jane.balance() - 413.612);
    db.update(jane);

    t2.commit();


}