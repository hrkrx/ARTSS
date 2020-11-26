/// \file       Obstacle.h
/// \brief      Data class of obstacle object
/// \date       Oct 01, 2019
/// \author     My Linh Würzburger
/// \copyright  <2015-2020> Forschungszentrum Juelich GmbH. All rights reserved.

#ifndef ARTSS_BOUNDARY_OBSTACLE_H
#define ARTSS_BOUNDARY_OBSTACLE_H

#include <cmath>
#include <vector>

#include "../utility/tinyxml2.h"
#include "BoundaryData.h"
#include "BoundaryDataController.h"
#include "../Domain.h"
#include "../utility/Utility.h"


class Obstacle {
public:
    Obstacle(real x1, real x2, real y1, real y2, real z1, real z2, const std::string& name);
    Obstacle(size_t coords_i1, size_t coords_j1, size_t coords_k1, size_t coords_i2, size_t coords_j2, size_t coords_k2, size_t level, std::string name);
    ~Obstacle();

    size_t* getObstacleList() { return m_obstacleList; }
    size_t* getObstacleFront() { return m_obstacleFront; }
    size_t* getObstacleBack() { return m_obstacleBack; }
    size_t* getObstacleTop() { return m_obstacleTop; }
    size_t* getObstacleBottom() { return m_obstacleBottom; }
    size_t* getObstacleLeft() { return m_obstacleLeft; }
    size_t* getObstacleRight() { return m_obstacleRight; }

    size_t getSize_obstacleList() { return m_size_obstacleList; }

    size_t getSize_obstacleFront() { return m_size_obstacleFront; }
    size_t getSize_obstacleBack() { return m_size_obstacleBack; }
    size_t getSize_obstacleBottom() { return m_size_obstacleBottom; }
    size_t getSize_obstacleTop() { return m_size_obstacleTop; }
    size_t getSize_obstacleLeft() { return m_size_obstacleLeft; }
    size_t getSize_obstacleRight() { return m_size_obstacleRight; }

    bool isObstacleCell(size_t i, size_t j, size_t k) const;

    void print();

    size_t getCoordinates_i1() { return m_i1; }
    size_t getCoordinates_j1() { return m_j1; }
    size_t getCoordinates_k1() { return m_k1; }
    size_t getCoordinates_i2() { return m_i2; }
    size_t getCoordinates_j2() { return m_j2; }
    size_t getCoordinates_k2() { return m_k2; }

    size_t getStrideX() { return m_i2 - m_i1 + 1; }
    size_t getStrideY() { return m_j2 - m_j1 + 1; }
    size_t getStrideZ() { return m_k2 - m_k1 + 1; }

    std::string get_name() { return m_name; }

    bool static removeCellsFacingAnotherObstacle(Obstacle *o1, Obstacle* o2);
    void replace_patch(size_t *indices, size_t size, Patch p);
    void control();

    void set_inner_cells(Field *f, real value);

private:
#ifndef BENCHMARKING
    std::shared_ptr<spdlog::logger> m_logger;
#endif
    std::string m_name;
    size_t m_i1, m_j1, m_k1;
    size_t m_i2, m_j2, m_k2;

    size_t m_level = 0;

    size_t* m_obstacleList;
    //size_t* m_obstacleBoundaryList;
    size_t* m_obstacleFront;
    size_t* m_obstacleBack;
    size_t* m_obstacleTop;
    size_t* m_obstacleBottom;
    size_t* m_obstacleLeft;
    size_t* m_obstacleRight;
    //size_t* m_obstacleInner;

    size_t m_size_obstacleList;
    //size_t m_size_obstacleBoundaryList;
    //size_t m_size_obstacleInner;

    //actual size
    size_t m_size_obstacleFront;
    size_t m_size_obstacleBack;
    size_t m_size_obstacleTop;
    size_t m_size_obstacleBottom;
    size_t m_size_obstacleLeft;
    size_t m_size_obstacleRight;

    void init(size_t level);
    void createObstacle(size_t Nx, size_t Ny);

    static real match_grid(real obstacle_coordinate, real spacing, real start_coordinate);
    static int get_matching_index(real obstacle_coordinate, real spacing, real start_coordinate);

    void printDetails();

    void removeCellsAtBoundary(size_t level);

    static bool hasOverlap(size_t o1_coord1, size_t o1_coord2, size_t o2_coord1, size_t o2_coord2);

    size_t getSize(){return getStrideZ() * getStrideY() * getStrideX();};

    void remove_patch(Patch patch);
};

#endif /* ARTSS_BOUNDARY_OBSTACLE_H */
