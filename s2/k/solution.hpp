#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

struct Solution {
    typedef List<Denom> Payout;
    List<Payout> solution;
    List<Denom> remainder;

    Solution(File const &file);
    void write(std::ostream &);
    void writeProtocol(std::ostream &);
    bool operator!() const;
private:
    File const &file;
    size_t minUsed;

    struct Stock;
    struct Branch;

    typedef List<Branch> Branches;
    typedef List<size_t> Agents;
    typedef Agents::ConstNode AgentNode;

    static size_t count(Payout const&);
    Branches generatePayouts(long amount, Stock stock);
    size_t solve(Stock &stock, AgentNode agents, List<Payout> &result);
    Denom &find(size_t);
};

typedef List<Solution::Payout>::ConstNode FinalPayout;

struct Solution::Stock : List<Denom> {
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

struct Solution::Branch {
    Payout payout;
    Stock stock;
};

#endif // SOLUTION_HPP_
