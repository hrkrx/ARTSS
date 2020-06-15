/// \file 		GlobalMacrosTypes.h
/// \brief 		Defines macros and typedefs
/// \details	This Header defines macros for indexing (\c IX(i,j)), swapping values (\c SWAP(a,b)) and nested for-loops (\c FOR_EACH_CELL, \c FOR_EACH_INNER_CELL)
/// \date 		Jun 15, 2015
/// \author 	Severt
/// \copyright 	<2015-2020> Forschungszentrum Juelich GmbH. All rights reserved.

#ifndef ARTSS_UTILITY_GLOBALMACROSTYPES_H_
#define ARTSS_UTILITY_GLOBALMACROSTYPES_H_

//======================================= Macros =======================================
// ***************************************************************************************
/// \def IX(i,j,k,Nx,Ny)
/// \brief Computes the one-dimensional row-major index of a three-dimensional array \a (i,j,k)
/// \def xi(i,x,dx)
/// \brief Physical x-coordinates at midpoints calculated by index \a i=0..Nx-1
/// \def yj(j,y,dy)
/// \brief Physical y-coordinates at midpoints calculated by index \a j=0..Ny-1
/// \def zk(k,z,dz)
/// \brief Physical z-coordinates at midpoints calculated by index \a k=0..Nz-1
/// \def tn(n,dt)
/// \brief Simulation time calculated by time steps \a n = 0...Nt
/// \def FOR_EACH_CELL(Nx, Ny, Nz)
/// \brief Creates a nested \c for -loop for each cell in the computational domain (\f$ i=0, ..., Nx-1 \f$, \f$ j=0, ..., Ny-1 \f$ and \f$ l=0, ..., Nz-1 \f$)
/// \def FOR_EACH_INNER_CELL(Nx, Ny, Nz)
/// \brief Creates a nested \c for -loop for the inner cells in the computational domain (\f$ i=1, ..., Nx-2 \f$, \f$ j=1, ..., Ny-2 \f$ and \f$ l=0, ..., Nz-2 \f$)
/// \def END_FOR
/// \brief End the nested \c for -loop
/// \def FOR_EACH_CELL_ACC(Nx, Ny, Nz)
/// \brief Creates a parallelized independent nested \c for -loop for each cell in the computational domain (\f$ i=0, ..., Nx-1 \f$, \f$ j=0, ..., Ny-1 \f$ and \f$ l=0, ..., Nz-1 \f$)
/// \def FOR_EACH_INNER_CELL_ACC(Nx, Ny, Nz)
/// \brief Creates a parallelized independent nested \c for -loop for the inner cells in the computational domain (\f$ i=1, ..., Nx-2 \f$, \f$ j=1, ..., Ny-2 \f$ and \f$ l=0, ..., Nz-2 \f$)
/// \def END_FOR_ACC
/// \brief End the parallelized independent nested \c for -loop
// ***************************************************************************************

#define IX(i,j,k,Nx,Ny) ((i) + (Nx)*(j) + (Nx)*(Ny)*(k)) // row-major index for one dimensional arrays (i=0..Nx-1 columns, j=0..Ny-1 rows, k = 0...Nz-1)
#define getCoordinateI(idx,Nx,Ny,j,k) (idx - k * Nx * Ny - j * Nx)
#define getCoordinateJ(idx,Nx,Ny,k) ((idx - k * Nx * Ny) / Nx)
#define getCoordinateK(idx,Nx,Ny)(idx / (Nx * Ny))


#define xi(i,x,dx) ((x) + ((i)-0.5)*(dx)) // physical xcoords at midpoints calculated by index i=0..Nx-1
#define yj(j,y,dy) ((y) + ((j)-0.5)*(dy)) // physical ycoords at midpoints calculated by index j=0..Ny-1
#define zk(k,z,dz) ((z) + ((k)-0.5)*(dz)) // physical zcoords at midpoints calculated by index k=0..Nz-1
#define tn(n, dt) ((n)*(dt))			  // simulation time calculated by time steps n = 0...Nt

#define FOR_EACH_CELL(Nx, Ny, Nz) for (size_t k=0; k<Nz; k++) {\
						for (size_t j=0; j<Ny; j++) {\
							for (size_t i=0; i<Nx; i++) {

#define FOR_EACH_INNER_CELL(Nx, Ny, Nz) for (size_t k=1; k<Nz-1; k++) {\
						for (size_t j=1; j<Ny-1; j++) {\
							for (size_t i=1; i<Nx-1; i++) {

#define END_FOR		  }}}

// OpenACC Loops
#define FOR_EACH_CELL_ACC(Nx, Ny, Nz) _Pragma("acc loop independent")\
								{\
									for (size_t k=0; k<Nz; k++) {\
										_Pragma("acc loop independent")\
											{\
												for (size_t j=0; j<Ny; j++) {\
													_Pragma("acc loop independent")\
															{\
																for (size_t i=0; i<Nx; i++) {

#define FOR_EACH_INNER_CELL_ACC(Nx, Ny, Nz) _Pragma("acc loop independent")\
								{\
									for (size_t k=1; k<Nz-1; k++) {\
										_Pragma("acc loop independent")\
												{\
													for (size_t j=1; j<Ny-1; j++) {\
														_Pragma("acc loop independent")\
																{\
																	for (size_t i=1; i<Nx-1; i++) {

#define END_FOR_ACC		  }}}}}}
//====================================== Typedefs ======================================
// ***************************************************************************************
/// \def real
/// \brief Defines the data type the solver calculates with
/// \def read_ptr
/// \brief Defines a constant pointer to a constant real, __restrict__ for GPU version
/// \def write_ptr
/// \brief Defines a constant pointer to a constant real, __restrict__ for GPU version
/// \def return_ptr
/// \brief For returning data by class functions
// ***************************************************************************************


typedef double real;								// data type for solver (float, double, ...)

typedef const real* __restrict const read_ptr;		//readable for for GPU version
typedef real* __restrict const write_ptr;			//writable ptr for GPU version
typedef const real* __restrict return_ptr;			//for returning data by class functions.
                                                    //const pointer is superfluous, because non-class type
                                                    //return values are not modifiable anyway.

typedef const real* const aliased_read_ptr;

#endif /* ARTSS_UTILITY_GLOBALMACROSTYPES_H_ */
