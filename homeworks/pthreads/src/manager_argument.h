// Copyright [2024] <Sebastián Orozco>

#ifndef MANAGER_ARGUMENT_H
#define MANAGER_ARGUMENT_H
#include <inttypes.h>
/**
 @class manager_argument
 @brief This class is going to controlate the inputs arguments and return the
 path of archives, to prevent incovenients
 @details his atributes are inputPath, this path contains the job file path and
 outputPath to get path correctly
*/

/** @var manager_argument::jobPath
 *  This is the path of job file
 *  @var manager_argument::outputPath
 * This is the path of output file
 */
typedef struct manager_argument {
  // this is the path of job file
  char *jobPath;
  // this is the path where is goint to save the report
  char *outputPath;
  // this is the count of threads
  uint64_t thread_count;
} manager_argument_t;

/**
 *  @brief This procedure initialize the Manager class, this is the constructor
 *  @param argv input argument, this parameter must be contain the archive job
 *  @param manager Type of class
 */
void init_managerArgument(manager_argument_t *manager, char *argv[]);

/**
 *  @brief This procedure is to get job file path
 *  @param manager struct manager_argument that contains jobPath
 */
char *get_jobPath(manager_argument_t *manager);

/**
 *  @brief This procedure is to get output file path
 *  @param manager struct manager_argument that contains jobPath
 */
char *get_outputPath(manager_argument_t *manager);

/// @brief This function is to know how lines have the file job
/// @param path Rute of job file
/// @return The count of lines of the job file
__uint64_t get_lines_to_read(char *path);

/// @brief This is to liberate the memory of manager argument
/// @param manager struct manager_argument
void destruct_manager(manager_argument_t *manager);
#endif
