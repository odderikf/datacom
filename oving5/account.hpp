#include <utility>

//
// Created by odderikf on 3/22/19.
//

#ifndef OVING5_ACCOUNT_HPP
#define OVING5_ACCOUNT_HPP

#include <odb/core.hxx>
#include <string>

using std::string;

#pragma db object optimistic
class account{
private:
    friend class odb::access;
    account(){}; //NOLINT(

    #pragma db id auto
    int _account_number;

    #pragma db default(0)
    double _balance;
    #pragma db type("VARCHAR(100)")
    string _name;

    #pragma db version default(0)
    unsigned long version;

public:
    const int &account_number() const{ return _account_number; }
    const double &balance() const{return _balance; }
    void balance(const double &v){ _balance=v; }
    const string &name() const{ return _name; }
    void name(const string &v) { _name=v; }

    account(const string &name) : _name(name){} explicit ;
};


#endif //OVING5_ACCOUNT_HPP
