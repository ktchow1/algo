/*


std::uint32_t knapsack_optimized(std::unordered_map<std::uint32_t, std::uint32_t>& items, std::uint32_t limit)
{
    std::unordered_map<std::uint32_t, std::uint32_t> states; 
    states[0] = 0;

    std::queue<std::uint32_t> q;
    q.push(0);

    while(!q.empty())
    {
        std::uint32_t weight = q.front();
        std::uint32_t value  = states[weight];
        q.pop();
        
        for(const auto& item:items)
        {
            auto new_weight = weight + item.first;
            auto new_value  = value  + item.second; 

            if (euler_update<optimization::max>(states, new_weight, new_value) && new_weight < limit) // <--- change 3 (min becomes max)
            {
                q.push(new_weight);
            }
        }
    }

    std::uint32_t ans = 0;
    for(const auto& x:states) 
    {
        if (ans < x.second) ans = x.second;
    }
    return ans;
}   

auto equal_partition(const std::vector<std::uint32_t>& vec) 
{
    std::unordered_map<std::uint32_t, std::vector<std::uint32_t>> map; // sum and subset-giving-the-sum
    map[0] = std::vector<std::uint32_t>{};

    for(const auto& x:vec)
    {
        std::unordered_map<std::uint32_t, std::vector<std::uint32_t>> new_map = map;
        for(auto& y:map)
        {
            std::uint32_t sum = x + y.first;
            if (auto iter = new_map.find(sum); iter!=new_map.end())
            {
                // do nothing
            }
            else
            {
                y.second.push_back(x);
                new_map[sum] = std::move(y.second);
            }
        }
        map = std::move(new_map);
    }

    // Find nearest sum to half of sum(vec)
    double half_sum_vec = 0;
    for(const auto& x:vec) 
    {
        half_sum_vec += x;
    }
    half_sum_vec /= 2;

    double min_dist = std::numeric_limits<double>::max();
    auto min_iter=map.begin();
    for(auto iter=map.begin(); iter!=map.end(); ++iter)
    {
        double dist = fabs(iter->first - half_sum_vec);
        if (min_dist > dist)
        {
            min_dist = dist;
            min_iter = iter;
        }
    }
    return *min_iter; // This is a pair, 1st item is partition sum, 2nd item is vector.
}


*/
