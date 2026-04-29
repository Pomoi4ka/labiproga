#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

struct Solution {
    struct Stock : List<Denom> {
        Stock();
        Stock(List<Denom> const &);
        Denom popLeft();
        void transfer_from(Stock &other);

        // different api, but to be able to
        // track it has to be like that
        void pushLeft(Denom);
        size_t avail() const;
    private:
        size_t totalAvailable;
    };
    typedef List<Denom> Payout;
    List<Payout> solution;
    Stock finalStock;

    Solution(File const &file);
private:
    struct Branch;

    typedef List<Branch> Branches;
    typedef List<size_t> Agents;
    typedef Agents::ConstNode AgentNode;

    static size_t count(Payout const&);
    static Branches generatePayouts(long amount, Stock stock);
    size_t solve(Stock &stock, AgentNode agents, List<Payout> &result);
};

typedef List<Solution::Payout>::ConstNode FinalPayout;

struct Solution::Branch {
    Payout payout;
    Stock stock;
};

#endif // SOLUTION_HPP_
