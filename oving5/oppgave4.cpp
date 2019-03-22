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

    string waiter;

    cin >> waiter;
    cout << john.balance() << endl;
    cout << "changing balance!" << endl;
    john.balance( john.balance()-413.612 );
    cout << john.balance() << endl;

    db.update(john);

    t.commit();

}