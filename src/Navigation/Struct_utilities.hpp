#ifndef STRUCTURE_UTILITIES
#define STRUCTURE_UTILITIES

#include <vector>
#include <functional>
#include <cstdint>

#include "polypartition.h"

template<>
struct std::hash<TPPLPoly*> {
    inline std::size_t operator() (const TPPLPoly* poly) const noexcept{
        auto addr = reinterpret_cast<uintptr_t>(poly);
#if SIZE_MAX < UINTPTR_MAX
      /* size_t is not large enough to hold the pointer’s memory address */
      addr %= SIZE_MAX; /* truncate the address so it is small enough to fit in a size_t */
#endif
      return addr;
    }
};

template<>
struct std::hash<TPPLPoly_it> {
    inline std::size_t operator() (const TPPLPoly_it& it) const noexcept{
        return std::hash<TPPLPoly*>()(&*it);
    }
};

struct poly_intersection {
    public:
        poly_intersection();

        poly_intersection(const poly_intersection& other);

        poly_intersection& operator=(const poly_intersection& other);

        std::vector<TPPLPoint> line;
};

struct Node{
    public:
        Node();

        Node(const TPPLPoly_it& p, double score);

        Node(const Node& other);

        Node& operator=(const Node& other);

        bool operator<(const Node& other) const;

        TPPLPoly_it poly;
        double f_score;
};

#endif
