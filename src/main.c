#include "../include/ngc.h"
#include <string.h>
#include <getopt.h>

int main(int argc, char* argv[]) {
    // Default parameters
    NozzleGeometry nozzle = {0};
    FlowConditions conditions = {0};
    PerformanceResults results = {0};
    
    // Set default values
    nozzle.throat_radius = 0.01;        // 1 cm
    nozzle.exit_radius = 0.03;          // 3 cm
    conditions.chamber_pressure = 1.0e6; // 1 MPa
    conditions.ambient_pressure = 101325; // 1 atm
    conditions.chamber_temperature = 3000; // 3000 K
    conditions.molecular_weight = 0.020;  // 20 g/mol (typical for combustion products)
    conditions.gamma = 1.3;               // Typical for hot gases
    conditions.gas_constant = 8314.5;     // J/(kmol·K)
    
    double length_fraction = 0.8;
    char output_filename[MAX_FILENAME] = "nozzle_plot.png";
    char data_filename[MAX_FILENAME] = "";
    
    // Command line options
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"throat-radius", required_argument, 0, 't'},
        {"exit-radius", required_argument, 0, 'e'},
        {"chamber-pressure", required_argument, 0, 'p'},
        {"ambient-pressure", required_argument, 0, 'a'},
        {"chamber-temp", required_argument, 0, 'T'},
        {"molecular-weight", required_argument, 0, 'M'},
        {"gamma", required_argument, 0, 'g'},
        {"length-fraction", required_argument, 0, 'l'},
        {"output", required_argument, 0, 'o'},
        {"data", required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

    int c;
    int option_index = 0;

    while ((c = getopt_long(argc, argv, "ht:e:p:a:T:M:g:l:o:d:", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                print_usage(argv[0]);
                return 0;
            case 't':
                nozzle.throat_radius = atof(optarg);
                break;
            case 'e':
                nozzle.exit_radius = atof(optarg);
                break;
            case 'p':
                conditions.chamber_pressure = atof(optarg);
                break;
            case 'a':
                conditions.ambient_pressure = atof(optarg);
                break;
            case 'T':
                conditions.chamber_temperature = atof(optarg);
                break;
            case 'M':
                conditions.molecular_weight = atof(optarg);
                break;
            case 'g':
                conditions.gamma = atof(optarg);
                break;
            case 'l':
                length_fraction = atof(optarg);
                break;
            case 'o':
                strncpy(output_filename, optarg, MAX_FILENAME - 1);
                output_filename[MAX_FILENAME - 1] = '\0';
                break;
            case 'd':
                strncpy(data_filename, optarg, MAX_FILENAME - 1);
                data_filename[MAX_FILENAME - 1] = '\0';
                break;
            case '?':
                print_usage(argv[0]);
                return 1;
            default:
                break;
        }
    }

    printf("=== ROCKET NOZZLE GEOMETRY CALCULATOR ===\n\n");

    // Validate input parameters
    if (validate_input_parameters(&nozzle, &conditions) != 0) {
        return 1;
    }

    // Print input parameters
    printf("Input Parameters:\n");
    printf("  Throat radius:       %.6f m\n", nozzle.throat_radius);
    printf("  Exit radius:         %.6f m\n", nozzle.exit_radius);
    printf("  Chamber pressure:    %.0f Pa\n", conditions.chamber_pressure);
    printf("  Ambient pressure:    %.0f Pa\n", conditions.ambient_pressure);
    printf("  Chamber temperature: %.0f K\n", conditions.chamber_temperature);
    printf("  Molecular weight:    %.6f kg/mol\n", conditions.molecular_weight);
    printf("  Specific heat ratio: %.3f\n", conditions.gamma);
    printf("  Length fraction:     %.3f\n", length_fraction);
    printf("\n");

    // Calculate nozzle geometry
    printf("Calculating nozzle geometry...\n");
    if (calculate_bell_nozzle_geometry(&nozzle, length_fraction) != 0) {
        printf("Error: Failed to calculate nozzle geometry\n");
        return 1;
    }

    printf("Nozzle geometry calculated:\n");
    printf("  Expansion ratio:     %.3f\n", nozzle.expansion_ratio);
    printf("  Nozzle length:       %.6f m\n", nozzle.exit_x);
    printf("  Number of points:    %d\n", nozzle.num_points);
    printf("\n");

    // Calculate performance
    printf("Calculating performance...\n");
    if (calculate_performance(&nozzle, &conditions, &results) != 0) {
        printf("Error: Failed to calculate performance\n");
        return 1;
    }

    // Print results
    print_performance_results(&results);

    // Generate plot
    printf("Generating nozzle geometry plot...\n");
    if (plot_nozzle_geometry(&nozzle, output_filename) != 0) {
        printf("Warning: Failed to generate plot\n");
    }

    // Write geometry data if requested
    if (strlen(data_filename) > 0) {
        if (write_geometry_data(&nozzle, data_filename) != 0) {
            printf("Warning: Failed to write geometry data\n");
        }
    }

    printf("Calculation completed successfully!\n");
    return 0;
}