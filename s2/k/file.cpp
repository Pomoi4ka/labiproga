#include <cstddef>
#include <ostream>

#include "consts.hpp"
#include "string.hpp"
#include "file.hpp"

Product::Product()
    : name()
    , price(PRODUCT_PRICE_UNKNOWN)
{}

Task *Agent::findTask(Product *p)
{
    tasks.reset();
    for (Task *t = tasks.next(); t; t = tasks.next())
        if (t->product == p) return t;
    return NULL;
}

Denom *File::findOrInsertDenom(size_t value)
{
    denoms.reset();
    for (Denom *d = denoms.next(); d; d = denoms.next())
        if (d->value == value) return d;
    Denom *d = denoms.append();
    d->value = value;
    return d;
}

Product *File::findOrInsertProduct(String const &name)
{
    products.reset();
    for (Product *p = products.next(); p; p = products.next())
        if (p->name == name) return p;
    Product *p = products.append();
    p->name = name;
    return p;
}

Agent *File::findOrInsertAgent(String const &name)
{
    agents.reset();
    for (Agent *a = agents.next(); a; a = agents.next())
        if (a->name == name) return a;
    Agent *a = agents.append();
    a->name = name;
    return a;
}

size_t Agent::sum() const
{
    size_t result = 0;
    List<Task>::ConstNode task = tasks.head();
    for (; *task; task = task.next()) {
        result += task->count * task->product->price;
    }
    return result;
}
