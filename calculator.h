#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <base/datatype.h>
#include <base/table.h>
#include <base/utility/common.h>
#include <base/xml/xml_node.h>
#include <base/xml/xml_document.h>
#include <base/xml/xml_schema.h>
#include <boost/shared_ptr.hpp>
#include <calculator/calcolumn.h>

class Calculator
{
 public:  
  Calculator(){};
  virtual ~Calculator() {}
  virtual void calculate(list<DataRow>& dataList, list<DataRow>& resultList) = 0;  

  /* check whether every operatand in each column is in externcolumns 
  also set _calctype for each column, _calctype is identical with type 
  of the first operand  in opvec, otherwise it is identical with calccolumn._type*/
  virtual bool checkColumns(const vector<Column>& externColumns) = 0;
   
  const vector<Column>& getColumns() const
  {
    return _columns;
  }
   
  const string& getName() const
  {
    return _name;
  }

 protected:
  string _name;  
  vector<Column> _columns;
};

#endif

