#include "datatable.h"
#include <fstream>
#include <sstream>

std::vector<std::string> headers;
std::vector<std::vector<double>> columns;

//public

DataTable::DataTable(const std::string &path)
{
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Could not open " + path);
    }

    std::string line;
    std::getline(in, line);
    headers = split(line, ',');
    columns.resize(headers.size());

    try {
        while (std::getline(in, line)) {
            std::vector<std::string> row = split(line, ',');
            for (int i = 0; i < row.size() && i < columns.size(); i++) {
                double val = row[i] == "" ? 0 : std::stod(row[i]);
                columns[i].push_back(val);
            }
        }
    }
    catch (const std::exception &){
        throw std::runtime_error("Error reading file at " + path);
    }
}

std::vector<double> DataTable::column(const int col) {
    if (col < 0 || col >= columns.size()) {
        throw std::runtime_error("Out of bounds column index");
    }
    return columns[col];
}

std::vector<double> DataTable::column(const std::string header) {
    auto it = std::find(headers.begin(), headers.end(), header);
    if (it == headers.end()) {
        throw std::runtime_error("No column named " + header);
    }
    return column(static_cast<int>(it - headers.begin()));
}

std::vector<double> DataTable::column(const QString header) {
    return column(header.toStdString());
}

QVector<double> DataTable::QVectorColumn(const int col) {
    if (col < 0 || col >= columns.size()) {
        throw std::runtime_error("Out of bounds column index");
    }
    QVector<double> x(columns[col].begin(), columns[col].end());
    return x;
}

QVector<double> DataTable::QVectorColumn(const std::string header) {
    auto it = std::find(headers.begin(), headers.end(), header);
    if (it == headers.end()) {
        throw std::runtime_error("No column named " + header);
    }
    return QVectorColumn(static_cast<int>(it - headers.begin()));
}

QVector<double> DataTable::QVectorColumn(const QString header) {
    return QVectorColumn(header.toStdString());
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
    return parts;
}