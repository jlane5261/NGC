#include "../include/ngc.h"

int plot_nozzle_geometry(const NozzleGeometry* nozzle, const char* filename) {
    if (!nozzle || !filename) {
        return -1;
    }

    FILE* gnuplot_script = fopen("plot_nozzle.gp", "w");
    if (!gnuplot_script) {
        printf("Error: Cannot create gnuplot script file\n");
        return -1;
    }

    // Write gnuplot script
    fprintf(gnuplot_script, "set terminal png size 800,600\n");
    fprintf(gnuplot_script, "set output '%s'\n", filename);
    fprintf(gnuplot_script, "set title 'Bell Nozzle Geometry'\n");
    fprintf(gnuplot_script, "set xlabel 'Axial Position (m)'\n");
    fprintf(gnuplot_script, "set ylabel 'Radius (m)'\n");
    fprintf(gnuplot_script, "set grid\n");
    fprintf(gnuplot_script, "set key top left\n");
    
    // Plot upper and lower contours
    fprintf(gnuplot_script, "plot 'nozzle_upper.dat' with lines title 'Upper Contour', \\\n");
    fprintf(gnuplot_script, "     'nozzle_lower.dat' with lines title 'Lower Contour'\n");
    
    fclose(gnuplot_script);

    // Create upper contour data file
    FILE* upper_file = fopen("nozzle_upper.dat", "w");
    if (!upper_file) {
        printf("Error: Cannot create upper contour data file\n");
        return -1;
    }

    // Create lower contour data file
    FILE* lower_file = fopen("nozzle_lower.dat", "w");
    if (!lower_file) {
        printf("Error: Cannot create lower contour data file\n");
        fclose(upper_file);
        return -1;
    }

    // Write geometry data
    for (int i = 0; i < nozzle->num_points; i++) {
        fprintf(upper_file, "%.6f %.6f\n", nozzle->geometry[i].x, nozzle->geometry[i].y);
        fprintf(lower_file, "%.6f %.6f\n", nozzle->geometry[i].x, -nozzle->geometry[i].y);
    }

    fclose(upper_file);
    fclose(lower_file);

    // Execute gnuplot (if available)
    int result = system("gnuplot plot_nozzle.gp 2>/dev/null");
    if (result == 0) {
        printf("Nozzle geometry plot saved to %s\n", filename);
    } else {
        printf("Gnuplot not available. Data files created: nozzle_upper.dat, nozzle_lower.dat\n");
        printf("You can manually plot using: gnuplot plot_nozzle.gp\n");
    }

    return 0;
}

int write_geometry_data(const NozzleGeometry* nozzle, const char* filename) {
    if (!nozzle || !filename) {
        return -1;
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Cannot create geometry data file %s\n", filename);
        return -1;
    }

    fprintf(file, "# Nozzle Geometry Data\n");
    fprintf(file, "# Throat radius: %.6f m\n", nozzle->throat_radius);
    fprintf(file, "# Exit radius: %.6f m\n", nozzle->exit_radius);
    fprintf(file, "# Expansion ratio: %.3f\n", nozzle->expansion_ratio);
    fprintf(file, "# Number of points: %d\n", nozzle->num_points);
    fprintf(file, "#\n");
    fprintf(file, "# X (m)\t\tY (m)\n");

    for (int i = 0; i < nozzle->num_points; i++) {
        fprintf(file, "%.6f\t\t%.6f\n", nozzle->geometry[i].x, nozzle->geometry[i].y);
    }

    fclose(file);
    printf("Geometry data written to %s\n", filename);
    return 0;
}

int print_performance_results(const PerformanceResults* results) {
    if (!results) {
        return -1;
    }

    printf("\n=== NOZZLE PERFORMANCE RESULTS ===\n");
    printf("Thrust:                  %.2f N\n", results->thrust);
    printf("Specific Impulse:        %.2f s\n", results->specific_impulse);
    printf("Exit Velocity:           %.2f m/s\n", results->exit_velocity);
    printf("Exit Pressure:           %.2f Pa\n", results->exit_pressure);
    printf("Exit Temperature:        %.2f K\n", results->exit_temperature);
    printf("Mass Flow Rate:          %.6f kg/s\n", results->mass_flow_rate);
    printf("Characteristic Velocity: %.2f m/s\n", results->characteristic_velocity);
    printf("Thrust Coefficient:      %.4f\n", results->thrust_coefficient);
    printf("==================================\n\n");

    return 0;
}