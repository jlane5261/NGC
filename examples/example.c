#include "../include/ngc.h"

// Example program demonstrating how to use the NGC library programmatically
int main() {
    // Initialize data structures
    NozzleGeometry nozzle = {0};
    FlowConditions conditions = {0};
    PerformanceResults results = {0};

    // Set up nozzle geometry parameters
    nozzle.throat_radius = 0.008;      // 8 mm throat radius
    nozzle.exit_radius = 0.032;        // 32 mm exit radius

    // Set up flow conditions (liquid propellant engine example)
    conditions.chamber_pressure = 2.5e6;    // 2.5 MPa
    conditions.ambient_pressure = 0.0;      // Vacuum conditions
    conditions.chamber_temperature = 3600;   // 3600 K
    conditions.molecular_weight = 0.022;     // 22 g/mol
    conditions.gamma = 1.25;                 // Typical for LOX/RP-1
    conditions.gas_constant = 8314.5;        // J/(kmol·K)

    printf("=== NGC LIBRARY EXAMPLE ===\n\n");

    // Validate parameters
    if (validate_input_parameters(&nozzle, &conditions) != 0) {
        printf("Error: Invalid input parameters\n");
        return 1;
    }

    // Calculate nozzle geometry (80% bell length)
    double length_fraction = 0.8;
    if (calculate_bell_nozzle_geometry(&nozzle, length_fraction) != 0) {
        printf("Error: Failed to calculate nozzle geometry\n");
        return 1;
    }

    printf("Nozzle Geometry:\n");
    printf("  Throat radius: %.6f m\n", nozzle.throat_radius);
    printf("  Exit radius: %.6f m\n", nozzle.exit_radius);
    printf("  Expansion ratio: %.3f\n", nozzle.expansion_ratio);
    printf("  Nozzle length: %.6f m\n", nozzle.exit_x);
    printf("\n");

    // Calculate performance
    if (calculate_performance(&nozzle, &conditions, &results) != 0) {
        printf("Error: Failed to calculate performance\n");
        return 1;
    }

    // Print results
    print_performance_results(&results);

    // Generate geometry data file
    write_geometry_data(&nozzle, "example_nozzle_geometry.dat");

    // Generate plot
    plot_nozzle_geometry(&nozzle, "example_nozzle_plot.png");

    printf("Example completed! Check the generated files:\n");
    printf("  - example_nozzle_geometry.dat\n");
    printf("  - example_nozzle_plot.png (if gnuplot is available)\n");
    printf("  - nozzle_upper.dat and nozzle_lower.dat\n");

    return 0;
}