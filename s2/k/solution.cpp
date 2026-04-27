#include <new>
#include <cstddef>
#include <ostream>

#include "list.hpp"
#include "string.hpp"
#include "file.hpp"
#include "solution.hpp"

Solution::Branches
Solution::generatePayouts(long amount, Stock stock)
{
    Branches branches;
    if (amount <= 0) {
        branches.append()->stock = stock;
        return branches;
    }
    if (!stock.hasNext()) return branches;
    Denom head = stock.popLeft();
    size_t d = head.value;
    size_t q = head.count;
    size_t maxUse = std::min(q, (amount + d - 1) / d);
    for (size_t k = 0; k <= maxUse; ++k) {
        Branches sub = generatePayouts(amount - k * d, stock);

        while (sub.hasNext()) {
            Branch *subBranch = sub.next();
            Branch *b = branches.append();
            b->payout = subBranch->payout;
            if (k > 0) {
                Denom *n = b->payout.pushLeft();
                n->value = d;
                n->count = k;
            }

            Stock &remaining = subBranch->stock;
            if (k < q) {
                Denom *rem = remaining.pushLeft();
                rem->value = d;
                rem->count = q - k;
            }

            b->stock = subBranch->stock;
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

size_t Solution::solve(Stock stock, AgentNode agents, List<Payout> &result)
{
    if (!*agents) return 0;
    size_t amount = **agents;
    AgentNode restAgents = agents.next();
    Branches branches = generatePayouts(amount, stock);
    size_t minCount = ~0;

    List<Payout> bestRestResult;
    Payout bestPayout;

    while (branches.hasNext()) {
        List<Payout> restResult;
        Branch *branch = branches.next();
        size_t subResult = solve(branch->stock, restAgents, restResult);
        if (subResult == ~0UL) continue;
        size_t used = count(branch->payout) + subResult;
        if (used < minCount) {
            minCount = used;
            bestPayout = branch->payout;
            bestRestResult = restResult;
        }
    }
    if (minCount != ~0UL) {
        result = bestRestResult;
        *result.pushLeft() = bestPayout;
    }
    return minCount;
}

Solution::Solution(File const &file)
    : solution()
{
    Agents agents;
    Stock stock = file.denoms;
    stock.reset();

    for (List<Agent>::ConstNode agent = file.agents.head();
         *agent; agent = agent.next())
        *agents.append() = agent->sum();

    List<Payout> result;
    if (solve(stock, agents.head(), result) != ~0UL)
        solution = result;
}
