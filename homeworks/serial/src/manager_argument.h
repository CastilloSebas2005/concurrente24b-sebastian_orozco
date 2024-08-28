// Copyright [2024] <Sebastián Orozco>

#ifndef MANAGER_ARGUMENT_h
#define MANAGER_ARGUMENT_h

/**
 @class manager_argument
 @brief This class is going to controlate the inputs arguments, to prevent
 incovenients
 @details his atributtes are the num of args needed to work argv1 is the name of
 job archive and argv2 is the path of job archive and argv3 the name of path to
 output archive
*/

typedef struct {
  /* data */
  /// name of the job archive
  char *argv1;
  /// path of the job archive and plates archives
  char *argv2;
  /// path of the output archive
  char *argv3;
} manager_argument;

/**
 *  @brief This procedure initialize the Manager class, this is the constructor
 *  @param argv input argument, this parameter must be contain the archive job
 *  @param manager Type of class
 */
void init_managerArgument(manager_argument *manager, char *argv[]);

/// @brief This procedure is to get the argv that contains the file
/// @param manager this is the type of class created by struct def
/// @return this function returns the argv1 of the manager_argument
char *get_arg1(manager_argument *manager);

/// @brief This procedure is to get the argv that contains the file
/// @param manager this is the type of class created by struct def
/// @return this function returns the argv2 of the manager_argument
char *get_arg2(manager_argument *manager);

/// @brief This procedure is to get the argv that contains the file
/// @param manager this is the type of class created by struct def
/// @return this function returns the argv2 of the manager_argument
char *get_arg3(manager_argument *manager);
#endif
