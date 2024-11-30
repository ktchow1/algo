// ********************** //
// *** Longest string *** //
// ********************** //
namespace hackerrank
{
    // As oppose to longest_target_subseq_sum
    // in which we use a map to store the first index of a specific sum
    // in here we use a map to store the latest index of a specific char
    int longest_non_duplicating_substr(const std::string& str)
    {
        std::unordered_map<char,int> index;
        int ans = 0;
        int n = 0;
        for(auto i = str.begin(); i!= str.end(); ++i, ++n)
        {
            if (auto iter = index.find(*i); iter != index.end())
            {
                ans = std::max(ans, n-iter->second);
            }
            index[*i] = n;
        }
        return ans;
    }
   
}

// ******************* //
// *** Stack trick *** // 
// ******************* //
// use std::vector<int> as input for this section
namespace hackerrank
{
    int min_num_of_stroke(const std::vector<int>& input)
    {
        int count = 0;
        std::stack<int> stack;

        for(const auto iter = input.begin(); iter != input.end(); ++iter)
        {
            if (stack.empty())
            {
                count += *iter; // easy to miss this
                stack.push(*iter);
            }
            else if (stack.top() < *iter)
            {
                count += *iter - stack.top();
                stack.push(*iter);
            }
            else if (stack.top() > *iter)
            {
                while (stack.top() > *iter) stack.pop();
            //  no need to push *iter
            }
        //  else no need to push *iter
        } 
        return count;
    }

    int min_length_of_unsorted_seq_by_sorting(const std::vector<int>& input)
    {
        std::vector<int> aux = input;
        std::sort(aux.begin(), aux.end()); // O(NlogN)

        // Forward
        const auto i_iter = input.begin();
        const auto a_iter = aux.begin(); 
        for(; i_iter != input.end(); ++i_iter, ++a_iter)
        {
            if (*i_iter != *a_iter) break;
        }
        if (i_iter == input.end()) return 0;

        // Backward
        const auto i_riter = input.rbegin();
        const auto a_riter = aux.rbegin(); 
        for(; i_riter != input.rend(); ++i_riter, ++a_riter)
        {
            if (*i_riter != *a_riter) break;
        }
        if (i_riter == input.rend()) return 0; // should exit previously, imply some errors

        return input.size() - std::distance(i_iter, input.end()) 
                            - std::distance(i_riter, input.rend());
    }

    int min_length_of_unsorted_seq_by_stack(const std::vector<int>& input)
    {
        std::stack<int> stack0;
        std::stack<int> stack1;

        // Forward
        bool popped0 = false;
        for(const auto iter = input.begin(); iter != input.end(); ++iter)
        {
            while(!stack0.empty() && stack0.top() < *iter)
            { 
                stack0.pop(); 
                popped0 = true; 

            }
            if (!popped0) stack0.push(*iter);
        }
        if (!popped0) return 0;

        // Backward
        bool popped1 = false;
        for(const auto riter = input.rbegin(); riter != input.rend(); ++riter)
        {
            while(!stack1.empty() && stack1.top() < *riter) 
            { 
                stack1.pop();
                popped1 = true; 
            } 
            if (!popped1) stack1.push(*iter);
        }
        if (!popped1) return 0; // should exit previously, imply some errors

        return input.size() - stack0.size() - stack1.size();
    }

    // Push-pop pattern is similar to previous question 
    int max_rectangle_in_histogram(const std::vector<int>& hist) 
    {
        std::stack<int> stack; // only store index
        int max_area = 0;

        for(int n=0; n!=input.size(); ++n)
        {
            while(!stack.empty() && hist[stack.top()] > hist[n])
            {
                int h = stack.top().first;
                int w;
                stack.pop();

                if (!stack.empty()) 
                     w = (n-1) - (stack.top()+1) + 1;
                else w = (n-1) - 0 + 1;
                max_area = std::max(max_area, h*w);
            }
            stack.push(input[n]);
        } 

        // clear the stack with same logic
        while(!stack.empty()) 
        {
            int h = stack.top().first;
            int w;
            stack.pop();
            
            if (!stack.empty()) 
                 w = (hist.size()-1) - (stack.top()+1) + 1;
            else w = (hist.size()-1) - 0 + 1;
            max_area = std::max(max_area, h*w);
        }
        return max_area;
    }

    // No stack trick needed
    int area_trapped_in_histogram(const std::vector<int>& hist)
    {
        std::vector<int> profile0(0, hist.size());
        std::vector<int> profile1(0, hist.size());

        int max0 = 0;
        for(int n=0; n!=hist.size(); ++n)
        {
            if (max0 < hist[n]) max0 = hist[n];
            profile0[n] = max0;
        }
        
        int max1 = 0;
        for(int n=hist.size()-1; n>=0; --n)
        {
            if (max1 < hist[n]) max1 = hist[n];
            profile1[n] = max1;
        }

        int ans = 0;
        for(int n=0; n!=hist.size(); ++n)
        {
            ans += std::min(profile0[n], profile1[n]);
        }
        return ans;
    }
}
