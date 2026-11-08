#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct Measurement {
    std::string partCode;
    double measuredValue;
};

struct CheckResult {
    std::string partCode;
    double measuredValue;
    double deviation;
    bool passed;
};

class QualityChecker {
public:
    QualityChecker(double target, double tolerance)
        : target_(target), tolerance_(tolerance) {}

    CheckResult check(const Measurement& measurement) const {
        const double deviation = measurement.measuredValue - target_;
        const bool passed = std::abs(deviation) <= tolerance_;

        return {
            measurement.partCode,
            measurement.measuredValue,
            deviation,
            passed
        };
    }

    std::vector<CheckResult> checkAll(
        const std::vector<Measurement>& measurements
    ) const {
        std::vector<CheckResult> results;
        results.reserve(measurements.size());

        for (const auto& measurement : measurements) {
            results.push_back(check(measurement));
        }

        return results;
    }

    void printReport(const std::vector<Measurement>& measurements) const {
        const auto results = checkAll(measurements);

        int passedCount = 0;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Quality Control Report\n";
        std::cout << "Target: " << target_
                  << " | Tolerance: +/-" << tolerance_ << "\n\n";

        for (const auto& result : results) {
            if (result.passed) {
                ++passedCount;
            }

            std::cout << result.partCode
                      << " | measured: " << result.measuredValue
                      << " | deviation: " << result.deviation
                      << " | result: " << (result.passed ? "PASS" : "FAIL")
                      << '\n';
        }

        const int total = static_cast<int>(results.size());
        const int failedCount = total - passedCount;
        const double passRate =
            total == 0 ? 0.0 : (100.0 * passedCount / total);

        std::cout << "\nSummary\n";
        std::cout << "Passed: " << passedCount << '\n';
        std::cout << "Failed: " << failedCount << '\n';
        std::cout << "Pass rate: " << passRate << "%\n";
    }

private:
    double target_;
    double tolerance_;
};

int main() {
    const QualityChecker checker(50.00, 0.25);

    const std::vector<Measurement> measurements = {
        {"PART-001", 50.10},
        {"PART-002", 49.90},
        {"PART-003", 50.40},
        {"PART-004", 49.76},
        {"PART-005", 50.00}
    };

    checker.printReport(measurements);

    return 0;
}
