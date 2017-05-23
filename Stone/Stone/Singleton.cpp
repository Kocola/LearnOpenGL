#include "Singleton.h"

//模板实现必须在.h文件中实现，否则即使在.cpp文件中实现，
//也会导致链接失败（无法解析的外部符号）
/*template<typename T>
T& Singleton<T>::getInstance()
{
	static T s_instance;
	return s_instance;
}*/