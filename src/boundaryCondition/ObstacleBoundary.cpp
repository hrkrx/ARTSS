/// \file 		  ObstacleBoundary.cpp
/// \brief 		  Applies boundary condition for obstacle boundary
/// \date 		  Feb 03, 2020
/// \author 	  My Linh Würzburger
/// \copyright 	<2015-2020> Forschungszentrum Juelich GmbH. All rights reserved.
#include "ObstacleBoundary.h"
#include "../Domain.h"
#include "../boundary/BoundaryController.h"

//======================================== Apply boundary condition ====================================
// ***************************************************************************************
/// \brief  Applies boundary condition for obstacle boundary
/// \param  dataField	Field
/// \param  indexFields List of indices for each patch
/// \param  patch_starts List of start indices
/// \param  patch_ends List of end indices
/// \param  level Multigrid level
/// \param  boundaryData Boundary data object of Domain
/// \param  id ID of obstacle
/// \param  sync synchronous kernel launching (true, default: false)
// ***************************************************************************************
void ObstacleBoundary::applyBoundaryCondition(real *dataField, size_t **indexFields, const size_t *patch_starts, const size_t *patch_ends, size_t level, BoundaryData *boundaryData, size_t id, bool sync) {
    for (size_t i = 0; i < numberOfPatches; i++) {
        size_t *d_patch = *(indexFields + i);
        size_t patch_start = *(patch_starts + i);
        size_t patch_end = *(patch_ends + i);
        Patch p = static_cast<Patch>(i);
        switch (boundaryData->getBoundaryCondition(p)) {
            case BoundaryCondition::DIRICHLET:
                applyDirichlet(dataField, d_patch, p, patch_start, patch_end, level, boundaryData->getValue(p));
                break;
            case BoundaryCondition::NEUMANN:
                applyNeumann(dataField, d_patch, p, patch_start, patch_end, level, boundaryData->getValue(p));
                break;
            case BoundaryCondition::PERIODIC:
                applyPeriodic(dataField, d_patch, p, patch_start, patch_end, level, id);
                break;
        }
    }
    if (sync) {
#pragma acc wait
    }
}

//======================================== Apply boundary condition ====================================
// ***************************************************************************************
/// \brief  Set boundary condition for obstacle boundary
/// \param  dataField	Field
/// \param  d_patch List of indices for given patch
/// \param  patch_start Start Index of Patch
/// \param  patch_end End index of patch
/// \param  level Multigrid level
/// \param  referenceIndex Index of reference
/// \param  value Value of boundary condition
/// \param  sign Sign of boundary condition
// ***************************************************************************************
void ObstacleBoundary::applyBoundaryCondition(real *dataField, const size_t *d_patch, size_t patch_start, size_t patch_end, size_t level, int referenceIndex, real value, int8_t sign) {
    Domain *domain = Domain::getInstance();
    size_t bsize = domain->get_size(level);
#pragma acc data present(dataField[:bsize])
    {
#pragma acc parallel loop independent present(d_patch[patch_start:(patch_end-patch_start+1)]) async
        for (size_t j = patch_start; j <= patch_end; ++j) { // <= because in patch_end is last index of the patch, so its inclusive
            const size_t index = d_patch[j];
            dataField[index] = sign * dataField[index + referenceIndex] + value;
        }
#pragma acc wait
    }
}

//======================================== Apply dirichlet ====================================
// ***************************************************************************************
/// \brief  Apply dirichlet boundary condition
/// \param  dataField	Field
/// \param  d_patch List of indices for given patch
/// \param  patch Patch
/// \param  patch_start Start Index of Patch
/// \param  patch_end End index of patch
/// \param  level Multigrid level
/// \param  value Value of boundary condition
// ***************************************************************************************
void ObstacleBoundary::applyDirichlet(real *dataField, size_t *d_patch, Patch patch, size_t patch_start, size_t patch_end, size_t level, real value) {
    if (level > 0) {
        value = 0;
    }
    Domain *domain = Domain::getInstance();
    int referenceIndex = 1;
    switch (patch) {
        case FRONT:
            referenceIndex = -1 * static_cast<int>(domain->get_Nx(level)) * static_cast<int>(domain->get_Ny(level));
            break;
        case BACK:
            referenceIndex = static_cast<int>(domain->get_Nx(level)) * static_cast<int>(domain->get_Ny(level));
            break;
        case BOTTOM:
            referenceIndex = -1 * static_cast<int>(domain->get_Nx(level));
            break;
        case TOP:
            referenceIndex = domain->get_Nx(level);
            break;
        case LEFT:
            referenceIndex = -1;
            break;
        case RIGHT:
            referenceIndex = 1;
            break;
    }
    applyBoundaryCondition(dataField, d_patch, patch_start, patch_end, level, referenceIndex, value * 2, -1);
}

//======================================== Apply neumann ====================================
// ***************************************************************************************
/// \brief  Apply neumann boundary condition
/// \param  dataField	Field
/// \param  d_patch List of indices for given patch
/// \param  patch Patch
/// \param  patch_start Start Index of Patch
/// \param  patch_end End index of patch
/// \param  level Multigrid level
/// \param  value Value of boundary condition
// ***************************************************************************************
void ObstacleBoundary::applyNeumann(real *dataField, size_t *d_patch, Patch patch, size_t patch_start, size_t patch_end, size_t level, real value) {
    if (level > 0) {
        value = 0;
    }
    Domain *domain = Domain::getInstance();
    int referenceIndex = 1;
    switch (patch) {
        case FRONT:
            value *= domain->get_dz(level);
            referenceIndex = -1 * static_cast<int>(domain->get_Nx(level)) * static_cast<int>(domain->get_Ny(level));
            break;
        case BACK:
            value *= domain->get_dz(level);
            referenceIndex = static_cast<int>(domain->get_Nx(level)) * static_cast<int>(domain->get_Ny(level));
            break;
        case BOTTOM:
            value *= domain->get_dy(level);
            referenceIndex = -1 *static_cast<int>(domain->get_Nx(level));
            break;
        case TOP:
            value *= domain->get_dy(level);
            referenceIndex = static_cast<int>(domain->get_Nx(level));
            break;
        case LEFT:
            value *= domain->get_dx(level);
            referenceIndex = -1;
            break;
        case RIGHT:
            value *= domain->get_dx(level);
            referenceIndex = 1;
            break;
    }
    applyBoundaryCondition(dataField, d_patch, patch_start, patch_end, level, referenceIndex, -value, 1);
}

//======================================== Apply periodic ====================================
// ***************************************************************************************
/// \brief  Apply periodic boundary condition
/// \param  dataField	Field
/// \param  d_patch List of indices for given patch
/// \param  patch Patch
/// \param  patch_start Start Index of Patch
/// \param  patch_end End index of patch
/// \param  level Multigrid level
// ***************************************************************************************
void ObstacleBoundary::applyPeriodic(real *dataField, size_t *d_patch, Patch patch, size_t patch_start, size_t patch_end, size_t level, size_t id) {
    Domain *domain = Domain::getInstance();
    size_t Nx = domain->get_Nx(level);
    size_t Ny = domain->get_Ny(level);
    BoundaryController *bdc = BoundaryController::getInstance();
    int referenceIndex = 1;
    switch (patch) {
        case BACK:
            referenceIndex = -1;
        case FRONT:
            referenceIndex *= Nx * Ny * (bdc->getObstacleStrideZ(id, level) - 2);
            break;
        case TOP:
            referenceIndex = -1;
        case BOTTOM:
            referenceIndex *= Nx * (bdc->getObstacleStrideY(id, level) - 2);
            break;
        case RIGHT:
            referenceIndex = -1;
        case LEFT:
            referenceIndex *= (bdc->getObstacleStrideX(id, level) - 2);
            break;
    }
    applyBoundaryCondition(dataField, d_patch, patch_start, patch_end, level, referenceIndex, 0, 1);
}
