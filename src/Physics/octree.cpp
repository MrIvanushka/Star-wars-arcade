#include "octree.h"
#include<iostream>
// calculate bounds of specified quadrant in bounding region
void Octree::calculateBounds(BoundingRegion &out, Octant octant, BoundingRegion parentRegion) {
    // find min and max points of corresponding octant
    
    glm::vec3 center = parentRegion.calculateCenter();
    if (octant == Octant::O1) {
        out = BoundingRegion(center, parentRegion.max);
    }
    else if (octant == Octant::O2) {
        out = BoundingRegion(glm::vec3(parentRegion.min.x, center.y, center.z), glm::vec3(center.x, parentRegion.max.y, parentRegion.max.z));
    }
    else if (octant == Octant::O3) {
        out = BoundingRegion(glm::vec3(parentRegion.min.x, parentRegion.min.y, center.z), glm::vec3(center.x, center.y, parentRegion.max.z));
    }
    else if (octant == Octant::O4) {
        out = BoundingRegion(glm::vec3(center.x, parentRegion.min.y, center.z), glm::vec3(parentRegion.max.x, center.y, parentRegion.max.z));
    }
    else if (octant == Octant::O5) {
        out = BoundingRegion(glm::vec3(center.x, center.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, parentRegion.max.y, center.z));
    }
    else if (octant == Octant::O6) {
        out = BoundingRegion(glm::vec3(parentRegion.min.x, center.y, parentRegion.min.z), glm::vec3(center.x, parentRegion.max.y, center.z));
    }
    else if (octant == Octant::O7) {
        out = BoundingRegion(parentRegion.min, center);
    }
    else if (octant == Octant::O8) {
        out = BoundingRegion(glm::vec3(center.x, parentRegion.min.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, center.y, center.z));
    }
}

/*
    constructors
*/

// default
Octree::node::node()
    : region(BoundTypes::AABB) {
        for(int i = 0; i < NO_CHILDREN; i++)
            children[i] = nullptr;
    }

// initialize with bounds (no objects yet)
Octree::node::node(BoundingRegion bounds)
    : region(bounds) {
        parent = nullptr;

        for(int i = 0; i < NO_CHILDREN; i++)
            children[i] = nullptr;
    }

// initialize with bounds and list of objects
Octree::node::node(BoundingRegion bounds, std::vector<BoundingRegion> objectList)
    : region(bounds) {
    // insert entire list of objects
    objects.insert(objects.end(), objectList.begin(), objectList.end());

    for(int i = 0; i < NO_CHILDREN; i++)
            children[i] = nullptr;
}

/*
    functionality
*/

// add instance to pending queue
void Octree::node::addToPending(OrientedPoint* instance, BoundingRegion* br) {
    br->instance = instance;
    br->transform();
    queue.push(*br);
}

// build tree (called during initialization)
void Octree::node::build() {
    // variable declarations
    BoundingRegion octants[NO_CHILDREN];
    glm::vec3 dimensions = region.calculateDimensions();
    std::vector<BoundingRegion> octLists[NO_CHILDREN]; // array of lists of objects in each octant
    
    /*
        termination conditions (don't subdivide further)
        - 1 or less objects (ie an empty leaf node or node with 1 object)
        - dimesnions are too small
    */

    // <= 1 objects
    if (objects.size() <= 1) {
        // set state variables
        goto setVars;
    }

    // too small
    for (int i = 0; i < 3; i++) {
        if (dimensions[i] < MIN_BOUNDS) {
            // set state variables
            goto setVars;
        }
    }

    // create regions
    for (int i = 0; i < NO_CHILDREN; i++) {
        calculateBounds(octants[i], (Octant)(1 << i), region);
    }

    // determine which octants to place objects in
    for (int i = 0, len = objects.size(); i < len; i++) {
        BoundingRegion br = objects[i];
        for (int j = 0; j < NO_CHILDREN; j++) {
            if (octants[j].containsRegion(br)) {
                // octant contains region
                octLists[j].push_back(br);
                objects.erase(objects.begin() + i);

                // offset because removed object from list
                i--;
                len--;
                break;
            }
        }
    }

    // populate octants
    for (int i = 0; i < NO_CHILDREN; i++) {
        if (octLists[i].size() != 0) {
            // if children go into this octant, generate new child
            children[i] = new node(octants[i], octLists[i]);
            children[i]->parent = this;
            children[i]->build();
        }
    }
    
setVars:
    // set state variables
    treeBuilt = true;
    treeReady = true;

    // set pointer to current cell of each object
    for (int i = 0; i < objects.size(); i++) {
        objects[i].cell = this;
    }
}

// update objects in tree (called during each iteration of main loop)
void Octree::node::update() {
    if (treeBuilt && treeReady) {
        // get moved objects that were in this leaf in previous frame
        std::stack<int> movedObjects;

        for (int i = 0, listSize = objects.size(); i < listSize; i++) {
            if (objects[i].collider->moved() && objects[i].collider->isEnabled()) {
                // if moved switch active, transform region and push to list
                objects[i].transform();
                movedObjects.push(i);
            }
        }
        // move moved objects into new nodes
        BoundingRegion movedObj; // placeholder
        while (movedObjects.size() != 0) {
            /*
                for each moved object
                - traverse up tree (start with current node) until find a node that completely encloses the object
                - call insert (push object as far down as possible)
            */

            movedObj = objects[movedObjects.top()]; // set to top object in stack
            node* current = this; // placeholder
            while (!current->region.containsRegion(movedObj)) {
                if (current->parent != nullptr) {
                    // set current to current's parent (recursion)
                    current = current->parent;
                }
                else {
                    break; // if root node, the leave
                }
            }
            /*
                once finished
                - remove from objects list
                - remove from movedObjects stack
                - insert into found region
            */
            objects.erase(objects.begin() + movedObjects.top());
            movedObjects.pop();
            current->queue.push(movedObj);
            // collision detection
            // itself
            current = movedObj.cell;
            current->checkCollisionsSelf(movedObj);
            // children
            current->checkCollisionsChildren(movedObj);
            // parents
            while (current->parent != nullptr) {
                current = current->parent;
                current->checkCollisionsSelf(movedObj);
            }
        }

        if (children != nullptr) {
            // go through each octant using flags
            for (int i = 0; i < NO_CHILDREN; i++) {
                if (children[i] != nullptr) {
                    children[i]->update();
                }
            }
        }
    }
    processPending();
}

// process pending queue
void Octree::node::processPending() {
    if (!treeBuilt) {
        // add objects to be sorted into branches when built
        while (queue.size() != 0) {
            objects.push_back(queue.front());
            queue.pop();
        }
        build();
    }
    else {
        for (int i = 0, len = queue.size(); i < len; i++) {
            BoundingRegion br = queue.front();
            if (region.containsRegion(br)) {
                // insert object immediately
                insert(br);
            }
            else {
                // return to queue
                br.transform();
                queue.push(br);
            }
            queue.pop();
        }
    }
}

// dynamically insert object into node
bool Octree::node::insert(BoundingRegion obj) {
    /*
        termination conditions
        - no objects (an empty leaf node)
        - dimensions are less than MIN_BOUNDS
    */

    glm::vec3 dimensions = region.calculateDimensions();
    if (objects.size() == 0 ||
        dimensions.x < MIN_BOUNDS ||
        dimensions.y < MIN_BOUNDS ||
        dimensions.z < MIN_BOUNDS
        ) {
        obj.cell = this;
        objects.push_back(obj);
        return true;
    }

    // safeguard if object doesn't fit
    if (!region.containsRegion(obj)) {
        return parent == nullptr ? false : parent->insert(obj);
    }

    // create regions if not defined
    BoundingRegion octants[NO_CHILDREN];
    for (int i = 0; i < NO_CHILDREN; i++) {
        if (children[i] != nullptr) {
            // child exists, so take its region
            octants[i] = children[i]->region;
        }
        else {
            // get region for this octant
            calculateBounds(octants[i], (Octant)(1 << i), region);
        }
    }

    objects.push_back(obj);

    // determine which octants to put objects in
    std::vector<BoundingRegion> octLists[NO_CHILDREN]; // array of list of objects in each octant
    for (int i = 0, len = objects.size(); i < len; i++) {
        objects[i].cell = this;
        for (int j = 0; j < NO_CHILDREN; j++) {
            if (octants[j].containsRegion(objects[i])) {
                octLists[j].push_back(objects[i]);
                // remove from objects list
                objects.erase(objects.begin() + i);
                i--;
                len--;
                break;
            }
        }
    }

    // populate octants
    for (int i = 0; i < NO_CHILDREN; i++) {
        if (octLists[i].size() != 0) {
            // objects exist in this octant
            if (children[i]) {
                for (BoundingRegion br : octLists[i]) {
                    children[i]->insert(br);
                }
            }
            else {
                // create new node
                children[i] = new node(octants[i], octLists[i]);
                children[i]->parent = this;
                children[i]->build();
            }
        }
    }

    return true;
}

// check collisions with all objects in node
void Octree::node::checkCollisionsSelf(BoundingRegion obj) {
    for (BoundingRegion br : objects) {
        if (br == obj || br.collider->isEnabled() == false || br.collider->isTrigger() == true) {
            // do not test collisions with the same instance
            continue;
        }

        // coarse check for bounding region intersection
        if (br.intersectsWith(obj)) {
            // coarse check passed

            auto* SphereObj = dynamic_cast<SphereCollider*>(obj.collider);
            auto* SphereBr = dynamic_cast<SphereCollider*>(br.collider);
            auto* MeshObj = dynamic_cast<MeshCollider*>(obj.collider);
            auto* MeshBr = dynamic_cast<MeshCollider*>(br.collider);

            glm::vec3 norm;

            if (MeshBr != nullptr) {
                unsigned int noFacesBr = MeshBr->faces.size();

                if (MeshObj != nullptr) {
                    // both have collision meshes
                    unsigned int noFacesObj = MeshObj->faces.size();
                    
                    // check all faces in br against all faces in obj
                    for (unsigned int i = 0; i < noFacesBr; i++) {
                        for (unsigned int j = 0; j < noFacesObj; j++) {
                            if (MeshBr->faces[i].collidesWithFace(
                                br.instance,
                                MeshObj->faces[j],
                                obj.instance,
                                norm
                            )) {
                                //throw an event!!
                                MeshObj->handleCollision(br, norm);
                            }
                        }
                    }
                }
                else {
                    // br has a collision mesh, obj does not
                    // check all faces in br against the obj's sphere
                    for (unsigned int i = 0; i < noFacesBr; i++) {
                        if (MeshObj->faces[i].collidesWithSphere(
                            br.instance,
                            obj,
                            norm
                        )) {
                            //throw an event!!
                            obj.collider->handleCollision(br, norm);
                            
                            break;
                        }
                    }
                }
            }
            else {
                if (MeshObj) {
                    // obj has a collision mesh, br does not
                    // check all faces in obj against br's sphere
                    unsigned int noFacesObj = MeshObj->faces.size();

                    for (int i = 0; i < noFacesObj; i++) {
                        if (MeshObj->faces[i].collidesWithSphere(
                            obj.instance,
                            br,
                            norm
                        )) {
                            
                            MeshObj->handleCollision(br, norm);

                            break;
                        }
                    }
                }
                else {
                    // neither have a collision mesh
                    // coarse grain test pased (test collision between spheres)
                
                    norm = obj.center - br.center;

                    MeshObj->handleCollision(br, norm);
                }
            }
        }
    }
}

// check collisions with all objects in child nodes
void Octree::node::checkCollisionsChildren(BoundingRegion obj) {
    if (children) {
        for (int i = 0; i < NO_CHILDREN; i++) 
        {
            if (children[i]) 
            {
                children[i]->checkCollisionsSelf(obj);
                children[i]->checkCollisionsChildren(obj);
            }
        }
    }
}

// check collisions with a ray
BoundingRegion* Octree::node::checkCollisionsRay(Ray r, float& tmin) {
    float tmin_tmp = std::numeric_limits<float>::max();
    float tmax_tmp = std::numeric_limits<float>::lowest();
    float t_tmp = std::numeric_limits<float>::max();

    // check current region
    if (r.intersectsBoundingRegion(region, tmin_tmp, tmax_tmp)) {
        // know ray collides with the current region
        if (tmin_tmp >= tmin) {
            // found nearer collision
            return nullptr;
        }

        BoundingRegion* ret = nullptr, * ret_tmp = nullptr;

        // check objects in the node
        for (BoundingRegion& br : this->objects) {
            tmin_tmp = std::numeric_limits<float>::max();
            tmax_tmp = std::numeric_limits<float>::lowest();
            auto* MeshBr = dynamic_cast<MeshCollider*>(br.collider);

            // coarse check - check against BR
            if (r.intersectsBoundingRegion(br, tmin_tmp, tmax_tmp)) {
                if (tmin_tmp > tmin) {
                    continue;
                }
                else if (MeshBr) {
                    // fine grain check with collision mesh
                    t_tmp = std::numeric_limits<float>::max();
                    if (r.intersectsMesh(MeshBr, br.instance, t_tmp)) {
                        if (t_tmp < tmin) {
                            // found closer collision
                            tmin = t_tmp;
                            ret = &br;
                        }
                    }
                }
                else {
                    // rely on coarse check
                    if (tmin_tmp < tmin) {
                        tmin = tmin_tmp;
                        ret = &br;
                    }
                }
            }
        }

        // check children
        if (children) {
            for (int i = 0; i < NO_CHILDREN; i++) {
                if (children[i])
                {
                    ret_tmp = children[i]->checkCollisionsRay(r, tmin);
                    if (ret_tmp) {
                        ret = ret_tmp;
                    }
                }
            }
        }

        return ret;
    }

    return nullptr;
}