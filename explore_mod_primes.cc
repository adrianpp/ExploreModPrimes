#include <iostream>
#include <vector>
#include <cmath> //for sqrt

std::vector<int> filled_mod_groups(int mod, std::vector<int> values)
{
    std::vector<int> ret(mod,0); //mod number of mod group slots [0...mod)
    for(std::vector<int>::iterator II = values.begin(); II != values.end(); ++II)
    {
        ++ret[*II % mod];
    }
    return ret;
}

template<class Func, class Cont, class Type=int>
Type applyCollect(Cont vec, Type init, Func f)
{
    Type ret = init;
    for(auto II = vec.begin(); II != vec.end(); ++II)
    {
        ret = f(*II, ret);
    }
    return ret;
}

int max(std::vector<int> i)
{
    return applyCollect(i, 0, [](int i, int m) {return (i>m) ? i : m;});
}

bool isPrime(int i)
{
    if( i <= 2 ) return true;
    for(int j  = 2; j < sqrt(i)+1; ++j)
    {
        if( i % j == 0 )
            return false;
    }
    return true;
}

bool areAllPrime(std::vector<int> i)
{
    return applyCollect(i, true, [](int i, bool a){return (isPrime(i) and a);});
}

template<class T>
std::ostream& operator << (std::ostream& os, std::vector<T> i)
{
    for(typename std::vector<T>::iterator II = i.begin(); II != i.end(); ++II)
        os << *II << " ";
    return os;
}

struct Modulus {
    int mod;
    int base;
    Modulus(int m, int b) : mod(m), base(b) {}
};

std::ostream& operator << (std::ostream& os, Modulus m)
{
    os << m.mod << "m" << m.base;
    return os;
}

std::vector<Modulus> get_remainder_requirements(std::vector<int> values)
{
    std::vector<Modulus> ret;
    for(int base = 2; base <= 5; ++base)
    {
      if( !isPrime(base) ) continue;
      std::vector<int> filled = filled_mod_groups(base, values);
      std::cout << "filled_" << base << " = {" << filled << "}\n";
      for(int mod = 0; mod < base; ++mod)
      {
          if( filled.at(mod) != 0 )
              ret.push_back(Modulus((base-mod)%base,base));
      }
    }
    return ret;
}

bool satisfies_all_remainder_requirements(int value, std::vector<Modulus> requirements)
{
  for(std::vector<Modulus>::iterator RI = requirements.begin(); RI != requirements.end(); ++RI)
  {
      if( value % RI->base == RI->mod ) return false;
  }
  return true;
}

int main()
{
    std::vector<int> values;
    values.push_back(13-5);
    values.push_back(23-5);
    values.push_back(37-5);
   
    std::cout << "Solving for values {" << values << "} + N:\n";
    std::vector<Modulus> requirements = get_remainder_requirements(values);
    std::cout << "N cannot be: " << requirements << "\n";
    std::vector<int> possibleN;
    for(int i = 0; i < 100; ++i)
    {
        if( satisfies_all_remainder_requirements(i, requirements) )
            possibleN.push_back(i);
    }
    std::cout << "Possible N = {" << possibleN << "}\n";
    for(std::vector<int>::iterator PI = possibleN.begin(); PI != possibleN.end(); ++PI)
    {
        std::vector<int> possibleVals;
        for(std::vector<int>::iterator VI = values.begin(); VI != values.end(); ++VI)
            possibleVals.push_back(*PI+*VI);
        std::cout << "N = " << *PI << ": values = {" << possibleVals << "}, allPrime = " << areAllPrime(possibleVals) << "\n";
    }
    return 0;
}
