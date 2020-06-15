/// \file 		main.cpp
/// \brief 		Hybrid (CPU and GPU) Version for solving Navier-Stokes equations in 3D
/// \date 		May 20, 2016
/// \author 	Severt
/// \copyright 	<2015-2020> Forschungszentrum Juelich GmbH. All rights reserved.

#include <iostream>
#include "interfaces/ISolver.h"
#include "solver/DiffusionTurbSolver.h"
#include "solver/DiffusionSolver.h"
#include "solver/AdvectionSolver.h"
#include "solver/AdvectionDiffusionSolver.h"
#include "solver/NSSolver.h"
#include "solver/NSTurbSolver.h"
#include "solver/NSTempSolver.h"
#include "solver/NSTempTurbSolver.h"
#include "solver/NSTempConSolver.h"
#include "solver/NSTempTurbConSolver.h"
#include "solver/PressureSolver.h"
#include "TimeIntegration.h"
#include "utility/tinyxml2.h"
#include "utility/Parameters.h"

#ifndef PROFILING
#include "analysis/Analysis.h"
#include "utility/Visual.h"
#endif

#ifdef _OPENACC
	#include <openacc.h>
#endif

int main(int argc, const char** argv) {

// 0. Initialization
	// Parameters
	std::string XMLfilename;
	if ( argc > 1 ) XMLfilename.assign( argv[1] );
    auto params = Parameters::getInstance();
    params->parse(XMLfilename);

  // Solver
	ISolver* solver;
  std::string string_solver = params->get("solver/description");
	if 		  (string_solver == SolverTypes::DiffusionSolver) 			   solver=new DiffusionSolver();
	else if (string_solver == SolverTypes::AdvectionSolver) 			   solver=new AdvectionSolver();
	else if (string_solver == SolverTypes::AdvectionDiffusionSolver) solver=new AdvectionDiffusionSolver();
	else if (string_solver == SolverTypes::PressureSolver) 					 solver=new PressureSolver();
	else if (string_solver == SolverTypes::DiffusionTurbSolver) 		 solver=new DiffusionTurbSolver();
	else if (string_solver == SolverTypes::NSSolver) 				    		 solver=new NSSolver();
	else if (string_solver == SolverTypes::NSTurbSolver) 			    	 solver=new NSTurbSolver();
	else if (string_solver == SolverTypes::NSTempSolver) 			    	 solver=new NSTempSolver();
	else if (string_solver == SolverTypes::NSTempTurbSolver) 		     solver=new NSTempTurbSolver();
	else if (string_solver == SolverTypes::NSTempConSolver) 			 	 solver=new NSTempConSolver();
	else if (string_solver == SolverTypes::NSTempTurbConSolver) 		 solver=new NSTempTurbConSolver();

	else {
		std::cout<<"Solver not yet implemented! Simulation stopped!"<<std::endl;
		std::flush(std::cout);
		std::exit(1);
	}

// 1. Visualize and test initial conditions
#ifndef PROFILING
	// Solution
	Analysis ana;
	ana.Analyse(solver, 0.);

	// Visualize
	Visual vis;
	vis.Visualize(solver, 0., argv[1]);
#endif

#ifdef _OPENACC
	// Initialize GPU
	acc_device_t dev_type = acc_get_device_type();
	acc_init( dev_type );
#endif

// 2. Integrate over time and solve numerically
	// Time integration
	TimeIntegration ti(solver, argv[1]);
	ti.run();

// 3. Compute analytical solution and compare
#ifndef PROFILING
	real t_end = params->getReal("physical_parameters/t_end");
	ana.Analyse(solver, t_end);
#endif

//4. Clean up
	delete solver;
	return 0;
}
