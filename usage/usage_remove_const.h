#include"../remove_const.h"
#include<string>

//remove_const<T>::type t;

remove_const<const std::string>::type t("look a non-const string");
