#if !defined(_TEST_CONFIG_H_)
#define _TEST_CONFIG_H_

#define TEST_AVAILABLE_LINALG_MAT 1
#define TEST_AVAILABLE_LINALG_VEC (1 && TEST_AVAILABLE_LINALG_MAT)

#define TEST_AVAILABLE_LINALG_COMPLEX 1

#define TEST_AVAILABLE_LINALG_SOLVE (1 && TEST_AVAILABLE_LINALG_MAT)
#define TEST_AVAILABLE_LINALG_UTILS (1 && TEST_AVAILABLE_LINALG_VEC && TEST_AVAILABLE_LINALG_SOLVE)
#define TEST_AVAILABLE_RANDOM (1 && TEST_AVAILABLE_LINALG_MAT)

#define TEST_AVAILABLE_RIGID_TRANSFORM (1 && TEST_AVAILABLE_LINALG_VEC)
#define TEST_AVAILABLE_GEOMETRY_RAY (1 && TEST_AVAILABLE_RIGID_TRANSFORM)
#define TEST_AVAILABLE_SPATIAL_AABB (1 && TEST_AVAILABLE_GEOMETRY_RAY)

#define TEST_AVAILABLE_CV_BASIC (1 && TEST_AVAILABLE_RANDOM)
#define TEST_AVAILABLE_BAZIER (1 && TEST_AVAILABLE_LINALG_MAT)
#define TEST_AVAILABLE_ALL (1)

#endif // _TEST_CONFIG_H_
