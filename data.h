#ifndef __IDATA_H__
#define __IDATA_H__

#include <map>
#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>
using std::map;
using std::string;
using boost::shared_ptr;
using Finance::message;

class IDataType;
class IData;
class INT;
class INT64;
typedef shared_ptr<IData> IDataPtr;

#define  IntNullValue  -9999
#define  IntNullValueStr  "-9999"
#define  DateTimeNullValue "1900-01-01 00:00:00"
#define  StringNullValue "^N"
// #define  MysqlNull "null"
#define  MysqlNull "^Null"

// null is now in mysql format, need change.
// this is a virtual class

class DataRow : public map<string, IDataPtr>
{ 
 public:
 	DataRow() : connPtr(0), pump_seq(0) {} 
    int producerPtr; // used in multiproducer
	int connPtr;
	int64_t pump_seq;
};

//hint: use two level dispatch to calculate
class IData 
{
 public:
  IData() {//FLOG_DEBUG("IData construct");
  }
  virtual ~IData() { //FLOG_DEBUG("IData destruct");
  }
  virtual  IDataType* type() = 0;
  virtual  operator  int()  const { return 0;}
  virtual  operator  int64_t() const { return 0;}
  virtual  operator  double()  const {return 0;}
  virtual  operator  bool() const { return false;}
  virtual  operator  string()  const { return string();}
  virtual  IDataPtr operator+ (IData& data) { return IDataPtr();}
  virtual  IDataPtr operator- (IData& data) { return IDataPtr();}
  virtual  IDataPtr operator* (IData& data)  { return IDataPtr();}
  virtual  IDataPtr operator/ (IData& data)  { return IDataPtr();}
  virtual  IDataPtr operator> (IData& data)  { return IDataPtr();}
  virtual  IDataPtr operator< (IData& data)  { return IDataPtr();}
  
  static IDataPtr doCalculate(IDataPtr d1, IDataPtr d2, string& op)
  {
    if(op == "+")
      return *d2 + *d1;
    else if(op == "-")
      return *d2 - *d1;
    else if( op == "*")
      return *d2 * *d1;
    else if( op == "/")
      return *d2 / *d1;	
    else if( op == ">")
      return *d2 > *d1;
    else if( op == "<")
      return *d2 < *d1;
    else
      throw FinanceException(message("invalid operator ") << op);
  }
  
   friend ostream& operator<<(ostream &os, const IData& data) 
   {
  	  data.realOutput(os);
  	  return os; 
   }
   virtual void realOutput(ostream &os) const  = 0;

};

class Bool : public IData
{
 public:
  Bool(bool num): _num(num) {}
  ~Bool() {}
  virtual IDataType* type()
  {
    return realType;
  }
  virtual operator int() const 
  { 
    return _num; 
  }
  virtual operator int64_t() const 
  { 
    return _num;
  }
  virtual operator double() const 
  {
    return _num;
  }
  virtual  operator  bool() const 
  { 
     return _num;
  }
  virtual operator string() const 
  { 
    return Lexical_Cast<string>(_num); 
  }
  virtual IDataPtr operator+(IData& data)
  {
    return IDataPtr(new Bool(_num + bool(data)));
  }
  virtual IDataPtr operator-(IData& data)
  {
    return IDataPtr(new Bool(_num - bool(data)));
  }  
  virtual void realOutput(ostream &os) const
  {
     os << _num;
  }

 private:
  bool _num;
  static IDataType* realType;
};


class INT : public IData
{
 public:
  INT(int num): _num(num) {}
  ~INT() {}
  virtual IDataType* type()
  {
    return realType;
  }
  virtual operator int() const 
  { 
    return _num; 
  }
  virtual operator int64_t() const 
  { 
    return _num;
  }
  virtual operator double() const 
  {
    return _num;
  }  
  virtual  operator  bool() const 
  { 
     return _num;
  }
  virtual operator string() const 
  { 
    return Lexical_Cast<string>(_num); 
  }
  virtual IDataPtr operator+(IData& data)
  {
    return IDataPtr(new INT(_num + int(data)));
  }
  virtual IDataPtr operator-(IData& data)
  {
    return IDataPtr(new INT(_num - int(data)));
  }  
  virtual IDataPtr operator*(IData& data)
  {
    return IDataPtr(new INT(_num * int(data)));
  }
  virtual IDataPtr operator/(IData& data)
  {
    return IDataPtr(new INT(_num / int(data)));
  }  
  virtual IDataPtr operator>(IData& data)
  {
    return IDataPtr(new Bool(_num > int(data)));
  }
  virtual IDataPtr operator<(IData& data)
  {
    return IDataPtr(new Bool(_num < int(data)));
  }
  virtual void realOutput(ostream &os) const
  {
     os << _num;
  }
 private:
  int _num;
  static IDataType* realType;
};

class INT64 : public IData
{
 public:
  INT64(int64_t num): _num(num) {}
  ~INT64() {}
  virtual IDataType* type()
  {
    return realType;
  }
  virtual operator int() const 
  {
    return _num;
  }
  virtual operator int64_t() const 
  {
    return _num; 
  }
  virtual operator double() const 
  {
    return _num;
  }
  virtual  operator  bool() const 
  { 
     return _num;
  }
  virtual operator string() const 
  { //change here..
   if(_num != IntNullValue)
     return Lexical_Cast<string>(_num);
	else
		return MysqlNull;
  }
  virtual IDataPtr operator+(IData& data)
  {
    return IDataPtr(new INT64(_num + int64_t(data)));
  }
  virtual IDataPtr operator-(IData& data)
  {
    return IDataPtr(new INT64(_num - int64_t(data)));
  }
  virtual IDataPtr operator*(IData& data)
  {
    return IDataPtr(new INT64(_num * int64_t(data)));
  }
  virtual IDataPtr operator/(IData& data)
  {
    return IDataPtr(new INT64(_num / int64_t(data)));
  }
  virtual IDataPtr operator>(IData& data)
  {
    return IDataPtr(new Bool(_num > int64_t(data)));
  }
  virtual IDataPtr operator<(IData& data)
  {
    return IDataPtr(new Bool(_num < int64_t(data)));
  }
  virtual void realOutput(ostream &os) const
  {
    // os << _num;
    
	if(_num != IntNullValue)
	    os << _num;
	else
		os << MysqlNull;
  }

 private:
  int64_t _num;
  static IDataType* realType;
};

class Double : public IData
{
 public:
  Double(double num): _num(num) {}
  ~Double() {}
  virtual IDataType* type()
  {
    return realType;
  }
  virtual operator int() const 
  {
    return static_cast<int>(_num);
  }
  virtual operator int64_t() const 
  {
    return static_cast<int64_t>(_num); 
  }
  virtual operator double() const 
  { 
    return _num; 
  }
  virtual  operator  bool() const 
  { 
     return _num;
  }
  virtual operator string() const 
  { 
    if(_num != IntNullValue)
       return Lexical_Cast<string>(_num);
	else
	   return MysqlNull;    
  }
  virtual IDataPtr operator+(IData& data)
  {
    return IDataPtr(new Double(_num + double(data)));
  }
  virtual IDataPtr operator-(IData& data)
  {
    return IDataPtr(new Double(_num - double(data)));
  }  
  virtual IDataPtr operator*(IData& data)
  {
    return IDataPtr(new Double(_num * double(data)));
  }
  virtual IDataPtr operator/(IData& data)
  {
    return IDataPtr(new Double(_num / double(data)));
  }
  
  virtual IDataPtr operator>(IData& data)
  {
    return IDataPtr(new Bool(_num > double(data)));
  }
  virtual IDataPtr operator<(IData& data)
  {
    return IDataPtr(new Bool(_num < double(data)));
  }
  
  virtual void realOutput(ostream &os) const
  {
    if(_num != IntNullValue)
	    os << _num;
	else
		os << MysqlNull;
  }
 private:
  double _num;
  static IDataType* realType;
};


class String: public IData
{
 public:
  String(string str) : _str(str) {}	
  ~String() {}
  virtual IDataType* type()
  {
    return realType;
  }
  virtual operator int() const
  {
     return Lexical_Cast<int>(_str);
  }
  virtual operator int64_t() const
  {
     return Lexical_Cast<int64_t>(_str);
  }
  virtual operator double() const
  {
      return Lexical_Cast<double>(_str);
  }
  virtual operator string() const 
  { 
   // return _str;
    if(_str != StringNullValue)
       return _str;
	else
	   return MysqlNull;   
  }
  virtual IDataPtr operator+(IData& data)
  {
    return IDataPtr(new String(_str + string(data)));
  }
  virtual void realOutput(ostream &os) const
  {
  //   os << _str;
    if(_str != StringNullValue)
       os <<  _str;
	else
	   os << MysqlNull;   
  }
 private:
  string _str;
  static IDataType* realType;
};

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time; 

//operate could be added here
class DateTime : public IData
{
 public:
  DateTime(string str) : _dateStr(str) {} 
  ~DateTime() {}
  virtual IDataType* type()
  {
    return realType;
  }
  virtual operator string() const 
  { 
   if(_dateStr != DateTimeNullValue)
       return _dateStr;
 	else
 	   return MysqlNull;
  }
  virtual operator ptime() const
  {
    return ptime(time_from_string(_dateStr));
  }
  virtual void realOutput(ostream &os) const
  {
	if(_dateStr != DateTimeNullValue)
		os << _dateStr;
	 else
		os << MysqlNull;
    
  }

 private:
  string _dateStr;
  static IDataType* realType;
};

typedef std::pair<string, IDataPtr> MetaData;

void freeDataRow(DataRow & drow);

#endif

