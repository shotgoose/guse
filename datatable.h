#ifndef DATATABLE_H
#define DATATABLE_H
#include <string>
#include <vector>
#include <QVector>
#include <QString>
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
    void append(std::string header, std::vector<double> column);
    void remove(int col);
    void remove(std::string header);
    void rename(int col, std::string header);
    void rename(std::string old, std::string header);
    int find(std::string header) const;
    std::vector<double> column(int col) const;
    std::vector<double> column(std::string header) const;
    std::vector<std::string> textColumn(int col) const;
    std::vector<std::string> textColumn(std::string header) const;
    double mean(std::vector<double> a); // not yet implemented
    double median(std::vector<double> a); // not yet implemented
    double max(std::vector<double> a); // not yet implemented
    double min(std::vector<double> a); // not yet implemented
    double mode(std::vector<double> a, double tolerance); // not yet implemented
    std::vector<double> add(std::vector<double> a, std::vector<double> b);
    std::vector<double> mult(std::vector<double>, double);
    std::vector<double> elprod(std::vector<double> a, std::vector<double> b);
    std::vector<double> reciprocal(std::vector<double> a);
    double dotprod(std::vector<double> a, std::vector<double> b);
    std::vector<double> diff(std::vector<double> dep, std::vector<double> indep); // not yet implemented
    DataTable slice(int start, int end); // not yet implemented
private:
    std::vector<std::string> m_headers;
    std::vector<Column> m_columns;
    std::vector<std::string> split(std::string s, char delim);
    bool isNumeric(std::string s);
};

#endif // DATATABLE_H
