#ifndef _CALCOLUMN_H_
#define _CALCOLUMN_H_

#include <list>
#include <set>
#include <base/utility/common.h>
#include <base/datatype.h>
#include <base/table.h>

using std::list;
using std::set;

class OperateUnit : public string
{
 public:
  OperateUnit(string op) : string(op) {}
  virtual ~OperateUnit() {}

  enum OPTYPE {
    OPERATOR = 0,
	OPERAND = 1,
	CONSTOPERAND = 2,
  };  
  virtual OPTYPE type() = 0;
};

class Operator: public OperateUnit
{
 public:
  Operator(string op) : OperateUnit(op) {}
  OPTYPE type() { return OPERATOR;}
};

class Operand : public OperateUnit
{
 public: 
  Operand(string op) : OperateUnit(op) {}
  OPTYPE type() { return OPERAND;}
};

class OperandConst : public OperateUnit
{
  public:
  	OperandConst(string op) : OperateUnit(op) {}
	OPTYPE type() { return CONSTOPERAND; }
	operator INT() 
	{
       return INT(Lexical_Cast<int>(*this));
	}	
	operator INT64() 
	{
       return INT64(Lexical_Cast<int64_t>(*this));
	}
	operator Double() 
	{
       return Double(Lexical_Cast<double>(*this));
	}
	operator Bool() 
	{
       return Bool(Lexical_Cast<bool>(*this));
	}
	operator String() 
	{
       return String(Lexical_Cast<string>(*this));
	}
	operator DateTime()
	{
        return DateTime(Lexical_Cast<string>(*this));
	}
};


class CalcColumn : public Column
{
 public:
  CalcColumn(xml_node& node);

  CalcColumn(string name, string type, string calcString);

  IDataPtr doCalculate(DataRow& dr);
  
  /*convert calcString to postfix format   */
  static void infixToPostfix(string& calcString, vector<shared_ptr<OperateUnit> >& opVec); 

  const vector< shared_ptr<OperateUnit> >& getOpVec() const
  {  
    return _opVec; 
  }
  IDataType* getCalcType()
  {
    return _calcType;
  }
  
  void setCalcType(IDataType* type)
  {
     _calcType = type;
  }
  
 private:

  /*store the postfix calculation */
  vector<shared_ptr<OperateUnit> > _opVec;
  
  /* store the type for const operator, it should be identical with type of operand in the calculation
  set _calcType in calculator::checkColumns()*/
  IDataType* _calcType;  
};

#endif

