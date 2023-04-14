#ifndef NAVMESH
#define NAVMESH

#include <vector>
#include <unordered_map>

#include "polypartition.h"
#include "Struct_utilities.hpp"

class NavMeshPath;

class NavMeshSurface {
    public:
        NavMeshSurface(const std::vector<TPPLPoint>& border);

        void add_area(const std::vector<TPPLPoint>& border);

        void expand(const NavMeshSurface& surface);

        [[nodiscard]] NavMeshPath get_path(const TPPLPoint& start, const TPPLPoint& end);

    private:
        std::vector<TPPLPoint> reconstruct_path(const std::unordered_map<TPPLPoly_it, TPPLPoly_it>& came_from, TPPLPoly_it& current, const TPPLPoint& start, const TPPLPoint& end);

        void update_grid();

        TPPLPolyList m_polygons;  // [0] element should always be border
        TPPLPolyList outpolys;
        std::unordered_map<TPPLPoly_it, std::unordered_map<TPPLPoly_it, poly_intersection>> poly_grid;
        std::unordered_map<TPPLPoly_it, TPPLPoint> centers;
};

class NavMeshPath{
    public:
        NavMeshPath();
        
        NavMeshPath(const std::vector<TPPLPoint>& points);

        const std::vector<TPPLPoint>& get_points() const;

        const TPPLPoint& operator[](size_t index) const;

    private:
        std::vector<TPPLPoint> path;
};

#endif // NavMesh
