// *************** //
// *** Sorting *** //
// *************** //
namespace hackerrank
{
    int find_kth_element(const std::vector<int>& input, int k) // in O(N)
    {
        if (input.size() == 0) return -1;
        if (input.size() == 1) return 0;

        int begin = 0;
        int last = input.size()-1;
        std::vector<int> sorted_input(input);

        while (begin != last)
        {
            // quick sort (with i, j, k)
            int i = begin;
            int j = last;
            while (i != j)
            {
                if (sorted_input[i] < sorted_input[j])
                {
                    --j;
                }
                else
                {
                    int temp = sorted_input[i];
                    sorted_input[i] = sorted_input[j];
                    sorted_input[j] = sorted_input[i+1];
                    sorted_input[i+1] = temp;
                    ++i;
                }
            }
            
            // bisection (with begin, last)
            if      (k == i)   return sorted_input[k];
            else if (k <  i)   last  = i;
            else               begin = i;
        }
        return sorted_input[begin];
    }

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

