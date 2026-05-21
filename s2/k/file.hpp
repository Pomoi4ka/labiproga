#ifndef FILE_HPP_
#define FILE_HPP_

struct Product {
    String name;
    size_t price;

    inline Product();
};

struct Task {
    Product const *product;
    size_t count;
    size_t row, col;
};

struct Agent {
    String name;
    List<Task> tasks;

    Task *findTask(Product const *);
    size_t sum() const;
};

struct Denom {
    size_t value;
    size_t count;
};

struct File {
    List<Product> products;
    List<Agent>   agents;
    List<Denom>   denoms;

    Product *findOrInsertProduct(String const &);
    Agent *findOrInsertAgent(String const &);
    Denom *findOrInsertDenom(size_t);
};

#endif // FILE_HPP_
