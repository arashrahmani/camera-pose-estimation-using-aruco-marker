#ifndef Matrix_h_DEFINED
#define Matrix_h_DEFINED

#include <vector>
namespace Arash{
  class squareMat{
  private:
    std::vector<std::vector<long double>> elements;
  public:
    int size;
    squareMat(){};
    squareMat(int);
    squareMat(std::vector<std::vector<long double>>v,int s):size(s),elements(v){};
    ~squareMat();
    void getCofactor(std::vector<std::vector<long double>> ,std::vector<std::vector<long double>>& , int p, int q,int n); 
    void getCofactor(long double** ,long double** ,int ,int ,int );
    long double det(std::vector<std::vector<long double>>& ,int n);
    long double det(long double** ,int);
    long double det();
    void inverse(squareMat&);
    squareMat inverse();
    void transpose(squareMat&);
    squareMat transpose();
    void minor(squareMat&);
    squareMat adjoint();
    long double& operator()(int ,int);
    long double operator()(int ,int) const;
    squareMat& operator=(const squareMat&);
  };
}
#endif
