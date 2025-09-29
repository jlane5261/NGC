#include "../include/ngc.h"

int calculate_bell_nozzle_geometry(NozzleGeometry* nozzle, double length_fraction) {
    if (!nozzle || length_fraction <= 0 || length_fraction > 1.0) {
        return -1;
    }

    // Calculate basic parameters
    nozzle->expansion_ratio = (nozzle->exit_radius * nozzle->exit_radius) / 
                              (nozzle->throat_radius * nozzle->throat_radius);
    
    // Set throat position at x = 0
    nozzle->throat_x = 0.0;
    
    // Calculate nozzle length based on the 15-degree half-angle conical equivalent
    double conical_length = (nozzle->exit_radius - nozzle->throat_radius) / tan(15.0 * PI / 180.0);
    double bell_length = conical_length * length_fraction;
    nozzle->exit_x = bell_length;

    // Generate bell nozzle contour using Rao's method approximation
    int i;
    double dx = bell_length / (MAX_POINTS - 1);
    nozzle->num_points = 0;

    for (i = 0; i < MAX_POINTS && nozzle->num_points < MAX_POINTS; i++) {
        double x = i * dx;
        double radius;

        if (x <= 0) {
            // Throat region
            radius = nozzle->throat_radius;
        } else {
            // Bell expansion using parabolic approximation
            double x_norm = x / bell_length;
            
            // Parabolic expansion from throat to exit
            double radius_ratio = 1.0 + (nozzle->exit_radius / nozzle->throat_radius - 1.0) * 
                                 (2.0 * x_norm - x_norm * x_norm);
            radius = nozzle->throat_radius * radius_ratio;
            
            // Ensure radius doesn't exceed exit radius
            if (radius > nozzle->exit_radius) {
                radius = nozzle->exit_radius;
            }
        }

        nozzle->geometry[nozzle->num_points].x = x;
        nozzle->geometry[nozzle->num_points].y = radius;
        nozzle->num_points++;
    }

    return 0;
}

int calculate_expansion_ratio(NozzleGeometry* nozzle) {
    if (!nozzle || nozzle->throat_radius <= 0) {
        return -1;
    }

    nozzle->expansion_ratio = (nozzle->exit_radius * nozzle->exit_radius) / 
                              (nozzle->throat_radius * nozzle->throat_radius);
    return 0;
}

double calculate_nozzle_area(double radius) {
    return PI * radius * radius;
}