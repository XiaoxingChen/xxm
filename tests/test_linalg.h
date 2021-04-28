#if !defined(_TEST_LINALG_H)
#define _TEST_LINALG_H

#include <iostream>
#include "mxm/linalg.h"
// #include "pixel.h"
// #include "accessor.h"


using namespace mxm;

inline void testOrthogonalComplement()
{
    {
        Mat in({2,3},{0,0,1, 0,1,0});

        if(in(Row(0)).matmul(orthogonalComplement(in).T())(0,0) > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));

        if(in(Row(1)).matmul(orthogonalComplement(in.T()))(0,0) > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    {
        Mat in({3,2},{-0.16224509,0.16913061, 0.08134538,  0., -0.06551123,  0.});
        Mat expected({3,1},{0., -0.01107995, -0.01375799});

        if((orthogonalComplement(in) - expected).norm() > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    if(0){
        Mat in({1,3},{1,0,0});
        Mat complement = orthogonalComplement(in);
        Vec out1 = complement(Row(0));
        Vec out2 = complement(Row(1));

        if(static_cast<Vec>(in).dot(out1) > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));

        if(static_cast<Vec>(in).dot(out2) > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

}

inline void testSolveLowerTriangle()
{
    Mat L({3,3}, {1,0,0, 2,3,0, 4,5,6});
    Vec b({2,3,4});
    Vec expect({2, -1./3, -7./18});
    Vec x = solveLowerTriangle(L, b);
    if((x - expect).norm() > eps())
    {
        std::cout << x.T().str() << std::endl;
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    Vec expect_u({-4./9, -1./9, 2./3});
    Vec x_u = solveUpperTriangle(L.T(), b);
    if((x_u - expect_u).norm() > eps())
    {
        std::cout << x_u.T().str() << std::endl;
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

}

inline void testQRcalcMatQ()
{
    // return;
    if(0)
    {
        Mat mat_a({3,3},{0, 0, 0, 0.431263924, -1, -1, 0.902225852, 0, -1});
        Mat expect_q({3,3},
        {0.00000000e+00,  5.55111512e-17, -1.00000000e+00,
        -4.31263911e-01, -9.02225825e-01, -5.55111512e-17,
        -9.02225825e-01,  4.31263911e-01,  1.11022302e-16});
        Mat mat_q = qr::calcMatQ(mat_a);
        if((mat_q - expect_q).norm() > 2 * eps())
        {
            std::cout << mat_q.str() << std::endl;
            std::cout << "norm: " << (mat_q - expect_q).norm() << std::endl;
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
        }
    }

    {

        Mat m({3,3},{
            0.99845566, 0.94293762, 0.80127313,
            0.6007756 , 0.30253734, 0.78378593,
            0.93970026, 0.781002  , 0.86480691});

        // Mat mat_q = qr::calcMatQFromRotation(m);
        Mat mat_q = qr::decomposeByRotation(m)[0];

        Mat expect_q({3,3},
            {0.66699001,  0.5087728 , 0.54431109,
            0.40133111, -0.86084531, 0.31285571,
            0.62774013,  0.00977734,  -0.77836157});

        if((mat_q - expect_q).norm() > 5 * eps())
        {
            std::cout << mat_q.str() << std::endl;
            std::cout << "norm: " << (mat_q - expect_q).norm() << std::endl;
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
        }

    }
}

inline void testQRSolve()
{
    // using namespace qr;
    Mat mat_a({3,3}, {12, -51, 4, 6, 167, -68, -4, 24, -41});
    Mat expect_q({3,3}, {6./7, -69./175, -58./175, 3./7, 158./175, 6./175, -2./7, 6./35, -33./35});
    Mat mat_q = qr::calcMatQFromReflection(mat_a);
    if((mat_q - expect_q).norm() > 2 * eps())
    {
        std::cout << mat_q.str() << std::endl;
        std::cout << "norm: " << (mat_q - expect_q).norm() << std::endl;
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    Vec b({2,3,9});
    Vec expect({-0.10244898, -0.08359184, -0.25844898});
    Vec x(qr::solve(mat_a, b));

    if((x - expect).norm() > eps())
    {
        std::cout << x.T().str() << std::endl;
        // std::cout << "norm: " << (mat_q - expect_q).norm() << std::endl;
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }
}

// Reference: https://www.cs.utexas.edu/users/flame/laff/alaff-beta/chapter10-QR1-simple-shifted-QR.html
// https://www.cs.utexas.edu/users/flame/laff/alaff-beta/Assignments/Week10/answers/SimpleShiftedQRAlg.m
inline void testQRIteration()
{
    size_t n = 3;
    Mat rot({n,n}, {
        -0.0816791 , -0.43273854,  0.89781172,
        -0.36035366,  0.85270191,  0.3782125 ,
        -0.92923289, -0.29263768, -0.22558684});

    Mat tmp = rot;
    Mat eig_vec = Mat::Identity(n);


    for(size_t i = 0; i < 100; i++)
    {
        FloatType rho = tmp(2,2);
        Mat shift = Mat::Identity(n) * rho;
        auto q_r = qr::decomposeByRotation(tmp - shift);
        tmp = q_r[1].matmul(q_r[0]) + shift;

        eig_vec = eig_vec.matmul(q_r[0]);
    }
    // std::cout << tmp.str() << std::endl;
    Mat recover = eig_vec.matmul(tmp).matmul(eig_vec.T());
    if((recover - rot).norm() > 20 * eps())
    {
        std::cout << (recover - rot).norm() << std::endl;
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

}

inline void testTridiagonalizeSkewSymmetric()
{
    {
        Mat skew({5,5},
        {0, 1, 2, 3, 4,
        -1, 0, 3, 6, 7,
        -2,-3, 0, 8, 2,
        -3,-6,-8, 0, 1,
        -4,-7,-2,-1, 0});

        Mat expected({5,5},
            {0.0000000000000000, -5.4772253036499023, -0.0000000000000000, -0.0000000000000000, 0.0000000000000000,
            5.4772253036499023, 0.0000000000000000, -11.6404466629028320, -0.0000000000000000, 0.0000000000000000,
            0.0000000000000000, 11.6404457092285156, 0.0000000000000000, -3.3462533950805664, -0.0000000000000000,
            0.0000000000000000, 0.0000000000000000, 3.3462533950805664, -0.0000000000000000, -4.0376458168029785,
            -0.0000000000000000, 0.0000000000000000, 0.0000000000000000, 4.0376458168029785, 0.0000000000000000});

        auto result_q_d = tridiagonalizeSkewSymmetric(skew);
        if((result_q_d[1] - expected).norm() > eps() * 30)
        {

            std::cout << "d: \n" << result_q_d[1].str() << "error: " << (result_q_d[1] - expected).norm() << std::endl;
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
        }
    }

    if(0){
        Mat skew({3,3},{
            0, -1, 1,
            1, 0, -1,
            -1,1, 0});

        auto result_q_d = blockDiagonalizeSkewSymmetric(skew);
        std::cout << result_q_d[1].str() << std::endl;
    }

}

inline Mat testMatRefTransposeReturn(size_t dim)
{
    return Mat::Identity(dim).T();
}

inline void testMatRef()
{
    {//test 01
        auto mat = testMatRefTransposeReturn(3);
        if(mat(0,0) != 1.f)
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }//test 01

    {//test 02
        Mat mat_a(Mat::ones({3, 10}));
        MatRef mr(mat_a(Col(1)).T());
        if((mr - Mat::ones({1, 3})).norm() > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }//test 02

    {//test 03
        Mat mat_a(Mat::ones({3, 10}));
        MatRef m1(mat_a(Block({},{3,7})).T());
        MatRef m2(m1(Row(1)).T());
        MatRef m3(m1(Col(1)));
        if((m2 - Mat::ones({3, 1})).norm() > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
        if((m3 - Mat::ones({4, 1})).norm() > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    {//test 04
        Mat vs(Mat::ones({2, 3}));
        MatRef vst = vs.T();
        auto b = vst(Block({1,},{0, end()}));
        if((b(0,1) - 1 )> eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    if(1){//test 05
        Mat vs(Mat::ones({2, 3}));
        auto block = vs(Row(0));
        auto v = block.asVector();
        if((v(2) - 1 )> eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    {//test 06
        Mat vs(Mat::ones({2, 3}));
        vs(Col(1)) = Vec({5,6});
        auto v = vs(Row(0)).asVector();
        if(v.T().shape() != Shape({3,1}))
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
        if(v(1) != 5)
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    {//test 07
        Mat mat(Mat::Identity(3));
        auto v = mat(Col(1));
        auto vt = v.T();
        if((vt - Vec({0,1,0})).norm() > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    {//test 08
        Mat mat(Mat::Identity(3));
        auto b = mat(Col(1));
        auto v = b.asVector();

        if((v - Vec({0,1,0})).norm() > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    {//test 09
        Mat mat = Mat::Identity(3);
        Mat mat_b = mat(Block({0,end()},{0, end()}));
        if(mat.shape() != Shape{3,3})
        {
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
        }
    }
}

inline void testMatInv()
{
    if(1){
        Mat m({3,3},{
            0.99845566, 0.94293762, 0.80127313,
            0.6007756 , 0.30253734, 0.78378593,
            0.93970026, 0.781002  , 0.86480691});

        Mat inv_expect({3,3},
            {-125.49635084,  -67.90837523,  177.82291256,
            77.68518512,   39.56953927, -107.84037325,
            66.20745954,   38.05430802,  -94.67626698});

        if((m.inv() - inv_expect).norm()> 0.01)
        {
            std::cout << m.inv().str() << std::endl;
            std::cout << "Norm: " << (m.inv() - inv_expect).norm() << std::endl;
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
        }

        if((m.inv() - inv_expect).norm() > eps())
        {
            std::cout << "error norm: " << (m.inv() - inv_expect).norm() << std::endl;
            std::cout << "WARNING: todo fix.\n" << std::string(__FILE__) + ":" + std::to_string(__LINE__) << std::endl;
        }

    }
}

inline void testRvalueReference()
{
    Mat a = Mat::ones({100, 100});
    Mat b = std::move(a);
    if(a.shape() != Shape({0,0}))
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
}

// inline void testComplexBase()
// {
//     Complex a({1,2});
//     Complex b({3,1});
//     Complex expected({1,7});

//     std::cout << a.str() << std::endl;
//     if((a * b - expected).norm() > eps())
//         throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
// }

inline void testLinearAlgebra()
{
    Mat m1({3,3},{1,1,1, 2,2,2, 3,3,3});
    Mat v1({3,1},{1,1,1});
    Mat u1(v1.normalized());
    Mat expected({3,1}, {3, 6, 9});

    if((m1.matmul(v1) - expected).norm() > eps())
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));

    if((u1 - v1 * sqrt(1./3)).norm() > eps())
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));

    if(fabs(Mat::Identity(3).det() - 1.) > eps())
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));


    testMatRef();
    testOrthogonalComplement();
    testSolveLowerTriangle();
    testQRcalcMatQ();
    testQRSolve();
    testMatInv();
    testRvalueReference();
    // testComplexBase();
    testTridiagonalizeSkewSymmetric();
    testQRIteration();


}


#endif // _TEST_LINALG_H
