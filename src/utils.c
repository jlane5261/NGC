#include "../include/ngc.h"

int validate_input_parameters(const NozzleGeometry* nozzle, const FlowConditions* conditions) {
    if (!nozzle || !conditions) {
        printf("Error: Null pointer passed to validation function\n");
        return -1;
    }

    // Validate nozzle geometry parameters
    if (nozzle->throat_radius <= 0) {
        printf("Error: Throat radius must be positive\n");
        return -1;
    }

    if (nozzle->exit_radius <= nozzle->throat_radius) {
        printf("Error: Exit radius must be greater than throat radius\n");
        return -1;
    }

    // Validate flow conditions
    if (conditions->chamber_pressure <= 0) {
        printf("Error: Chamber pressure must be positive\n");
        return -1;
    }

    if (conditions->ambient_pressure < 0) {
        printf("Error: Ambient pressure cannot be negative\n");
        return -1;
    }

    if (conditions->chamber_temperature <= 0) {
        printf("Error: Chamber temperature must be positive\n");
        return -1;
    }

    if (conditions->molecular_weight <= 0) {
        printf("Error: Molecular weight must be positive\n");
        return -1;
    }

    if (conditions->gamma <= 1.0) {
        printf("Error: Specific heat ratio must be greater than 1.0\n");
        return -1;
    }

    if (conditions->gas_constant <= 0) {
        printf("Error: Gas constant must be positive\n");
        return -1;
    }

    printf("Input parameters validated successfully\n");
    return 0;
}

void print_usage(const char* program_name) {
    printf("Usage: %s [options]\n", program_name);
    printf("\nRocket Nozzle Geometry Calculator (NGC)\n");
    printf("A tool for calculating and plotting bell nozzle geometries and propulsion performance\n\n");
    printf("Options:\n");
    printf("  -h, --help              Show this help message\n");
    printf("  -t, --throat-radius     Throat radius in meters (default: 0.01)\n");
    printf("  -e, --exit-radius       Exit radius in meters (default: 0.03)\n");
    printf("  -p, --chamber-pressure  Chamber pressure in Pa (default: 1000000)\n");
    printf("  -a, --ambient-pressure  Ambient pressure in Pa (default: 101325)\n");
    printf("  -T, --chamber-temp      Chamber temperature in K (default: 3000)\n");
    printf("  -M, --molecular-weight  Molecular weight in kg/mol (default: 0.02)\n");
    printf("  -g, --gamma             Specific heat ratio (default: 1.3)\n");
    printf("  -l, --length-fraction   Nozzle length fraction (default: 0.8)\n");
    printf("  -o, --output            Output plot filename (default: nozzle_plot.png)\n");
    printf("  --data                  Output geometry data filename\n");
    printf("\nExamples:\n");
    printf("  %s -t 0.005 -e 0.025 -p 2000000\n", program_name);
    printf("  %s --throat-radius 0.01 --exit-radius 0.05 --output my_nozzle.png\n", program_name);
    printf("\n");
}