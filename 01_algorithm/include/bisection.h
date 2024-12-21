#pragma once
#include<vector>
#include<functional>
#include<optional>

namespace alg
{
    std::optional<double> bisection(const std::function<double(double)>& fct, double x0, double x1)
    {
        // 1. check edge case (or init y in here)
        double y0 = fct(x0);
        double y1 = fct(x1);

        // 2. check solution existence
        if (y0 * y1 > 0) return std::nullopt;

        // 3. check stop condition
        while(fabs(x0 - x1) > 0.000000001)
        {
            // 4. mid point
            double xm = (x0 + x1) / 2;
            double ym = fct(xm);
            
            // 5. bisection
            if (y0 * ym > 0)
            {
                x0 = xm;
                y0 = ym;
            }
            else 
            {
                x1 = xm;
                y1 = ym;
            }
        }            

        // 6. answer
        return std::make_optional((x0 + x1) / 2);
    }
}

namespace alg
{
    std::optional<std::size_t> bisection(const std::vector<std::int32_t>& vec, std::int32_t target)
    {
        return std::nullopt;
    }

    std::optional<std::size_t> bisection_impl(const std::vector<std::int32_t>& vec, std::int32_t target, std::size_t x0, std::size_t x1)
    {
        return std::nullopt;
    }
}

namespace alg
{
    std::optional<std::size_t> mode_bisection(const std::vector<std::int32_t>& vec)
    {
        return std::nullopt;
    }
}

namespace alg
{
    std::optional<std::size_t> rotated_bisection(const std::vector<std::int32_t>& vec, std::int32_t target)
    {
        return std::nullopt;
    }
}

