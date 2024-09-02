#include "controller.h"
#include "plate.h"
#include "simulation.h"
#include "manager_argument.h"

void init_controller(char *argv[]){
    manager_argument_t manager_argument;
    init_managerArgument(&manager_argument, argv);
    char* jobPath = get_jobPath(&manager_argument);
    char* output_Path = get_outputPath(&manager_argument);
    free(output_Path);
    free(jobPath);
}