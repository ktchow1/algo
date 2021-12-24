#include<iostream>
#include<string>
#include<libxml/parser.h>
#include<libxml/xmlmemory.h>

class xml_iterator;
class xml
{
public:
    xml() : doc_ptr(nullptr) 
    {
    }

    xml(const std::string& filename) : doc_ptr(nullptr)
    {

        doc_ptr = xmlParseFile(filename.c_str());
        if (doc_ptr == nullptr)
        {
            throw std::runtime_error("cannot open xml file");
        }


    }

   ~xml()
    {
        if (doc_ptr != nullptr)
        {
            xmlFreeDoc(doc_ptr);
        }
    }

    xml(const xml&) = default;
    xml(xml&&) = default;
    xml& operator=(const xml&) = default;
    xml& operator=(xml&&) = default;


public:
    xml_iterator begin() const;

private:
    xmlDocPtr doc_ptr;
};

class xml_iterator
{
public:
    friend class xml;

    xml_iterator() : doc_ptr(nullptr), node_ptr(nullptr)
    {
    }

   ~xml_iterator() = default;
    xml_iterator(const xml_iterator&) = default;
    xml_iterator(xml_iterator&&) = default;
    xml_iterator& operator=(const xml_iterator&) = default;
    xml_iterator& operator=(xml_iterator&&) = default;

public:
    bool operator==(const xml_iterator& rhs)
    {
        return !(*this!=rhs);
    }

    bool operator!=(const xml_iterator& rhs)
    {
        if (doc_ptr != rhs.doc_ptr) return true;
        if (node_ptr != rhs.node_ptr) return true;
        return false;
    }

    xml_iterator& operator++() 
    {
        node_ptr = node_ptr->next;
        return *this;
    }

private:
    xmlDocPtr doc_ptr;
    xmlNodePtr node_ptr;
};

xml_iterator xml::begin() const
{
    auto node_ptr = xmlDocGetRootElement(doc_ptr);

    if (node_ptr != nullptr)
    {
        xml_iterator iter;
        iter.doc_ptr = doc_ptr;
        iter.node_ptr = node_ptr;
        return iter;
    }
    else
    {
        return xml_iterator{};
    }
}



void test_xml()
{
    std::cout << "\nTest xml with include and library";


    xml x;

}

