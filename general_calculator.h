#ifndef _GENERAL_CALCULATOR_H_
#define _GENERAL_CALCULATOR_H_

#include <calculator/calculator.h>


class GeneralCalculator : public Calculator
{
 public:  
  GeneralCalculator(xml_node& node);
  virtual ~GeneralCalculator() {}
  
  virtual void calculate(list<DataRow>& dataList, list<DataRow>& resultList);  

  /* check whether every operatand in each column is in externcolumns 
  also set _calctype for each column, _calctype is identical with type 
  of the first operand  in opvec, otherwise it is identical with calccolumn._type*/
  bool checkColumns(const vector<Column>& externColumns);

private:
	vector<shared_ptr<CalcColumn> > _calcColumns;
};

#endif

