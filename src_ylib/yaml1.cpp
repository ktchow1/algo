#include<iostream>
#include<fstream>
#include<string>
#include<cstring> // for std::memcpy
#include<yaml-cpp/yaml.h>


// *********************************************** //
// *** Make YAML to understand customized type *** //
// *********************************************** //
void test_yaml2()
{
    // ****************** //
    // *** Write yaml *** //
    // ****************** //
    {
        YAML::Node node;

        node["key0"]["AAA"].push_back("value00");
        node["key0"]["BBB"].push_back("value01");
        node["key0"]["BBB"].push_back("value02");
        node["key0"]["CCC"] = "value03";

        node["key1"]["AAA"].push_back("value10");
        node["key1"]["AAA"].push_back("value11");
        node["key1"]["BBB"].push_back("value12");
        node["key1"]["CCC"].push_back("       ");

        node["key2"]["AAA"].push_back("value20");
        node["key2"]["BBB"].push_back("value21");
        node["key2"]["CCC"].push_back("value22");
        node["key2"]["CCC"].push_back("value23");
        node["key2"]["CCC"].push_back("value24");

        std::ofstream ofs("temp.yaml");
        ofs << node;
    }

    // ***************** //
    // *** Read yaml *** //
    // ***************** //
    {
        YAML::Node node = YAML::LoadFile("temp.yaml");

        std::cout << "\n\nChecking";
        const YAML::Node& nA = node["key0"]["AAA"];
        const YAML::Node& nC = node["key0"]["CCC"];
        const YAML::Node& nE = node["XYZW"]["123"]["XXX"];
        if (nA) 
        {
            std::cout << "\nnode key0::AAA exists"; 
            std::cout << "\nnode key0::AAA IsScalar() = " << nA.IsScalar();
            std::cout << "\nnode key0::AAA length() = "   << nA.Scalar().length(); // get byte array
        }
        if (nC) 
        {
            std::cout << "\nnode key0::CCC exists"; 
            std::cout << "\nnode key0::CCC IsScalar() = " << nC.IsScalar();
            std::cout << "\nnode key0::CCC length() = "   << nC.Scalar().length(); // get byte array
        }
        if (nE) 
        {
            std::cout << "\nnode random exists"; 
        }
        else
        {
            std::cout << "\nnode random does not exist"; 
        }
        std::cout << "\n";

        YAML::Node node1 = node["key1"];
        std::string strAAA = "AAA";
        if (node1[strAAA]) std::cout << "\nnode key1::AAA [" << node1["AAA"][0].as<std::string>() << "]"; // consider the 1st entry only
        else               std::cout << "\nnode key1::AAA does not exist";
        if (node1["BBB"])  std::cout << "\nnode key1::BBB [" << node1["BBB"][0].as<std::string>() << "]"; 
        else               std::cout << "\nnode key1::BBB does not exist";
        if (node1["CCC"])  std::cout << "\nnode key1::CCC [" << node1["CCC"][0].as<std::string>() << "]";
        else               std::cout << "\nnode key1::CCC does not exist";
        if (node1["DDD"])  std::cout << "\nnode key1::DDD [" << node1["DDD"][0].as<std::string>() << "]";
        else               std::cout << "\nnode key1::DDD does not exist";
        std::cout << "\n";

        char ac[10];
        std::cout << "\nis_same = " << std::is_same<char[10], decltype(ac)>::value;
        std::cout << "\nis_same = " << std::is_same<char[11], decltype(ac)>::value;
        std::cout << "\nis_same = " << std::is_same<char[12], decltype(ac)>::value;
        std::cout << "\n";
    }
}



