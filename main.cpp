#include <iostream>


class AOne { };
class ATwo { };

template <typename A> class BOne { };
template <typename A> class BTwo { };

template <typename> struct AName;
template <> struct AName<AOne> { static constexpr const char * const value = "AOne"; };
template <> struct AName<ATwo> { static constexpr const char * const value = "ATwo"; };

template <typename B> struct BName;
template <typename A> struct BName<BOne<A>> { static constexpr const char * const value = "BOne"; };
template <typename A> struct BName<BTwo<A>> { static constexpr const char * const value = "BTwo"; };

template <typename B> struct BGetAName;
template <typename A, template<class> class B> struct BGetAName<B<A>> { static constexpr const char * const value = AName<A>::value; };


int main()
{
    AOne aOne;
    ATwo aTwo;
    
    BOne<AOne> bOneOne;
    BOne<ATwo> bOneTwo;
    BTwo<AOne> bTwoOne;
    BTwo<ATwo> bTwoTwo;

    std::cout << AName<decltype(aOne)>::value << std::endl;
    std::cout << AName<decltype(aTwo)>::value << std::endl;

    std::cout << BName<decltype(bOneOne)>::value << std::endl;
    std::cout << BName<decltype(bOneTwo)>::value << std::endl;
    std::cout << BName<decltype(bTwoOne)>::value << std::endl;
    std::cout << BName<decltype(bTwoTwo)>::value << std::endl;

    std::cout << BGetAName<decltype(bOneOne)>::value << std::endl;
    std::cout << BGetAName<decltype(bOneTwo)>::value << std::endl;
    std::cout << BGetAName<decltype(bTwoOne)>::value << std::endl;
    std::cout << BGetAName<decltype(bTwoTwo)>::value << std::endl;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// class AOne { };
// class ATwo { };

// template <typename T> constexpr const char * AName()
// {
//     if constexpr (std::is_same_v<T, AOne>) return "AOne";
//     else if constexpr (std::is_same_v<T, ATwo>) return "ATwo";
//     else return "Unknown";
// }


// int main()
// {
//     AOne aOne;
//     ATwo aTwo;

//     std::cout << AName<decltype(aOne)>() << std::endl;
//     std::cout << AName<decltype(aTwo)>() << std::endl;

//     return 0;
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// class AOne { };
// class ATwo { };

// template <typename> struct AName;
// template <> struct AName<AOne> { static constexpr const char * const value = "AOne"; };
// template <> struct AName<ATwo> { static constexpr const char * const value = "ATwo"; };


// int main()
// {
//     AOne aOne;
//     ATwo aTwo;

//     std::cout << AName<decltype(aOne)>::value << std::endl;
//     std::cout << AName<decltype(aTwo)>::value << std::endl;

//     return 0;
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// class AOne { };
// class ATwo { };


// template <typename B> class BOne { };
// template <typename B> class BTwo { };


// template <class> struct AName;
// template <> struct AName<AOne> { static constexpr const char * const value = "AOne"; };
// template <> struct AName<ATwo> { static constexpr const char * const value = "ATwo"; };


// template <class> struct BName;
// template <class B> struct BName<BOne<B>> { static constexpr const char * const value = "BOne"; };
// template <class B> struct BName<BTwo<B>> { static constexpr const char * const value = "BTwo"; };


// int main()
// {
//     AOne aOne;
//     ATwo aTwo;
//     BOne<AOne> bOneOne;
//     BOne<ATwo> bOneTwo;
//     BTwo<AOne> bTwoOne;
//     BTwo<ATwo> bTwoTwo;

//     std::cout << AName<bOne>::value << std::endl;

//     return 0;
// }
