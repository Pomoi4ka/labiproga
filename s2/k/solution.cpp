#include <new>
#include <cstddef>
#include <ostream>

#include "list.hpp"
#include "string.hpp"
#include "file.hpp"
#include "solution.hpp"

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

struct Solution::SolveResult {
    size_t used;
    size_t distributed;

    inline SolveResult operator+(size_t used) const;
    inline bool operator<(SolveResult const &other) const;
    inline operator bool() const;
    inline bool operator!() const;
    inline SolveResult();
    inline SolveResult(size_t);
};

inline Solution& Solution::operator<<(const char *cstr)
{
    String &s = protLines.lastOrEnqueueDefault();
    while (*cstr) s.append(*cstr++);
    return *this;
}

inline Solution& Solution::operator<<(size_t number)
{
    char buf[32];
    int len = 0;

    String &s = protLines.lastOrEnqueueDefault();
    while (number) buf[len++] = number%10 + '0', number /= 10;
    while (len) s.append(buf[--len]);
    return *this;
}

inline Solution& Solution::operator<<(Payout const &payout)
{
    *this << "[";
    for (Payout::ConstNode node = payout.head();
         *node; node = node.next()) {
        *this << "<" << node->value
              << ": " << node->count
              << ">";
        if (*node.next())
            *this << ", ";
    }
    return *this << "]";
}

inline Solution& Solution::operator<<(Branch const &branch)
{
    return *this << "{ payout = " << branch.payout
                 << ", stock = " << branch.stock
                 << " }";
}

inline Solution& Solution::operator<<(std::ostream &(&op)(std::ostream&))
{
    assert(&op == &(std::ostream &(&)(std::ostream&))std::endl);

    String s;
    protLines.enqueue(s);
    return *this;
}

Solution::Branches
Solution::generatePayouts(long amount, Stock stock)
{
    Branches branches;
    if (amount <= 0) {
        branches.append()->stock.transfer_from(stock);
        return branches;
    }
    if (!stock.hasNext()) return branches;
    if ((long) stock.avail() < amount) return branches;
    Denom head = stock.popLeft();
    size_t d = head.value;
    size_t q = head.count;
    size_t maxUse = std::min(q, (amount + d - 1) / d);
    for (size_t k = 0; k <= maxUse; ++k) {
        Branches sub = generatePayouts(amount - k * d, stock);

        while (sub.hasNext()) {
            Branch *subBranch = sub.next();
            Branch *b = branches.append();
            b->payout.transfer_from(subBranch->payout);
            if (k > 0) {
                Denom *n = b->payout.pushLeft();
                n->value = d;
                n->count = k;
            }

            Stock &remaining = subBranch->stock;
            if (k < q) {
                Denom rem = {d, q - k};
                remaining.pushLeft(rem);
            }

            b->stock.transfer_from(subBranch->stock);
        }
    }
    return branches;
}

size_t Solution::count(Payout const &payout)
{
    size_t count = 0;
    for (Payout::ConstNode node = payout.head();
         *node; node = node.next())
        count += node->count;
    return count;
}

inline Solution::SolveResult::operator bool() const
{
    return used != ~0UL;
}

inline bool Solution::SolveResult::operator!() const
{
    return !this->operator bool();
}

inline bool Solution::SolveResult::operator<(SolveResult const &other) const
{
    bool distMore = distributed >= other.distributed;
    bool usedLess = used < other.used;
    return distMore && usedLess;
}

inline Solution::SolveResult Solution::SolveResult::operator+(size_t used) const
{
    SolveResult result;
    result.used = this->used + used;
    result.distributed = this->distributed;
    return result;
}

inline Solution::SolveResult::SolveResult(size_t used)
    : used(used)
    , distributed()
{}

inline Solution::SolveResult::SolveResult()
    : used(~0)
    , distributed()
{}

Solution::SolveResult Solution::solve(Stock &stock, AgentNode agents, List<Payout> &result)
{
    if (!*agents) return 0;
    size_t amount = **agents;
    AgentNode restAgents = agents.next();
    Branches branches = generatePayouts(amount, stock);
    SolveResult minCount;

    List<Payout> bestRestResult;
    Payout bestPayout;

    while (branches.hasNext()) {
        List<Payout> restResult;
        Branch *branch = branches.next();
        *this << "[" << amount << "] "
              << "Probing branch: " << *branch << std::endl;
        SolveResult subResult = solve(branch->stock, restAgents, restResult);
        int skip;
        AgentNode agents = restAgents;
        for (skip = 0; !subResult; ++skip) {
            *this << "[" << amount << "] "
                  << "Branch failed to distribute for agent sum "
                  << **agents << std::endl;
            agents = agents.next();
            if (*agents)
                *this << "[" << amount << "] "
                      << "Skipping to next agent sum "
                      << **agents << std::endl;
            subResult = solve(branch->stock, agents, restResult);
        }
        while (skip--) restResult.pushLeft();

        SolveResult used = subResult + count(branch->payout);
        used.distributed += 1;
        if (used < minCount) {
            *this << "[" << amount << "] "
                  << "Found new optimal payout "
                  << branch->payout << std::endl;
            minCount = used;
            bestPayout.transfer_from(branch->payout);
            bestRestResult.transfer_from(restResult);
        }
    }
    if (minCount) {
        result.transfer_from(bestRestResult);
        result.pushLeft()->transfer_from(bestPayout);
    }
    return minCount;
}

Solution::Solution(File const &file)
    : solution()
    , remainder(file.denoms)
    , file(file)
{
    Agents agents;
    Stock stock = file.denoms;
    stock.reset();

    for (List<Agent>::ConstNode agent = file.agents.head();
         *agent; agent = agent.next())
        *agents.append() = agent->sum();

    List<Payout> result;
    if (solve(stock, agents.head(), result))
        solution.transfer_from(result);

    List<Payout>::ConstNode fp = solution.head();
    for (; *fp; fp = fp.next()) {
        Payout::ConstNode d = fp->head();
        for (; *d; d = d.next()) find(d->value).count -= d->count;
    }
}

Solution::Stock::Stock()
    : List()
    , totalAvailable()
{}

Solution::Stock::Stock(List<Denom> const &other)
    : List()
    , totalAvailable()
{
    for (List<Denom>::ConstNode node = other.head();
         *node; node = node.next()) {
        totalAvailable += node->count * node->value;
        *List::append() = **node;
    }
}

Denom Solution::Stock::popLeft()
{
    Denom d = List::popLeft();
    totalAvailable -= d.value * d.count;
    return d;
}

void Solution::Stock::pushLeft(Denom d)
{
    *List::pushLeft() = d;
    totalAvailable += d.value * d.count;
}

size_t Solution::Stock::avail() const
{
    return totalAvailable;
}

void Solution::Stock::transfer_from(Stock &other)
{
    totalAvailable = other.totalAvailable;
    List::transfer_from(other);
}

Denom &Solution::find(size_t value)
{
    remainder.reset();
    for (Denom *d = remainder.next(); d; d = remainder.next())
        if (d->value == value) return *d;
    assert(0);
}

void Solution::write(std::ostream &strm)
{
    List<Agent>::ConstNode agent = file.agents.head();
    size_t total = 0;
    for (List<Payout>::ConstNode payout = solution.head(); *payout;
         payout = payout.next(), agent = agent.next()) {
        assert(*agent);
        strm << agent->name << " (sum " << agent->sum()
             << "):" << std::endl;
        for (Solution::Payout::ConstNode denom = payout->head();
             *denom; denom = denom.next()) {
            strm << "    " << denom->value << ": "
                 << denom->count << std::endl;
            total += denom->count;
        }
    }
    strm << "Total: " << total << std::endl;

    *this << "Denom remainder:" << std::endl;
    List<Denom>::ConstNode left = remainder.head();
    for (; *left; left = left.next()) {
        *this << "   " << left->value
              << ": " << left->count << std::endl;
    }
}

bool Solution::operator!() const
{
    return !solution.hasNext();
}

void Solution::writeProtocol(std::ostream &sink)
{
    String s;
    protLines.dequeue(s);
    while (!s.isEmpty()) {
        sink << s << std::endl;
        protLines.dequeue(s);
    }
}
