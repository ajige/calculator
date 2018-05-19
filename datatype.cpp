#include <base/datatype.h>

IDataType* INT::realType = IntType::instance();
IDataType* INT64::realType = Int64Type::instance();
IDataType* Double::realType = DoubleType::instance();
IDataType* Bool::realType = BoolType::instance();
IDataType* String::realType = StringType::instance();
IDataType* DateTime::realType = DateTimeType::instance();

IntType IntType::intType = IntType();
Int64Type Int64Type::int64Type = Int64Type();
DoubleType DoubleType::doubleType = DoubleType();
BoolType BoolType::boolType = BoolType();
StringType StringType::strType = StringType();
DateTimeType DateTimeType::dateTimeType = DateTimeType();


