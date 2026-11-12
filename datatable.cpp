#include "datatable.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <charconv>
#include <string_view>

//public

DataTable::DataTable(const std::string &path)
{
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Could not open " + path);
    }

    std::string line;
    if (!line.empty() && line.back() == '\r') line.pop_back();

    // determine delimiter
    int max = 0;
    std::string sample;
    m_delimiter = ',';
    for (int i = 0; i < 10 && std::getline(in, line); i++) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        sample.append(line);
    }
    for (char c : {',', '\t', ';', '|', ':'}) {
        int count = std::count(sample.begin(), sample.end(), c);
        if (count > max) {
            m_delimiter = c;
            max = count;
        }
    }
    if (max == 0) {
        m_delimiter = ' ';
    }

    in.clear();
    in.seekg(0);

    // get headers
    std::getline(in, line);
    std::vector<std::string_view> hsv; split(line, hsv, m_delimiter);
    int num_cols = hsv.size();
    if (!numericVector(hsv)) {
        stringVector(hsv, m_headers);
    }
    else {
        m_headers.resize(num_cols);
        in.clear();
        in.seekg(0);
    }

    // read data and save to rows and stringview columns
    std::vector<std::string> srows;
    std::vector<std::vector<std::string_view>> svcols;
    srows.reserve(1000000);
    svcols.reserve(1000000);
    svcols.resize(num_cols);

    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        srows.push_back(line);
        std::vector<std::string_view> svrow; split(srows.back(), svrow, m_delimiter);
        for (std::size_t i = 0; i < svrow.size(); i++) {
            svcols[i].push_back(svrow[i]);
        }
    }

    // parse double from numeric columns, strings otherwise
    m_columns.reserve(num_cols);
    for (auto &c : svcols) {
        std::vector<double> nums;
        if (numericVector(c, nums)) {
            m_columns.push_back(nums);
        }
        else {
            std::vector<std::string> strings; stringVector(c, strings);
            m_columns.push_back(strings);
        }
    }
}

DataTable::DataTable() {}

std::vector<std::string> DataTable::headers() const {
    return m_headers;
}

int DataTable::rowCount() const {
    if (m_columns.empty())
        return 0;
    return std::visit([](const auto &v) { return v.size(); }, m_columns[0]);
}

int DataTable::columnCount() const {
    return m_columns.size();
}

bool DataTable::isNumeric(const int &col) const {
    return std::holds_alternative<std::vector<double>>(m_columns.at(col));
}

void DataTable::append(const std::string &header, const std::vector<double> &column) {
    if (!m_columns.empty() && column.size() != rowCount()) {
        throw std::runtime_error("Inconsistent number of column entries");
    }
    m_headers.push_back(header);
    m_columns.push_back(column);
}

void DataTable::remove(const int &col) {
    if (col < 0 || col >= m_columns.size()) {
        throw std::runtime_error("Out of bounds column index");
    }
    m_headers.erase(m_headers.begin() + col);
    m_columns.erase(m_columns.begin() + col);
}

void DataTable::remove(const std::string &header) {
    remove(find(header));
}

void DataTable::rename(const int &col, const std::string &header) {
    if (col < 0 || col >= m_columns.size()) {
        throw std::runtime_error("Out of bounds column index");
    }
    m_headers[col] = header;
}

void DataTable::rename(const std::string &old, const std::string &header) {
    rename(find(old), header);
}

int DataTable::find(const std::string &header) const {
    auto it = std::find(m_headers.begin(), m_headers.end(), header);
    if (it == m_headers.end()) {
        throw std::runtime_error("No column named " + header);
    }
    return static_cast<int>(it - m_headers.begin());
}

std::vector<double> DataTable::column(const int &col) const {
    if (col < 0 || col >= m_columns.size()) {
        throw std::runtime_error("Out of bounds column index");
    }
    if (!isNumeric(col)) {
        throw std::runtime_error("Column is not numeric");
    }
    return std::get<std::vector<double>>(m_columns.at(col));
}

std::vector<double> DataTable::column(const std::string &header) const {
    return column(find(header));
}

std::vector<std::string> DataTable::textColumn(const int &col) const {
    if (col < 0 || col >= m_columns.size()) {
        throw std::runtime_error("Out of bounds column index");
    }
    if (isNumeric(col)) {
        throw std::runtime_error("Column is not text");
    }
    return std::get<std::vector<std::string>>(m_columns.at(col));
}

std::vector<std::string> DataTable::textColumn(const std::string &header) const {
    return textColumn(find(header));
}

std::vector<double> DataTable::add(const std::vector<double> &a, const std::vector<double> &b) const {
    if (a.size() != b.size()) {
        throw std::runtime_error("Cannot add vectors of different sizes");
    }
    std::vector<double> out(a.size());
    for (int i = 0; i < a.size(); i++) {
        out[i] = a[i] + b[i];
    }
    return out;
}

std::vector<double> DataTable::mult(const std::vector<double> &a, const double &s) const {
    std::vector<double> out(a.size());
    for (int i = 0; i < a.size(); i++) {
        out[i] = a[i] * s;
    }
    return out;
}

std::vector<double> DataTable::elprod(const std::vector<double> &a, const std::vector<double> &b) const {
    if (a.size() != b.size()) {
        throw std::runtime_error("Cannot element wise multiply vectors of different sizes");
    }
    std::vector<double> out(a.size());
    for (int i = 0; i < a.size(); i++) {
        out[i] = a[i] * b[i];
    }
    return out;
}

std::vector<double> DataTable::reciprocal(const std::vector<double> &a) const {
    std::vector<double> out(a.size());
    for (int i = 0; i < a.size(); i++) {
        out[i] = a[i] == 0 ? 0 : 1/a[i];
    }
    return out;
}

double DataTable::dotprod(const std::vector<double> &a, const std::vector<double> &b) const {
    if (a.size() != b.size()) {
        throw std::runtime_error("Cannot dot product vectors of different sizes");
    }
    double out = 0;
    for (int i = 0; i < a.size(); i++) {
        out += a[i] * b[i];
    }
    return out;
}

//private

void DataTable::split(const std::string &s, std::vector<std::string_view> &parts, const char &delim) const {
    bool quote = false;
    size_t last = 0;

    for (std::size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == '"') {
            quote = !quote;
        }
        else if (c == delim && !quote) {
            std::string_view sv(s.data() + last, i - last);
            last = i + 1;
            while (!sv.empty() && sv[0] == '"' || sv[0] == ' ') {
                sv.remove_prefix(1);
            }
            while (!sv.empty() && sv[sv.size() - 1] == '"' || sv[sv.size() - 1] == ' ') {
                sv.remove_suffix(1);
            }
            parts.push_back(sv);
        }

    }

    if (last <= s.size()) {
        std::string_view sv(s.data() + last, s.size() - last);
        while (!sv.empty() && ((sv.front() == ' ' || sv.front() == '"'))) {
            sv.remove_prefix(1);
        }
        while (!sv.empty() && ((sv.back() == ' ' || sv.back() == '"'))) {
            sv.remove_suffix(1);
        }
        parts.push_back(sv);
    }
}

void DataTable::stringVector(const std::vector<std::string_view> &in, std::vector<std::string> &out) const {
    out.clear();
    out.reserve(in.size());

    for (auto &s : in) {
        out.push_back(std::string(s));
    }
}

bool DataTable::numericVector(const std::vector<std::string_view> &in, std::vector<double> &out) const {
    out.clear();
    out.reserve(in.size());

    for (auto &s : in) {
        double val = 0.0;
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), val);
        if (ec != std::errc() || ptr != s.data() + s.size()) {
            out.clear();
            return false;
        }
        out.push_back(val);
    }
    return true;
}

bool DataTable::numericVector(const std::vector<std::string_view> &in) const {
    std::vector<double> out;
    return numericVector(in, out);
}