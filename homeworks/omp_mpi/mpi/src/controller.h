// Copyright [2024] <Sebastián Orozco>

#ifndef CONTROLLER_H
#define CONTROLLER_H

/**
 @class controller
 @brief This class is going to controlate the calls other classes
 @details In this moment his implementation are null.
*/

/// @brief this is to control the simulation
/// @param argv arguments
/// @param rank rank of process
void init_controller(char *argv[], int rank, int rank_count);

#endif
