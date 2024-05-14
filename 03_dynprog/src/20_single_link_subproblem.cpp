#include<iostream>
#include<vector>
#include<string>
#include<02_dynprog.h>

std::string longest_common_subseq(const std::string& lhs, const std::string& rhs)
{
    matrix<std::uint32_t> mat(lhs.size(), rhs.size());

    // ******************* //
    // *** Fill matrix *** //
    // ******************* //
    mat(0,0) = (lhs[0]==rhs[0]? 1:0);

    for(std::uint32_t y=1; y!=lhs.size(); ++y)
    {
        if (lhs[y]==rhs[0])  mat(y,0) = 1;
        else                 mat(y,0) = mat(y-1,0);
    }
    for(std::uint32_t x=1; x!=rhs.size(); ++x)
    {
        if (rhs[x]==lhs[0])  mat(0,x) = 1;    
        else                 mat(0,x) = mat(0,x-1);  
    }
    for(std::uint32_t y=1; y!=lhs.size(); ++y)
    {
        for(std::uint32_t x=1; x!=rhs.size(); ++x)
        {
            if (lhs[y]==rhs[x])
            {
                mat(y,x) = mat(y-1,x-1)+1;
            }
            else
            {
                mat(y,x) = std::max(mat(y-1,x), mat(y,x-1));
            }
        }
    }  

    // ******************************************** //
    // *** Backtrace (beware in defining cases) *** //
    // ******************************************** //
    std::string ans;
    std::uint32_t y=mat.size_y-1;
    std::uint32_t x=mat.size_x-1;
    while(y!=0 || x!=0)
    {
        // case 1 : move in direction with same value
        if (y>0 && mat(y,x) == mat(y-1,x))
        {
            --y;
        }
        else if (x>0 && mat(y,x) == mat(y,x-1))
        {
            --x;
        }
        // case 2 : move in direction with dropped value
        else 
        {
            if (y>0 && x>0)
            {
                ans.push_back(lhs[y]); // or equivalently rhs[m]
                --y;
                --x;
            }
            else if (y>0) // i.e. x==0 
            {
                ans.push_back(lhs[y]); 
                --y; 
            }
            else // i.e. x==0
            {
                ans.push_back(rhs[x]); 
                --x; 
            }
        }
    }
    if (mat(0,0)==1) ans.push_back(lhs[0]);
   
    // *************** //
    // *** Reverse *** //
    // *************** //
    std::uint32_t i=0;
    std::uint32_t j=ans.size()-1;
    while(i<j) 
    {
        std::swap(ans[i],ans[j]); 
        ++i;
        --j;
    }
    return ans;
}

// Edit distance from lhs to rhs
std::uint32_t edit_distance(const std::string& lhs, const std::string& rhs)
{
    matrix<std::uint32_t> mat(lhs.size(), rhs.size());

    // ******************* //
    // *** Fill matrix *** //
    // ******************* //
    mat(0,0) = (lhs[0]==rhs[0]? 0:1);
  
    for(std::uint32_t y=1; y!=lhs.size(); ++y)
    {
        if (lhs[y]==rhs[0])  mat(y,0) = y;
        else                 mat(y,0) = mat(y-1,0)+1;
    }
    for(std::uint32_t x=1; x!=rhs.size(); ++x)
    {
        if (rhs[x]==lhs[0])  mat(0,x) = x;    
        else                 mat(0,x) = mat(0,x-1)+1;  
    }
    for(std::uint32_t y=1; y!=lhs.size(); ++y)
    {
        for(std::uint32_t x=1; x!=rhs.size(); ++x)
        {
            if (lhs[y]==rhs[x])
            {
                mat(y,x) = mat(y-1,x-1);
            }
            else
            {
                mat(y,x) = std::min(mat(y,x-1)+1,   // insert rhs[x]
                           std::min(mat(y-1,x)+1,   // delete lhs[y]
                                    mat(y-1,x-1)+1)); // modify lhs[y] into rhs[x]
            }
        }
    } 
    return mat(lhs.size()-1, rhs.size()-1);
}


// ************ //
// *** Test *** //
// ************ //
void test_longest_common_subseq()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        std::string lhs("ABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJ");
        std::string rhs(lhs);
        std::uint32_t num_insert = rand()%15; 
        std::uint32_t num_delete = rand()%15;
        std::uint32_t num_modify = rand()%15;

        for(std::uint32_t n=0; n!=num_insert; ++n) rhs.insert(rand()%rhs.size(), "*");
        for(std::uint32_t n=0; n!=num_delete; ++n) rhs.erase(rhs.begin() + rand()%rhs.size());
        for(std::uint32_t n=0; n!=num_modify; ++n) rhs[rand()%rhs.size()] = '-';

        std::cout << "\nlhs = " << lhs;
        std::cout << "\nrhs = " << rhs;
        std::cout << "\nans = " << longest_common_subseq(lhs,rhs);
        std::cout << "\nedit distance = " << num_insert + num_delete + num_modify << " " << edit_distance(lhs, rhs);
    }
}

void test_singly_link_subproblem()
{
    test_longest_common_subseq();
}


