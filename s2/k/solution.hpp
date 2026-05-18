#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

#include "queue.hpp"

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
    Queue<String, 1000> protLines;

    static size_t count(Payout const&);
    static Branches generatePayouts(long amount, Stock stock);
    SolveResult solve(Stock &stock, AgentNode agents, List<Payout> &result);
    Denom &find(size_t);

    inline Solution& operator<<(const char *);
    inline Solution& operator<<(size_t);
    inline Solution& operator<<(Branch const &);
    inline Solution& operator<<(Payout const &);
    inline Solution& operator<<(std::ostream &(&)(std::ostream&));
};

#endif // SOLUTION_HPP_
