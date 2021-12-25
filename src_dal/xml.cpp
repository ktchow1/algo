#include<iostream>
#include<sstream>
#include<string>
#include<stack>
#include<libxml/parser.h>
#include<libxml/xmlmemory.h>
#include<libxml/xpath.h>
// ******************************************************************************* //
// XML element include :
// 1. start tag
// 2. end tag
// 3. mixture of : attribute / text / sub-elements
//
// Given XML node :
// node_ptr->next            returns sibling
// node_ptr->xmlChildrenNode returns children
// ******************************************************************************* //
// XML DOM   = document object model (i.e. XML tree structure)
// XML XPath = expression used in XQuery (like regular expression in regex query)
// ******************************************************************************* //


// There are a lot of space characters from xmlNodeListGetString(), hence beautify it.
inline std::string get_string_from_xml_node(const xmlDocPtr& doc_ptr, 
                                            const xmlNodePtr& node_ptr)
{
    if (doc_ptr == nullptr) return "";
    if (node_ptr == nullptr) return "";

    const xmlChar* pc = xmlNodeListGetString(doc_ptr, node_ptr, 1);
    std::string str((const char*)pc);
    
    size_t pos0 = str.find_first_not_of("\n\r\t\f\v ");
    size_t pos1 = str.find_last_not_of("\n\r\t\f\v ");

    if (pos0 == std::string::npos) return "";
    if (pos1 == std::string::npos) return "";
    return str.substr(pos0, pos1-pos0+1);
}

void print_node(const std::string& tag, 
                const xmlDocPtr& doc_ptr,   // caller ensures this is non-null
                const xmlNodePtr& node_ptr) // caller ensures this is non-null
{
    std::string key((const char*)(node_ptr->name));
    if (key == "text")
    {
        std::string value = get_string_from_xml_node(doc_ptr, node_ptr);
    
        // 1. Extra text-element in between 2 sub-elements (Why it happens?)
        if (value == "")
        {
            // Skip it. 
        }
        // 2. Leaf text-element
        else
        {
            std::cout << tag << key << " = " << value;
        }
    }
    // 3. Non-leaf element
    else
    {
        std::cout << tag << key;
    }
}

void test_xml0()
{
    std::cout << "\nTest xml0";
    std::string filename0("../../data/test.xml");
    std::string filename1("../../data/test_out.xml");

    // *** Get doc *** //
    xmlDocPtr doc_ptr = xmlParseFile(filename0.c_str());
    if (doc_ptr == nullptr) 
    {
        std::cout << "\ndoc_ptr : null"; 
        return;
    }

    // *** Get root node *** //
    xmlNodePtr node_ptr0 = xmlDocGetRootElement(doc_ptr);
    if (node_ptr0 == nullptr)
    {
        std::cout << "\nnode_ptr0 : null";
        xmlFreeDoc(doc_ptr);
        return;
    }
    print_node("\nnode_ptr0 : ", doc_ptr, node_ptr0);

    // Remark 1 : libxml2 supports single root only.
    // Remark 2 : Lets have multi-layer node1_ptr.
    auto node_ptr1 = node_ptr0->xmlChildrenNode; 
    while(node_ptr1 != nullptr)
    {
        print_node("\n  node_ptr1 : ", doc_ptr, node_ptr1);

        auto node_ptr2 = node_ptr1->xmlChildrenNode;
        while(node_ptr2 != nullptr)
        {
            print_node("\n    node_ptr2 : ", doc_ptr, node_ptr2); 
        
            auto node_ptr3 = node_ptr2->xmlChildrenNode;
            while(node_ptr3 != nullptr)
            {   
                std::cout << "\n      node_ptr3 : " 
                          << node_ptr3->name << " = " 
                          << get_string_from_xml_node(doc_ptr, node_ptr3);

                node_ptr3 = node_ptr3->next;
            }        
            node_ptr2 = node_ptr2->next;
        }
        node_ptr1 = node_ptr1->next;
    }

    xmlSaveFormatFile(filename1.c_str(), doc_ptr, 1);
    xmlFreeDoc(doc_ptr);
}

// ******************* //
// *** Using XPath *** //
// ******************* //
void test_xml1()
{
    std::cout << "\nTest xml1";
    std::string filename("../../data/test.xml");
    std::string xpath("/book/body/para1");

    // *** Get doc *** //
    xmlDocPtr doc_ptr = xmlParseFile(filename.c_str());
    if (doc_ptr == nullptr) 
    {
        std::cout << "\ndoc_ptr is null"; 
        return;
    }

    // *** Get context *** //
    xmlXPathContextPtr context_ptr = xmlXPathNewContext(doc_ptr);
    if (context_ptr == nullptr)
    {
        std::cout << "\ncontext_ptr is null";
        xmlFreeDoc(doc_ptr);
        return;
    }

    // *** Evaulate XPath *** //
    xmlXPathObjectPtr result_ptr = xmlXPathEvalExpression((const xmlChar*)(xpath.c_str()), context_ptr);
    if (result_ptr == nullptr)
    {
        std::cout << "\nresult_ptr is null";
        xmlXPathFreeContext(context_ptr);
        xmlFreeDoc(doc_ptr);
        return;
    }

    // *** Iterate result *** //
    xmlNodeSetPtr nodeset_ptr = result_ptr->nodesetval;
    std::cout << "\nresult number = " << nodeset_ptr->nodeNr;

    for(int n=0; n!=nodeset_ptr->nodeNr; ++n)
    {
        std::stringstream ss;
        ss << "\nresult " << n << " : ";

        auto node_ptr = nodeset_ptr->nodeTab[n]->xmlChildrenNode;
        print_node(ss.str(), doc_ptr, node_ptr);
    }

    // *** Cleanup *** //
    xmlXPathFreeContext(context_ptr);
    xmlFreeDoc(doc_ptr);
}

/*
// ************************* //
// *** XML wrapper class *** //
// ************************* //
class xml_iterator;
class xml
{
public:
    xml() : doc_ptr(nullptr) 
    {
    }

    explicit xml(const std::string& filename) : doc_ptr(nullptr)
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

*/

