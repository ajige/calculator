#ifndef _CALCULATOR_MANAGER_H_
#define _CALCULATOR_MANAGER_H_

#include <map>
#include <boost/shared_ptr.hpp>
#include <calculator/calculator.h>

using std::map;
using boost::shared_ptr;

class CalculatorManager 
{
public:
   CalculatorManager() {}
   
   void addCalculator(shared_ptr<Calculator> calc)
   {
      _calculators[calc->getName()] = calc;
   }
   
   shared_ptr<Calculator> getCalculator(string name)
   {
      shared_ptr<Calculator> calc;
	  map<string, shared_ptr<Calculator> >::iterator iter = _calculators.find(name);
	  if(iter != _calculators.end())
	  	calc = iter->second;
	  else
	  	throw FinanceException(message("invalid calculator name") << name);
      return calc;
   }
   
private:
  map<string, shared_ptr<Calculator> > _calculators;   
};

#endif

