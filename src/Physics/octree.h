#ifndef OCTREE_H
#define OCTREE_H

#define NO_CHILDREN 8
#define MIN_BOUNDS 1

#include <vector>
#include <queue>
#include <stack>

#include "list.hpp"
#include "bounds.h"
#include "ray.h"

// forward declaration
class BoundingRegion;

/*
    namespace to tie together all classes and functions relating to octree
*/

namespace Octree {
    /*
        enum to represent octants
    */

    enum class Octant : unsigned char {
        O1 = 0x01,	// = 0b00000001
        O2 = 0x02,	// = 0b00000010
        O3 = 0x04,	// = 0b00000100
        O4 = 0x08,	// = 0b00001000
        O5 = 0x10,	// = 0b00010000
        O6 = 0x20,	// = 0b00100000
        O7 = 0x40,	// = 0b01000000
        O8 = 0x80	// = 0b10000000
    };

    /*
        utility methods callbacks
    */

    // calculate bounds of specified quadrant in bounding region
    void calculateBounds(BoundingRegion &out, Octant octant, BoundingRegion parentRegion);

    /*
        class to represent each node in the octree
    */
    class node {
    public:
        // parent pointer
        node* parent;
        // array of children (8)
        node* children[NO_CHILDREN];

        // if tree is ready
        bool treeReady = false;
        // if tree is built
        bool treeBuilt = false;

        // list of objects in node
        std::vector<BoundingRegion> objects;
        // queue of objects to be dynamically inserted
        std::queue<BoundingRegion> queue;

        // region of bounds of cell (AABB)
        BoundingRegion region;

        /*
            constructors
        */

        // default
        node();
        
        // initialize with bounds (no objects yet)
        node(BoundingRegion bounds);

        // initialize with bounds and list of objects
        node(BoundingRegion bounds, std::vector<BoundingRegion> objectList);

        /*
            functionality
        */

        // add instance to pending queue
        void addToPending(OrientedPoint* instance, BoundingRegion* br);

        // build tree (called during initialization)
        void build();

        // update objects in tree (called during each iteration of main loop)
        void update();

        // process pending queue
        void processPending();

        // dynamically insert object into node
        bool insert(BoundingRegion obj);

        // check collisions with all objects in node
        void checkCollisionsSelf(BoundingRegion obj);

        // check collisions with all objects in child nodes
        void checkCollisionsChildren(BoundingRegion obj);

        // check collisions with a ray
        BoundingRegion* checkCollisionsRay(Ray r, float& tmin);
    };
}

#endif