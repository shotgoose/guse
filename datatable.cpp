#include "datatable.h"
#include <fstream>
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

    // save headers if not numeric, else reset reader
    std::getline(in, line);
    std::vector<std::string> rawHeaders = split(line, m_delimiter);
    std::vector<std::vector<std::string>> raw(rawHeaders.size());
    if (!allFieldsNumeric(rawHeaders)) {
        m_headers = rawHeaders;
    }
    else {
        m_headers.resize(raw.size());
        in.clear();
        in.seekg(0);
    }

    // read data and save to raw columns
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::vector<std::string> row = split(line, m_delimiter);
        for (int i = 0; i < raw.size(); i++) {
            raw[i].push_back(i < row.size() ? row[i] : "");
        }
    }

    // parse double from numeric columns
    m_columns.resize(raw.size());
    for (int i = 0; i < raw.size(); i++) {
        std::vector<double> nums;
        if (allFieldsNumeric(raw[i], nums)) {
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

std::vector<std::string> DataTable::split(const std::string &s, const char &delim) const {
    std::vector<std::string> parts;
    std::string field;
    bool quote = false;
    field.reserve(32);
    for (std::size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == delim && !quote) {
            parts.push_back(field);
            field.clear();
        }
        else if (c == '"') {
            quote = !quote;
        }
        else {
            field += c;
        }
    }
    if (!field.empty()) parts.push_back(field);
    return parts;
}

bool DataTable::allFieldsNumeric(const std::vector<std::string> &in, std::vector<double> &out) const {
    out.clear();
    out.reserve(in.size());
    for (const auto &s : in) {
        try {
            std::size_t pos = 0;
            double v = s.empty() ? 0.0 : std::stod(s, &pos);
            if (pos != s.size()) {
                throw std::invalid_argument("trailing");
            }
            out.push_back(v);
        }
        catch (std::exception &e) {
            out.clear();
            return false;
        }
    }
    return true;
}

bool DataTable::allFieldsNumeric(const std::vector<std::string> &in) const {
    std::vector<double> out;
    return allFieldsNumeric(in, out);
}