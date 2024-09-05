// Copyright [2024] <Sebastián Orozco>

#ifndef SIMULATION_H
#define SIMULATION_H

#include "plate.h"
/// @brief This is the part of math of the simulation and report
/// @param plate objetc plate, this contains the necesary to make the simulation
/// @param output_path this is the path of report
/// @return 1 if all is good 0 if occurs an error
uint8_t init_simulation(plate_t plate, char* output_path);

#endif
