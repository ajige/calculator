#include <algorithm>
#include <stack>
#include <calculator/general_calculator.h>
#include <base/utility/common.h>
#include <boost/tokenizer.hpp> 

using Finance::message;
using boost::tokenizer;

GeneralCalculator::GeneralCalculator(xml_node& node) 
{
  _name = node.get_attr("name").get_value();

  // add behavior and primary_id at first, take care here
  shared_ptr<CalcColumn> permanentCol 
      ( new CalcColumn(string(BEHAVIOR), string("int"), string("$"BEHAVIOR)));
  _calcColumns.push_back(permanentCol);
  _columns.push_back(*permanentCol);
  
  permanentCol.reset( new CalcColumn(string(PRIMARY_ID), string("int64"), 
        string("$"PRIMARY_ID)));
  _calcColumns.push_back(permanentCol);
  _columns.push_back(*permanentCol);
  
  // leave behind... change
  list<xml_node> columnList;
  node.get_first_child("columns").get_children("column", columnList);
  for(list<xml_node>::iterator iter = columnList.begin();
      iter != columnList.end();
      iter++)
  {
    shared_ptr<CalcColumn> column(new CalcColumn(*iter)); 
    _calcColumns.push_back(column);
    _columns.push_back(*column);
  }
      
}

void GeneralCalculator::calculate(list<DataRow>& dataList, list<DataRow>& resultList)
{
  for(list<DataRow>::iterator iter = dataList.begin(), end = dataList.end();
      iter != end;
      iter++)
  {
    DataRow dr;
    dr.pump_seq = iter->pump_seq;// important here.
    for(size_t i = 0; i < _calcColumns.size(); i++)
    {
      string name = _calcColumns[i]->_name;
      IDataPtr data = _calcColumns[i]->doCalculate(*iter);
      if(_calcColumns[i]->_type == data->type())
          dr[name] = data;  
      else // type conversion should be here in case src type is different from result type
      {     
        IDataPtr newData = _calcColumns[i]->_type->newObject(data);
        dr[name] = newData;
      }
    }
    
    resultList.push_back(dr);
  }
}

bool GeneralCalculator::checkColumns(const vector<Column>& externColumns)
{
  size_t icol = 0, iop = 0, iextern = 0;
  for(icol = 0; icol < _calcColumns.size(); icol++)
  {
    const vector<shared_ptr<OperateUnit> >& opvec = _calcColumns[icol]->getOpVec();
    for(iop = 0; iop < opvec.size(); iop++)
    {
      if( opvec[iop]->type() == OperateUnit::OPERAND )
      {
         for( iextern = 0; iextern < externColumns.size(); iextern++)
         {
            if( externColumns[iextern]._name == *opvec[iop] )
            {
                break;
            }
         } 
         if(iextern >= externColumns.size())// operand is not found in producer columns
         {
            FLOG_ERROR(*opvec[iop] << "not found in producers");//change
            return false;
         }
         if( _calcColumns[icol]->getCalcType() == NULL)
            _calcColumns[icol]->setCalcType(externColumns[iextern]._type);
      }
    }
    
    if( _calcColumns[icol]->getCalcType() == NULL )
        _calcColumns[icol]->setCalcType(_calcColumns[icol]->_type);
    
  }
  return true;
}

