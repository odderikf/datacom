#include <utility>

//
// Created by odderikf on 3/22/19.
//

#ifndef OVING5_ACCOUNT_HPP
#define OVING5_ACCOUNT_HPP

#include <odb/core.hxx>
#include <string>

using std::string;

#pragma db object
class account{
private:
    friend class odb::access;
    account(){};

    #pragma db id auto
    int _account_number;

    double _balance;
    #pragma db type("VARCHAR(100)")
    string _name;

    int version;

public:
    const int &account_number() const{ return _account_number; }
    const double &balance() const{return _balance; }
    void balance(const double &v){ _balance=v; }
    const string &name() const{ return _name; }
    void name(const string &v) { _name=v; }

    account(string name) : _name(name), _balance(0), version(0) {};
};

#endif //OVING5_ACCOUNT_HPP
