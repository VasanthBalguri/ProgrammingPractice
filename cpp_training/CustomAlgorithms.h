#ifndef CUSTOMALGORITHMS_H
#define CUSTOMALGORITHMS_H

template<typename T>void printStr(T value)
{
    char list[sizeof(T)/sizeof(char) + sizeof(char)];
    for(int i = 0; i< sizeof(T)/sizeof(char) + sizeof(char) ; i++)
    {
        if(i == sizeof(T)/sizeof(char))
        {
            list[i] = '\0';
        }
        {
            list[i] = ((char*)&value)[i];
            std::cout<<list[i];
        }
    }
}

template<typename T> int _generateHash(T& value)
{
    char data[sizeof(T)/sizeof(char) + sizeof(char)];
    for(int i = 0; i < sizeof(T)/sizeof(char); i = i + sizeof(char))
    {
        data[i] = ((char*)(&value))[i];
    }
    data[sizeof(T)/sizeof(char)] = '\0';
    int hash = 0;
    for(int i = 0; i < sizeof(T)/sizeof(char); i = i + sizeof(char))
    {
       hash = hash + data[i] * (i % 3) * 2 * 3 / 5 + (data[i] > 0 ? -1 : 1 ) * data[i] * (i % 7) * 2 * 3 / 11;
    }
    return hash;
}

template<typename T,typename T>
struct is_same_type { static constexpr bool value = true;};

template<typename T,typename U>
struct is_same_type { static constexpr bool value = false;};

template<typename ...T>
struct varadicUnpack
{
   constexpr bool is_end = false;
    static varadicUnpack unpack();
};

template<typename First,typename ...Rest>
struct varadicUnpack
{
   constexpr bool is_end = false;
   static varadicUnpack unpack()
   {
        return varadicUnpack<Rest...>::unpack();
   }
};

template<>
struct varadicUnpack
{
    constexpr bool is_end = true;
};

#endif // CUSTOMALGORITHMS_H
