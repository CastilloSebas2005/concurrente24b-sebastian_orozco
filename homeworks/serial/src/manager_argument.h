// Copyright [2024] <Sebastián Orozco>

#ifndef MANAGER_ARGUMENT_h
#define MANAGER_ARGUMENT_h

/**
 @class Manager_argument
 @brief This class is going to controlate the inputs arguments, to prevent
 incovenients
 @param argv (this is the input argument)
 @details In this moment his implementation are null.
*/

typedef struct {
  /* data */
  char *argv;
} manager_argument;

/// @brief This procedure initialize the Manager class
/// @param manager Type of class
/// @param argv input argument, this parameter must be contain the archive job
void init_managenrArgument(manager_argument *manager, char *argv[]);

#endif
