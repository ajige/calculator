
#include <calculator/calcolumn.h>
#include <boost/tokenizer.hpp> 

using Finance::message;
using boost::tokenizer;
using boost::char_separator;
using boost::drop_empty_tokens;
using std::stack;

static OperateUnit::OPTYPE getType(const string& op)
{
  if(op == "+" || op == "-" || op == "*" || op == "/" || op == "(" || op == ")" || op == "<" || op==">")
    return OperateUnit::OPERATOR;
  else if( op[0] == '$' )
    return OperateUnit::OPERAND;
  else
    return OperateUnit::CONSTOPERAND;
}

static uint16_t getLevel(const string& op)
{
  if(op == "*" || op =="/")
    return 3;
  else if( op == "+" || op == "-")
    return 2;  
  else if( op == ">" || op == "<")
    return 1;
  else // (   and  )
    return 0;
}

static bool compareOperatorLevel(const string& op1, const string& op2)
{
   if(getLevel(op1) > getLevel(op2))
     return true;
   else
     return false;
}

CalcColumn::CalcColumn(xml_node& node) : Column(node)
{    
  string calcString = node.get_attr("value").get_value();
  infixToPostfix(calcString, _opVec);
  _calcType = NULL;
}

CalcColumn::CalcColumn(string name, string type, string calcString) : Column(name, type)
{
  infixToPostfix(calcString, _opVec);
  _calcType = NULL;
}

void CalcColumn::infixToPostfix(string& calcString, vector<shared_ptr<OperateUnit> >& opVec)
{
  char_separator<char> separators(" ", "", drop_empty_tokens);
  tokenizer<char_separator<char> > tokens(calcString, separators);
  tokenizer<char_separator<char> >::iterator tokIter = tokens.begin();
  stack<string> opStack;
  
  for(; tokIter != tokens.end(); tokIter++)
  {
    if(getType(*tokIter) == OperateUnit::OPERAND)
    {
       char tempstr[128] = {0};
       (*tokIter).copy(tempstr, (*tokIter).size() - 1, 1); // erase the prefix : $
       opVec.push_back(shared_ptr<OperateUnit>(new Operand(tempstr)));       
       continue;
    }
    else if( getType(*tokIter) == OperateUnit::CONSTOPERAND)
    {
       opVec.push_back(shared_ptr<OperateUnit>(new OperandConst(*tokIter)));
       continue;
    }

    if(*tokIter == "(")
    {
      opStack.push(*tokIter);     
    }
    else if( *tokIter == ")" )
    {
      while(!opStack.empty())
      {
        string topOp = opStack.top();
        opStack.pop();
        if( topOp == "(")
          break;
        else
          opVec.push_back(shared_ptr<OperateUnit>(new Operator(topOp)));
      }
    }
    else
    {
      if( opStack.empty())
         opStack.push(*tokIter);
      else
      {
        string topOp = opStack.top();
        if(compareOperatorLevel(*tokIter, topOp) == true)
          opStack.push(*tokIter);
        else
        {
          opVec.push_back(shared_ptr<OperateUnit>(new Operator(topOp)));
          opStack.pop();
          opStack.push(*tokIter);
        }
      } 
    }
  }

  while(!opStack.empty())
  {
    string topOp = opStack.top();
    opVec.push_back(shared_ptr<OperateUnit>(new Operator(topOp)));
    opStack.pop();
  }

  // for debug
  string opVecString = *opVec[0];
  for(size_t i = 1; i < opVec.size(); i++)
    opVecString.append(*opVec[i]);
  FLOG_DEBUG(opVecString);
}

//only binary operator is supported currently.
IDataPtr CalcColumn::doCalculate(DataRow& dr)
{
  stack<IDataPtr> calcStack;
  for(size_t i = 0; i < _opVec.size(); i++)
  {
    if( _opVec[i]->type() == OperateUnit::OPERAND )
    {
       string operand = *_opVec[i];
       calcStack.push(dr[operand]);
    }
    else if ( _opVec[i]->type() == OperateUnit::CONSTOPERAND)
    {
       IDataPtr dptr = _calcType->newObject(String(*_opVec[i]));
       calcStack.push(dptr);
    }
    else
    {
      if(!calcStack.empty()) 
      {
        IDataPtr data1 = calcStack.top();
        calcStack.pop();
        if( !calcStack.empty() )
        {
           IDataPtr data2 = calcStack.top();
           calcStack.pop();
           IDataPtr result = IData::doCalculate(data1, data2, *_opVec[i]);
           calcStack.push(result);
        }
      }
    }
  }
  
  return calcStack.top();
}

