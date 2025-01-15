#include<iostream>
#include<vector>
#include<string>
#include<02_dynprog.h>

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


