#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#include <octopus/octopus.h>
#include <msoal/MConnection.h>
#include <msoal/MSoalException.h>
#include <msoal/MStatement.h>
#include <base/utility/common.h>
#include <base/utility/lexical_cast.h>
#include <boost/shared_ptr.hpp>
#include <base/data.h>

using boost::shared_ptr;
using octopus::OctopusException;
using octopus::ResultSet;

using MSoal::MSoalException;
using MSoal::MResultSet;

class IDataType 
{
 public:
  virtual IDataPtr getValue(shared_ptr<ResultSet> rs, const string& name) { return IDataPtr();}
  virtual IDataPtr getValue(MResultSet& rs, const string& name) {return IDataPtr();}  
  virtual IDataPtr newObject(IDataPtr data) {return IDataPtr();}
  virtual IDataPtr newObject(String data) { return IDataPtr(); }
  virtual string getNullValue(){  return "NULL";}
  virtual ~IDataType() {}
};


class IntType : public IDataType
{
 public:  	
  IntType() {}
  virtual ~IntType() {}
  IDataPtr getValue(shared_ptr<ResultSet> rs, const string& name)
  {
    int ret = rs->getInt(name);
    return IDataPtr(new INT(ret));
  }
  IDataPtr getValue(MResultSet&rs, const string& name)
  {
    int ret = rs.getInt(name);
    return IDataPtr(new INT(ret));
  }
  IDataPtr newObject(IDataPtr data)
  {
     return IDataPtr(new INT(int(*data)));
  }
  IDataPtr newObject(String data)
  {
     return IDataPtr(new INT(data));
  }
  static IDataType* instance()
  {
    return &intType;
  }
 private:
  static IntType intType;
};

class Int64Type : public IDataType
{
 public:  	
  Int64Type() {}
  virtual ~Int64Type() {}
  IDataPtr getValue(shared_ptr<ResultSet> rs, const string& name)
  {
    if(rs->isNull(name))
		return IDataPtr( new INT64(IntNullValue));
    int64_t ret = rs->getInt64(name);
    return IDataPtr(new INT64(ret));
  }
  IDataPtr getValue(MResultSet&rs, const string& name)
  {
    int64_t ret = rs.getLong(name);
    return IDataPtr(new INT64(ret));
  }  
  IDataPtr newObject(IDataPtr data)
  {
     return IDataPtr(new INT64(int64_t(*data)));
  }
  IDataPtr newObject(String data)
  {
     return IDataPtr(new INT64(data));
  }
  static IDataType* instance()
  {
    return &int64Type;
  }
  string getNullValue()
  {  
     return string(IntNullValueStr);
  }
 private:
  static Int64Type int64Type;
};

class DoubleType: public IDataType
{
 public:  	
  DoubleType() {}
  virtual ~DoubleType() {}
  IDataPtr getValue(shared_ptr<ResultSet> rs, const string& name)
  {
    if(rs->isNull(name))
		return IDataPtr( new Double(IntNullValue));
    double ret = rs->getDouble(name);
    return IDataPtr(new Double(ret));
  }
  IDataPtr getValue(MResultSet&rs, const string& name)
  {
    double ret = rs.getDouble(name);
    return IDataPtr(new Double(ret));
  }
  IDataPtr newObject(IDataPtr data)
  {
     return IDataPtr(new Double(double(*data)));
  }
  IDataPtr newObject(String data)
  {
     return IDataPtr(new Double(data));
  }
  static IDataType* instance()
  {
    return &doubleType;
  }
  string getNullValue()
  {  
    return string(IntNullValueStr);
  }
 private:
  static DoubleType doubleType;  
};

class BoolType: public IDataType
{
 public:  	
  BoolType() {}
  virtual ~BoolType() {}
  IDataPtr getValue(shared_ptr<ResultSet> rs, const string& name)
  {
    bool ret = rs->getBoolean(name);
    return IDataPtr(new Bool(ret));
  }
  IDataPtr getValue(MResultSet&rs, const string& name)
  {
    int ret = rs.getUInt(name);
    return IDataPtr(new Bool(ret));
  }
  IDataPtr newObject(IDataPtr data)
  {
     return IDataPtr(new Bool(bool(*data)));
  }
  static IDataType* instance()
  {
    return &boolType;
  }
 private:
  static BoolType boolType;  
};

class StringType : public IDataType
{
 public:	
  StringType() {}
  virtual ~StringType() {}
  IDataPtr getValue(shared_ptr<ResultSet> rs, const string& name)
  {
    // todo replace
    if(rs->isNull(name))
		//return IDataPtr(new String("'"StringNullValue"'"));
		return IDataPtr(new String(StringNullValue));

	string val = rs->getString(name);
	replaceSpecialCharacter(val);
    return IDataPtr(new String(val));
  }
  IDataPtr getValue(MResultSet& rs, const string& name)
  {
    string val = rs.getString(name);
	replaceSpecialCharacter(val);
    return IDataPtr(new String(val));
  }
  IDataPtr newObject(IDataPtr data)
  {
	return IDataPtr(new String(*data));
  }
  IDataPtr newObject(String data)
  {
     return IDataPtr(new String(data));
  }
  string getNullValue()
  {  
     return string("'"StringNullValue"'");
  }
  static IDataType* instance()
  {
    return &strType;
  }
 private:

  // first replace \ with \\
  // then replace ' with \'
  // then replace $# with \$\#, since octopus will replace $# with -1
  // because \ in mysql is special. this function used only when from oracle2mysql.
  void replaceSpecialCharacter(string& str)
  {
	string::size_type  pos(0); 
	while((pos=str.find("\\", pos))!=string::npos)
	{
      	 str.replace(pos,1,"\\\\");																										 
		 pos = pos + 2;	
	}	
	while((pos=str.find("'", pos))!=string::npos)
	{	
	  str.replace(pos,1,"\\'"); 																										 
	  pos = pos + 2;	
	}
	while((pos=str.find("$#", pos))!=string::npos)
    {   
      str.replace(pos,2,"\\$\\#");    
      pos = pos + 4;  
    }   
  }

  static StringType strType;
};

class DateTimeType: public IDataType
{
 public:  	
  DateTimeType() {}
  virtual ~DateTimeType() {}
  IDataPtr getValue(shared_ptr<ResultSet> rs, const string& name)
  {    
    if(rs->isNull(name))
		return IDataPtr(new DateTime(DateTimeNullValue));

    string ret = rs->getString(name);
    return IDataPtr(new DateTime(ret));
  }
  IDataPtr getValue(MResultSet&rs, const string& name)
  {
    string ret = rs.getDate(name).toString();
    return IDataPtr(new DateTime(ret));
  }
  IDataPtr newObject(IDataPtr data)
  {
     return IDataPtr(new DateTime(*data));
  }
  static IDataType* instance()
  {
    return &dateTimeType;
  }
   // in oracle format, emergency
  string getNullValue()
  { 
      return string("to_date('"DateTimeNullValue"', 'yyyy-mm-dd hh24:mi:ss')");
  }
 private:
  static DateTimeType dateTimeType;  
};

class IDataTypeContainer
{
 public:
  static IDataType* getType(const string& dtype)
  {
    if(dtype == "int")
      return IntType::instance();
    else if(dtype == "string")
      return StringType::instance();
    else if(dtype == "int64")
      return Int64Type::instance(); // change here..
    else if(dtype == "double")
	  return DoubleType::instance();
	else if(dtype == "datetime")
	  return DateTimeType::instance();
	else if(dtype == "bool")
	  return BoolType::instance();
	//add other types here..
    else
    {
      throw FinanceException("IDataTypeFactory::getType unknown dtype");
    }  
  }
};

#endif

