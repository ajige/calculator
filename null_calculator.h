#ifndef _NULL_CALCULATOR_H_
#define _NULL_CALCULATOR_H_

#include <calculator/calculator.h>
#include <algorithm>

class NullCalculator: public Calculator
{  
 public:
  NullCalculator(vector<Column>& pcolumns) 
  {
    _name = "NullCalculator";
	_columns = pcolumns;
  }
  
  virtual ~NullCalculator() {}

  virtual void calculate(list<DataRow>& dataList, list<DataRow>& resultList)
  {
      resultList = dataList;
  }

  bool checkColumns(const vector<Column>& externColumns) 
  {
     return true;
  }
};

#endif

