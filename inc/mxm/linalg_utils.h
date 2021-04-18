#if !defined(_LINALG_UTILS_H)
#define _LINALG_UTILS_H

#include "mxm/linalg_mat.h"
#include <algorithm>

namespace mxm
{

template<typename DType>
inline Matrix<DType> orthogonalComplement(const Matrix<DType>& vs)
{
    int diff = static_cast<int>(vs.shape(0)) - vs.shape(1);
    if(diff == -1)
        return orthogonalComplement(vs.T()).T();
    if(diff != 1)
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));

    Matrix<DType> ret({vs.shape(0), 1});
    for(size_t i = 0; i < vs.shape(0); i++)
    {
        Matrix<DType> adjoint_mat(Matrix<DType>::zeros({vs.shape(1), vs.shape(1)}));

        if(i > 0)
            adjoint_mat(Block({0, i},{})) = vs(Block({0, i},{}));
        if(i < vs.shape(0) - 1)
            adjoint_mat(Block({i, },{})) = vs(Block({i+1,},{}));

        ret(i, 0) = adjoint_mat.det();
        if((i % 2) == 1) ret(i, 0) *= -1;
    }
    return ret;

}

inline size_t argMax(const Vec& v)
{
    size_t idx_max(0);
    FloatType val_max(v(0));
    for(size_t i = 0; i < v.size(); i++)
    {
        if(v(i) > val_max) idx_max = i;
    }
    return idx_max;
}

inline std::vector<size_t> argSort(const Vec& v)
{
    std::vector<size_t> indices(v.size());
    sort(indices.begin(), indices.end(),
        [&v](size_t i1, size_t i2){ return v(i1) < v(i2); });
    return indices;
}

inline size_t min(const Vec& v)
{
    FloatType m(INFINITY);
    for(size_t i = 0; i < v.size(); i++) m = v(i) < m ? v(i) : m;
    return m;
}

inline size_t max(const Vec& v)
{
    FloatType m(INFINITY);
    for(size_t i = 0; i < v.size(); i++) m = v(i) > m ? v(i) : m;
    return m;
}

template <typename DType>
inline Matrix<DType> hstack(const Matrix<DType>& lhs, const Matrix<DType>& rhs)
{
    if(lhs.shape(0) != rhs.shape(0))
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));

    Matrix<DType> ret({lhs.shape(0), lhs.shape(1) + rhs.shape(1)});
    ret.setBlock(0,0,lhs);
    ret.setBlock(0,lhs.shape(1),rhs);
    return ret;
}

template <typename DType>
inline Matrix<DType> vstack(const Matrix<DType>& lhs, const Matrix<DType>& rhs)
{
    return hstack(lhs.T(), rhs.T()).T();
}

inline size_t factorial(size_t x)
{
    size_t ret = 1;
    while(x > 1) ret *= x--;
    return ret;
}

template<typename DType>
Vector<DType> binaryToVector(size_t dim, uint32_t bin)
{
    Vector<DType> ret(Vector<DType>::zeros(dim));
    size_t axis = 0;
    while(axis < dim)
    {
        if(((bin >> axis) & 1u) > 0) ret(axis) = DType(1);
        axis++;
    }
    return ret;
}

} // namespace mxm
#endif // _LINALG_UTILS_H
