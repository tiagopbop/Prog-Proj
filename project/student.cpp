#include <iostream>
#include <string>
using namespace std;
#include "Student.h"

  Student::Student(const string& name, const string& id, 
          short ac, short p1, short p2)
          {
            this -> name_ = name;
            this -> id_ = id;
            this -> ac_ = ac;
            this -> p1_ = p1;
            this -> p2_ = p2;
          }

    string Student:: get_id() const
    {
        return id_;
    }
    
    string Student:: get_name() const
    {
        return name_;
    }
    
    double Student:: actual_grade() const
    {
        return (0.1*ac_ + 0.45*p1_ + 0.45*p2_);
    }



