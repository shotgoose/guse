#ifndef DATATABLE_H
#define DATATABLE_H
#include <string>
#include <vector>
#include <QVector>
#include <QString>

class DataTable
{
public:
    DataTable(const std::string &path);
    std::vector<double> column(int col);
    std::vector<double> column(std::string header);
    std::vector<double> column(QString header);
    QVector<double> QVectorColumn(int col);
    QVector<double> QVectorColumn(std::string header);
    QVector<double> QVectorColumn(QString header);
    std::vector<double> add(std::vector<double> a, std::vector<double> b);
    std::vector<double> mult(std::vector<double>, double);
    std::vector<double> elprod(std::vector<double> a, std::vector<double> b);
    std::vector<double> reciprocal(std::vector<double> a);
    double dotprod(std::vector<double> a, std::vector<double> b);
private:
    std::vector<std::string> split(std::string s, char delim);
};

#endif // DATATABLE_H
