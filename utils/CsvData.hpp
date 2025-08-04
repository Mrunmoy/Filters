#pragma once
#include <string>
#include <vector>

struct CsvSeries
{
    std::vector<double> t;  // time (optional; synthesized if missing)
    std::vector<double> y;  // signal
};

class CsvIO
{
public:
    // Load CSV with header; locate x/y by column names.
    // If xcol is missing, synthesizes t = 0..N-1
    // Throws std::runtime_error on errors.
    static CsvSeries Load(const std::string& csvPath,
                          const std::string& xcol = "t",
                          const std::string& ycol = "z");

    // Write 3-column CSV with a header (default names: t, y, avg).
    static void Write3(const std::string& csvPath,
                       const std::vector<double>& t,
                       const std::vector<double>& y1,
                       const std::vector<double>& y2,
                       const std::string& h1 = "t",
                       const std::string& h2 = "y",
                       const std::string& h3 = "avg");

    // Population standard deviation (N-1 in the denominator).
    static double StdDev(const std::vector<double>& v);
};
