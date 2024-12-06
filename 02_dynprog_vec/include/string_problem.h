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

/*    
    std::uint32_t longest_palindrome_substr(const std::string& str) 
    {
        std::uint32_t ans=1;
        for(std::uint32_t n=1; n!=str.size()-1; ++n) 
        {
            // case 1 : when str[n] is the mirror
            std::uint32_t m=1;
            while(true)
            {
                if (n<m || n+m>=str.size()) 
                {
                    break;
                }
                else if (str[n-m]==str[n+m])
                {
                    ans = std::max(ans, 2*m+1);
                    ++m;
                }
                else
                {
                    break;
                }
            }

            // case 2 : when str[n-1] str[n] is the mirror
            // please implement ... 
        }
        return ans;
    } */
}



// *********************************************** //
// *** Maximization - benchmark implementation *** //
// *********************************************** //
namespace alg
{
    std::uint32_t longest_non_duplicated_substr_bmk(const std::string& str)
    {
        if (str.size()==0) return 0;

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
}



