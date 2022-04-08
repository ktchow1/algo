#include<iostream>
#include<fstream>
#include<string>

void test_fileio()
{
    std::cout << "\ntest file-io" << std::flush;

    // **************** //
    // **** Writing *** //
    // **************** //
    {
        std::ofstream ofs("temp.txt");
        for(std::uint32_t n=0; n!=40; ++n)
        {
            ofs << n << " : ";
            for(std::uint32_t m=0; m!=n+10; ++m)
            {
                ofs << (char)('A' + m%26);
            }
            ofs << "\n";
        }
    }

    // **************** //
    // **** Reading *** //
    // **************** //
    std::cout << "\n\n[debug A]\n";
    {
        std::ifstream ifs("temp.txt");
        while(ifs)
        {
            std::string str;
            std::getline(ifs, str);
            std::cout << str << "\n";
        }
    }

    std::cout << "\n\n[debug B]\n";
    {
        std::ifstream ifs("temp.txt");
        bool read_flag = true;
        while(ifs && read_flag) // Note : readsome does not set eof flag
        {
            char buf[32];
            auto n = ifs.readsome(buf, 32); 
            std::cout << std::string(buf, n);
            if (n==0) read_flag = false;
        }
    }

    std::cout << "\n\n[debug C]\n";
    {
        std::ifstream ifs("data/wsdali.wsdl");
        bool read_flag = true;
        while(ifs && read_flag) // Note : readsome does not set eof flag
        {
            char buf[32];
            auto n = ifs.readsome(buf, 32); 
            std::cout << std::string(buf, n);
            if (n==0) read_flag = false;
        }
    }
    std::cout << "\n\n";
}
