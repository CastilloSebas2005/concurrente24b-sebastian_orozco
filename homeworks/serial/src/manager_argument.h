// Copyright [2024] <Sebastián Orozco>

#ifndef MANAGER_ARGUMENT_H
#define MANAGER_ARGUMENT_H

/**
 @class manager_argument
 @brief This class is going to controlate the inputs arguments and return the path of archives, to prevent
 incovenients
 @details his atributes are inputPath, this path contains the job file path and outputPath to get path correctly
*/

typedef struct {
  char *jobPath;
  char *outputPath;
} manager_argument;

/**
 *  @brief This procedure initialize the Manager class, this is the constructor
 *  @param argv input argument, this parameter must be contain the archive job
 *  @param manager Type of class
 */
void init_managerArgument(manager_argument *manager, char *argv[]);

/**
 *  @brief This procedure is to get job file path
 *  @param manager Type of class that contains jobPath
 */
char* get_jobPath(manager_argument *manager);

/**
 *  @brief This procedure is to get output file path
 *  @param manager Type of class that contains outputPath
 */
char* get_outputPath(manager_argument *manager);
#endif
