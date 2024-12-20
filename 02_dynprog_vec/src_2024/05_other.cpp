// *************** //
// *** Sorting *** //
// *************** //
namespace hackerrank
{
    enum class num_of_swap_method
    {
        LL, UR, LL_OPTIMIZED
    };

    int num_of_swap(const std::vector<int>& orig_pos, num_of_swap_method method)
    {
        // checking
        for (int n=0; n!=orig_pos.size(); ++n)
        {
            if (n < orig_pos[n]-2) return -1; // incorrect input
        }

        // counting 
        int count = 0;
        if (method == LL)
        {
            for (int n=0; n!=orig_pos.size(); ++n)
            {
                for (int m=0; m!=n; ++m) // given that m < n
                {
                    if (orig_pos[m] > orig_pos[n]) ++count;
                }
            }   
        }
        else if (method == UR)
        {
            for (int n=0; n!=orig_pos.size(); ++n)
            {
                for (int m=n+1; m!=orig_pos.size(); ++m) // given that m > n
                {
                    if (orig_pos[m] < orig_pos[n]) ++count;
                }
            }   
        }
        else
        {
            for (int n=0; n!=orig_pos.size(); ++n)
            {
                for (int m=std::max(0, orig_pos[n]-2); m!=n; ++m) // given that m < n, optimized
                {
                    if (orig_pos[m] > orig_pos[n]) ++count;
                }
            }   
        }
    }
}

