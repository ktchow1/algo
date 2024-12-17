namespace hackerrank
{

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

}
