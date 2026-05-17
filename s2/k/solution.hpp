#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

struct Solution {
    Solution(File const &file);
    void write(std::ostream &);
    void writeProtocol(std::ostream &);
    bool operator!() const;
private:
    struct Stock;
    struct Branch;
    struct SolveResult;

    typedef List<Branch> Branches;
    typedef List<size_t> Agents;
    typedef Agents::ConstNode AgentNode;
    typedef List<Denom> Payout;

    List<Payout> solution;
    List<Denom> remainder;
    File const &file;

    static size_t count(Payout const&);
    static Branches generatePayouts(long amount, Stock stock);
    static SolveResult solve(Stock &stock, AgentNode agents, List<Payout> &result);
    Denom &find(size_t);
};

#endif // SOLUTION_HPP_
