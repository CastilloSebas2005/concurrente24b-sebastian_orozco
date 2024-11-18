// Copyright [2024] <Sebastián Orozco>

#ifndef SIMULATION_H
#define SIMULATION_H

#include "plate.h"

/// @brief This is the simulation of the plate
/// @param plate This is the plate to make the simulation
/// @param thread_count This is the number of threads
/// @return The number of states
uint64_t init_simulation(plate_t plate, uint64_t thread_count);

/// @brief This is to make line to report and save in the shared_data
/// @param lineReport This is the line to report
/// @param time This is the time of the simulation
/// @param states This is the number of states
/// @return The line to report with the time and states formatted
char *make_line_to_report(char *lineReport, time_t time, uint64_t states);
#endif
