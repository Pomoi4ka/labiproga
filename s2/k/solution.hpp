#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

struct Solution {
    typedef List<Denom> Payout;
    List<Payout> solution;

    Solution(File const &file);
private:
    typedef List<Denom> Stock;
    struct Branch { Payout payout; Stock stock; };

    typedef List<Branch> Branches;
    typedef List<size_t> Agents;
    typedef Agents::ConstNode AgentNode;

    static size_t count(Payout const&);
    static Branches generatePayouts(long amount, Stock stock);
    size_t solve(Stock &stock, AgentNode agents, List<Payout> &result);
};

typedef List<Solution::Payout>::ConstNode FinalPayout;

#endif // SOLUTION_HPP_
