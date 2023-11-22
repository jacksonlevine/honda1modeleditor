
struct intTup {
public:
    int x;
    int y;
    int z;
    intTup(int x, int y, int z);
    intTup(int x, int z);
    bool operator==(const intTup& other) const;
    intTup& operator+=(const intTup& other);
    
};
// non-mutating => non-member function
intTup operator+(intTup first, // parameter as value, move-construct (or elide)
    const intTup& second);


struct intTupHash {
    size_t operator()(const intTup& tup) const;
};