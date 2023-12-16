#include <iostream>

struct PIDController {
    double kp, ki, kd;
    double previous_error, integral;

    PIDController(double p, double i, double d) : kp(p), ki(i), kd(d), previous_error(0), integral(0) {}

    double calculate(double setpoint, double process_variable) {
        double error = setpoint - process_variable;
        integral += error;
        double derivative = error - previous_error;
        double output = kp * error + ki * integral + kd * derivative;
        previous_error = error;
        return output;
    }
};

int main() {
    // Example usage
    PIDController pid_controller{0.1, 0.01, 0.05};

    double setpoint = 100.0;  // Desired setpoint
    double process_variable = 0.0;  // Initial process variable

    for (int i = 0; i < 100; ++i) {
        double control_output = pid_controller.calculate(setpoint, process_variable);
        process_variable += control_output;

        std::cout << "Iteration " << i << ": Setpoint = " << setpoint
                  << ", Process Variable = " << process_variable
                  << ", Control Output = " << control_output << std::endl;
    }

    return 0;
}