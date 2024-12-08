#include<iostream>
#include<string>
#include<array>
#include<vector>
#include<unordered_map>
#include<algorithm>


// ******************** //
// *** Maximization *** //
// ******************** //
namespace alg
{
    std::uint32_t longest_non_duplicated_substr(const std::string& str)
    {
        if (str.size()==0) return 0;
        std::unordered_map<char, std::uint32_t> index; 

        std::int32_t sub = 0;
        std::int32_t ans = sub;
        for(std::uint32_t n=0; n!=str.size(); ++n)
        {
            if (auto iter=index.find(str[n]); iter!=index.end())
            {
                if (iter->second < n-sub)
                {
                    sub = sub + 1;
                    ans = std::max(ans, sub);
                    iter->second = n;
                }
                else
                {
                    sub = n - iter->second;
                    ans = std::max(ans, sub);
                    iter->second = n;
                }
            }
            else
            {
                sub = sub + 1;
                ans = std::max(ans, sub);
                index[str[n]] = n;
            }
        }
        return ans;
    }


    // ********************************************************************************************************* //
    // Even palindrome can be converted into odd palindrome by
    // * inserting char '_' in front of input string
    // * inserting char '_' at end of input string
    // * inserting char '_' in between any characters in input string
    // Now only consider odd palindrome-in-palindrome. 
    //
    // Given parent-palindrome-radius = 5, there are 3 cases :                 0123456789012
    // 1. when LHS sub-palindrome is completely inside parent-palindrome, like M*ABA.T.ABA*N
    // 2. when LHS sub-palindrome   barely lies inside parent-palindrome, like M.ABA.T.ABA.T 
    // 3. when LHS sub-palindrome     extrudes outside parent-palindrome, like T.ABA.T.ABA.N
    //
    // we can copy LHS-palindrome-radius to RHS-palindrome-radius for position [7-11] : 
    //
    // 1. M*ABA.T.ABA*N     when 9 + rad[3] < 6 + rad[6] 
    //     001005~~x~~      x = rad[9] = rad[3] = 1, no further growing
    //
    // 2. M.ABA.T.ABA.T     when 9 + rad[3] = 6 + rad[6] 
    //     002005~~y~~      y = rad[9] = rad[3] = 2, with further growing
    //
    // 3. T.ABA.T.ABA.N     when 9 + rad[3] > 6 + rad[6] 
    //     003005~~z~~      z = rad[9] = min(rad[3],11-9) = 2, no further growing, N wont be T, or rad[6] = 6
    //   
    //    ~ = rad[6+n] = rad[6-n] = 0, no further growing
    //    where n = 1,2,4,5
    // ********************************************************************************************************* //
    //
    std::uint32_t longest_odd_palindrome_substr(const std::string& str) 
    {
        std::uint32_t ans=0;
        for(std::uint32_t n=0; n!=str.size()-1; ++n) // centre of palindrome
        {

        }
        return ans;
    } 
}



// *********************************************** //
// *** Maximization - benchmark implementation *** //
// *********************************************** //
namespace alg
{
    std::uint32_t longest_non_duplicated_substr_bmk(const std::string& str)
    {
        std::uint32_t ans = 0;
        for(std::uint32_t n=0; n!=str.size(); ++n) // consider substr starting with n, and reaches as far as possible 
        {
            std::array<char,26> hist;
            hist.fill(false);

            for(std::uint32_t m=n; m!=str.size(); ++m) 
            {
                if (!hist[str[m]-'a']) 
                {
                    hist[str[m]-'a'] = true;
                    ans = std::max(ans,m-n+1);
                }
                else break;
            }
        }
        return ans;
    }
    
    std::uint32_t longest_odd_palindrome_substr_bmk(const std::string& str) 
    {
        std::uint32_t max_rad = 0;                 // radius of palindrome
        for(std::uint32_t n=0; n!=str.size(); ++n) // centre of palindrome
        {
            for(std::uint32_t rad=1; n>=rad && n+rad<str.size(); ++rad)
            {
                if (str[n-rad] == str[n+rad])
                {
                    max_rad = std::max(max_rad, rad);
                }
                else break;
            }
        }
        return 2 * max_rad + 1; // length of palindrome
    }
}



