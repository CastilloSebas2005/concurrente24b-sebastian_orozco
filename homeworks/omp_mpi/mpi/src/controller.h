// Copyright [2024] <Sebastián Orozco>

#ifndef CONTROLLER_H
#define CONTROLLER_H

/**
 @class controller
 @brief This class is going to controlate the calls other classes
 @details In this moment his implementation are null.
*/

/// @brief This is the shared data of threads
/// @param argv This is the arguments of the program
/// @param rank This is the rank of the process
/// @param rank_count This is the count of ranks
void init_controller(char *argv[], int rank, int rank_count);

#endif
