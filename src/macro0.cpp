
#include<iostream>
#include<string>
#include<map>
#include<variant>

struct T
{
    std::uint32_t first;
    std::uint32_t second;
};

inline std::ostream& operator<<(std::ostream& os, const T& t)
{
    os << t.first << "-" << t.second;
    return os;   
}

inline std::string create_xxx()
{
    return std::string("ABCDEFG");
}

inline std::uint32_t create_yyy()
{
    return 12345;
}

inline T create_zzz()
{
    return {1100, 2200};
}

struct S
{
    std::string xxx;
    std::uint32_t yyy;
    T zzz;
};

/*  member = complete name of function / object / member 
   #member = string, no explicit double quote "" needed
  ##member = partial name of function / object / member   */

#define fill_map(map_object, member) \
    map_object[#member] = create_##member();
    
#define fill_struct(struct_object, member) \
    struct_object.member = create_##member();


void test_macro()
{
    std::map<std::string, std::variant<std::string, std::uint32_t, T>> m0;
    std::map<std::string, std::variant<std::string, std::uint32_t, T>> m1;
    S s0;
    S s1;

    m0["xxx"] = create_xxx();
    m0["yyy"] = create_yyy();
    m0["zzz"] = create_zzz(); 
    s0.xxx = create_xxx();
    s0.yyy = create_yyy();
    s0.zzz = create_zzz();

    std::cout << "\nwithout macro";
    std::cout << "\nmap[" << "xxx" << "] = " << std::get<0>(m0["xxx"]);
    std::cout << "\nmap[" << "yyy" << "] = " << std::get<1>(m0["yyy"]);
    std::cout << "\nmap[" << "zzz" << "] = " << std::get<2>(m0["zzz"]);
    std::cout << "\nobj." << "xxx" << "  = " << s0.xxx;
    std::cout << "\nobj." << "yyy" << "  = " << s0.yyy;
    std::cout << "\nobj." << "zzz" << "  = " << s0.zzz;
    std::cout << "\n";

    fill_map(m1, xxx);
    fill_map(m1, yyy);
    fill_map(m1, zzz);
    fill_struct(s1, xxx);
    fill_struct(s1, yyy);
    fill_struct(s1, zzz);

    std::cout << "\nwith macro";
    std::cout << "\nmap[" << "xxx" << "] = " << std::get<0>(m1["xxx"]);
    std::cout << "\nmap[" << "yyy" << "] = " << std::get<1>(m1["yyy"]);
    std::cout << "\nmap[" << "zzz" << "] = " << std::get<2>(m1["zzz"]);
    std::cout << "\nobj." << "xxx" << "  = " << s1.xxx;
    std::cout << "\nobj." << "yyy" << "  = " << s1.yyy;
    std::cout << "\nobj." << "zzz" << "  = " << s1.zzz;
    std::cout << "\n";
}

