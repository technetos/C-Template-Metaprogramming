// primary template to handle types that are not const qualified, just return the type given :D
template<typename T>
  struct remove_const
{
  using type = T;
};

// partial specialization match for const qualified types
template<typename T>
  struct remove_const<T const>
{
  // Just ignore the const and return the type 
  using type = T;
};
