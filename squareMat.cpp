
#include "squareMat.h"
using namespace Arash;
squareMat::squareMat(int s):size(s)
{
  elements = std::vector<std::vector<long double> >( size , std::vector<long double> (size,0));
}
squareMat::~squareMat(){}
void squareMat::getCofactor(long double** mat,long double** temp, int p, int q,int n) 
{ 
  int i = 0, j = 0; 
  for (int row = 0; row < n; row++) 
  { 
    for (int col = 0; col < n; col++) 
    { 
      if (row != p && col != q) 
      { 
        temp[i][j++] = mat[row][col]; 
        if (j == n - 1) 
        { 
          j = 0; 
          i++; 
        } 
      } 
    } 
  } 
}
void squareMat::getCofactor(std::vector<std::vector<long double>> mat,std::vector<std::vector<long double>>& temp, int p, int q,int n) 
{ 
  int i = 0, j = 0; 
  for (int row = 0; row < n; row++) 
  { 
    for (int col = 0; col < n; col++) 
    { 
      if (row != p && col != q) 
      { 
        temp[i][j++] = mat[row][col]; 
        if (j == n - 1) 
        { 
          j = 0; 
          i++; 
        } 
      } 
    } 
  }
}
long double squareMat::det(std::vector<std::vector<long double>>& mat,int n)
{
  double D = 0;
  std::vector<std::vector<long double> > temp( n , std::vector<long double> (n,0)); 
  int sign = 1;
  if(n == 1)
    return mat[0][0];
  for(int col = 0;col < n;col ++)
  {
    getCofactor(mat,temp,0,col,n);
//     std::cout<<"i'm here:    \n";
//     for(int i=0;i<n;i++){
//       std::cout<<std::endl;
//       for(int j=0;j<n;j++){
//         std::cout<<temp[i][j]<<"\t";
//       }
//     }
//     std::cout<<"\n\n\n";
    D += sign * mat[0][col] * det(temp,n-1);
    sign *= -1;
  }
  return D;
}
long double squareMat::det(long double** mat,int n)
{
  int D = 0;
  long double** temp;
  temp = new long double*[n];
  int sign = 1;
  for(int i = 0;i < n;i ++)
  {
    temp[i] = new long double[n];
  }
  if(n == 1)
    return mat[0][0];
  for(int col = 0;col < n;col ++)
  {
    getCofactor(mat,temp,0,col,n);
    D += sign * mat[0][col] * det(temp,n-1);
    sign *= -1;
  }
  delete[]* temp;
  return D;
}
long double squareMat::det()
{
  long double** temp;
  temp = new long double*[size];
  for(int i = 0;i < size;i ++)
  {
    temp[i] = new long double[size];
    for(int j = 0;j < size;j ++)
      temp[i][j] = elements[i][j];
  }
//   std::cout<<"fucking bug is\n";
//   for(int i = 0;i < size;i ++)
//   {
//     std::cout<<std::endl;
//     for(int j = 0;j < size;j ++)
//     {
//       std::cout<<temp[i][j]<<"\t";
//     }
//   }
  return det(temp,size);
}
  
squareMat squareMat::adjoint()
{
  std::vector<std::vector<long double> > ans( size , std::vector<long double> (size,0)); 
  std::vector<std::vector<long double> > tempMat( size , std::vector<long double> (size,0)); ;
  int sign;
  for(int row = 0;row < size;row ++)
  {
    for(int col = 0;col < size;col ++)
    {
      getCofactor(elements,tempMat,row,col,size);
      sign = ((row+col)%2==0)? 1: -1; 
      ans[col][row] = sign * det(tempMat,size-1);
    }
  }
  return squareMat(ans ,size);
}

squareMat squareMat::inverse()
{
  long double determinant = det();
  squareMat tempAdjoint = adjoint();
  squareMat answer(size);
  for(int i = 0;i < size;i++)
  {
    for(int j = 0;j < size;j++)
    {
      answer(i,j) = tempAdjoint(i,j) / (long double)determinant;
    }
  }
  return answer;
}

long double& Arash::squareMat::operator() (int i, int j) {
  return elements[i][j];
}


long double squareMat::operator()(int i, int j) const
{
  return elements[i][j];
}

squareMat& squareMat::operator=(const squareMat& rhs)
{
  for(int i = 0;i<size;i++)
    for(int j = 0;j<size;j++)
      elements[i][j] = rhs(i,j);
  return (*this);
}

