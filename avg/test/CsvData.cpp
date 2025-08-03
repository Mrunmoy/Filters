#include "CsvData.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;

static inline void trim(string& s)
{
    auto not_space = [](int ch){ return !std::isspace(ch); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
}

CsvSeries CsvIO::Load(const string& csvPath, const string& xcol, const string& ycol)
{
    ifstream in(csvPath);
    if (!in.is_open())
        throw runtime_error("CsvIO::Load: failed to open: " + csvPath);

    string header;
    if (!getline(in, header))
        throw runtime_error("CsvIO::Load: CSV has no header: " + csvPath);

    // Parse header
    vector<string> cols;
    {
        istringstream hs(header);
        string tok;
        while (getline(hs, tok, ','))
        {
            trim(tok);
            cols.push_back(tok);
        }
    }

    auto find_col = [&](const string& name) -> int {
        for (size_t i = 0; i < cols.size(); ++i)
            if (cols[i] == name) return static_cast<int>(i);
        return -1;
    };

    int xi = find_col(xcol);
    int yi = find_col(ycol);
    if (yi < 0)
        throw runtime_error("CsvIO::Load: y column '" + ycol + "' not found in " + csvPath);

    CsvSeries data;
    string line;
    int k = 0;
    while (getline(in, line))
    {
        if (line.empty()) continue;
        istringstream ls(line);
        string val;
        int col = 0;

        double tval = static_cast<double>(k);
        double yval = 0.0;

        while (getline(ls, val, ','))
        {
            trim(val);
            if (col == yi) yval = stod(val);
            if (xi >= 0 && col == xi) tval = stod(val);
            ++col;
        }

        data.t.push_back(tval);
        data.y.push_back(yval);
        ++k;
    }

    return data;
}

void CsvIO::Write3(const string& csvPath,
                   const vector<double>& t,
                   const vector<double>& y1,
                   const vector<double>& y2,
                   const string& h1,
                   const string& h2,
                   const string& h3)
{
    if (t.size() != y1.size() || t.size() != y2.size())
        throw runtime_error("CsvIO::Write3: vector size mismatch");

    std::filesystem::path outp(csvPath);
    std::filesystem::create_directories(outp.parent_path());

    ofstream out(csvPath, ios::out | ios::trunc);
    if (!out.is_open())
        throw runtime_error("CsvIO::Write3: failed to open: " + csvPath);

    out << h1 << "," << h2 << "," << h3 << "\n";
    out.setf(std::ios::fixed); out.precision(10);

    for (size_t i = 0; i < t.size(); ++i)
        out << t[i] << "," << y1[i] << "," << y2[i] << "\n";
}

double CsvIO::StdDev(const vector<double>& v)
{
    if (v.size() < 2) return 0.0;
    const double mean = accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
    double accum = 0.0;
    for (double x : v) accum += (x - mean) * (x - mean);
    return std::sqrt(accum / static_cast<double>(v.size() - 1));
}
