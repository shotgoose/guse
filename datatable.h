#ifndef DATATABLE_H
#define DATATABLE_H
#include <string>
#include <vector>
#include <QVector>
#include <QString>
#include <QVariant>
#include <variant>

using Column = std::variant<std::vector<double>, std::vector<std::string>>;

class DataTable
{
public:
    DataTable(const std::string &path);
    DataTable();
    std::vector<std::string> headers() const;
    int rowCount() const;
    int columnCount() const;
    bool isNumeric(int col) const;
    void append(const std::string &header, const std::vector<double> &column);
    void remove(int col);
    void remove(const std::string &header);
    void rename(int col, const std::string &header);
    void rename(const std::string &old, const std::string &header);
    int find(const std::string &header) const;
    Column column(int col) const;
    Column column(const std::string &header) const;
    QVariant entry(int row, int col) const;
    std::vector<std::string> textColumn(int col) const;
    std::vector<std::string> textColumn(const std::string &header) const;
    double mean(const std::vector<double> &a) const; // not yet implemented
    double median(const std::vector<double> &a) const; // not yet implemented
    double max(const std::vector<double> &a) const; // not yet implemented
    double min(const std::vector<double> &a) const; // not yet implemented
    double mode(const std::vector<double> &a, const double &tolerance) const; // not yet implemented
    std::vector<double> add(const std::vector<double> &a, const std::vector<double> &b) const;
    std::vector<double> mult(const std::vector<double> &a, const double &s) const;
    std::vector<double> elprod(const std::vector<double> &a, const std::vector<double> &b) const;
    std::vector<double> reciprocal(const std::vector<double> &a) const;
    double dotprod(const std::vector<double> &a, const std::vector<double> &b) const;
    std::vector<double> diff(const std::vector<double> &dep, const std::vector<double> &indep) const; // not yet implemented
    DataTable slice(int start, int end) const; // not yet implemented

private:
    std::vector<std::string> m_headers;
    std::vector<Column> m_columns;
    char m_delimiter;
    void split(const std::string &s, std::vector<std::string_view> &parts, const char &delim) const;
    void stringVector(const std::vector<std::string_view> &in, std::vector<std::string> &out) const;
    bool numericVector(const std::vector<std::string_view> &in, std::vector<double> &out) const;
    bool numericVector(const std::vector<std::string_view> &in) const;
};

#endif // DATATABLE_H
