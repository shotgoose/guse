#include "datatable.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

//public

DataTable::DataTable(const std::string &path)
{
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Could not open " + path);
    }

    std::string line;
    std::getline(in, line);
    if (!line.empty() && line.back() == '\r') line.pop_back();
    m_headers = split(line, ',');
    m_columns.resize(m_headers.size());

    std::vector<std::vector<std::string>> raw(m_columns.size());
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::vector<std::string> row = split(line, ',');
        for (int i = 0; i < m_columns.size(); i++) {
            raw[i].push_back(i < row.size() ? row[i] : "");
        }
    }

    for (int i = 0; i < raw.size(); i++) {
        std::vector<double> nums;
        nums.reserve(raw[i].size());
        bool allNumeric = true;
        for (const auto &s : raw[i]) {
            try {
                std::size_t pos = 0;
                double v = s.empty() ? 0.0 : std::stod(s, &pos);
                if (pos != s.size()) {
                    throw std::invalid_argument("trailing");
                }
                nums.push_back(v);
            }
            catch (std::exception &e) {
                allNumeric = false;
                break;
            }
        }
        if (allNumeric) {
            m_columns[i] = std::move(nums);
        }
        else {
            m_columns[i] = std::move(raw[i]);
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

bool DataTable::isNumeric(int col) const
{
    return std::holds_alternative<std::vector<double>>(m_columns.at(col));
}

void DataTable::append(const std::string header, const std::vector<double> column) {
    if (!m_columns.empty() && column.size() != rowCount()) {
        throw std::runtime_error("Inconsistent number of column entries");
    }
    m_headers.push_back(header);
    m_columns.push_back(column);
}

void DataTable::remove(const int col) {
    if (col < 0 || col >= m_columns.size()) {
        throw std::runtime_error("Out of bounds column index");
    }
    m_headers.erase(m_headers.begin() + col);
    m_columns.erase(m_columns.begin() + col);
}

void DataTable::remove(const std::string header) {
    remove(find(header));
}

void DataTable::rename(int col, std::string header) {
    if (col < 0 || col >= m_columns.size()) {
        throw std::runtime_error("Out of bounds column index");
    }
    m_headers[col] = header;
}

void DataTable::rename(std::string old, std::string header) {
    rename(find(old), header);
}

int DataTable::find(const std::string header) const {
    auto it = std::find(m_headers.begin(), m_headers.end(), header);
    if (it == m_headers.end()) {
        throw std::runtime_error("No column named " + header);
    }
    return static_cast<int>(it - m_headers.begin());
}

std::vector<double> DataTable::column(const int col) const {
    if (col < 0 || col >= m_columns.size()) {
        throw std::runtime_error("Out of bounds column index");
    }
    if (!isNumeric(col)) {
        throw std::runtime_error("Column is not numeric");
    }
    return std::get<std::vector<double>>(m_columns.at(col));
}

std::vector<double> DataTable::column(const std::string header) const {
    return column(find(header));
}

std::vector<std::string> DataTable::textColumn(const int col) const {
    if (col < 0 || col >= m_columns.size()) {
        throw std::runtime_error("Out of bounds column index");
    }
    if (isNumeric(col)) {
        throw std::runtime_error("Column is not text");
    }
    return std::get<std::vector<std::string>>(m_columns.at(col));
}

std::vector<std::string> DataTable::textColumn(const std::string header) const {
    return textColumn(find(header));
}

std::vector<double> DataTable::add(std::vector<double> a, std::vector<double> b) {
    if (a.size() != b.size()) {
        throw std::runtime_error("Cannot add vectors of different sizes");
    }
    std::vector<double> out(a.size());
    for (int i = 0; i < a.size(); i++) {
        out[i] = a[i] + b[i];
    }
    return out;
}

std::vector<double> DataTable::mult(std::vector<double> a, double s) {
    std::vector<double> out(a.size());
    for (int i = 0; i < a.size(); i++) {
        out[i] = a[i] * s;
    }
    return out;
}

std::vector<double> DataTable::elprod(std::vector<double> a, std::vector<double> b) {
    if (a.size() != b.size()) {
        throw std::runtime_error("Cannot element wise multiply vectors of different sizes");
    }
    std::vector<double> out(a.size());
    for (int i = 0; i < a.size(); i++) {
        out[i] = a[i] * b[i];
    }
    return out;
}

std::vector<double> DataTable::reciprocal(std::vector<double> a) {
    std::vector<double> out(a.size());
    for (int i = 0; i < a.size(); i++) {
        out[i] = a[i] == 0 ? 0 : 1/a[i];
    }
    return out;
}

double DataTable::dotprod(std::vector<double> a, std::vector<double> b) {
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

std::vector<std::string> DataTable::split(std::string s, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        parts.push_back(item);
    if (!s.empty() && s.back() == delim)
        parts.push_back("");
    return parts;
}