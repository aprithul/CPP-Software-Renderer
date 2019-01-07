#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include "Utils.hpp"

namespace utils
{
    class Matrix4x4d
    {
        private:
            double matrix_array[4][4];
        public:
            Matrix4x4d( double _0_0, double _0_1, double _0_2, double _0_3,
                        double _1_0, double _1_1, double _1_2, double _1_3,
                        double _2_0, double _2_1, double _2_2, double _2_3,
                        double _3_0, double _3_1, double _3_2, double _3_3 )
            {
                matrix_array[0][0] = _0_0;
                matrix_array[0][1] = _0_1;
                matrix_array[0][2] = _0_2;
                matrix_array[0][3] = _0_3;
                matrix_array[1][0] = _1_0;
                matrix_array[1][1] = _1_1;
                matrix_array[1][2] = _1_2;
                matrix_array[1][3] = _1_3;
                matrix_array[2][0] = _2_0;
                matrix_array[2][1] = _2_1;
                matrix_array[2][2] = _2_2;
                matrix_array[2][3] = _2_3;
                matrix_array[3][0] = _3_0;
                matrix_array[3][1] = _3_1;
                matrix_array[3][2] = _3_2;
                matrix_array[3][3] = _3_3;
            }
            
            void set_value_at(int row, int col, double value)
            {
                matrix_array[row][col] = value;
            }

            double get_value_at(int row, int col) const
            {
                return matrix_array[row][col];
            }

            Matrix4x4d operator+(const Matrix4x4d& mat) const
            {
                return Matrix4x4d(  matrix_array[0][0] + mat.get_value_at(0,0), matrix_array[0][1] + mat.get_value_at(0,1),  matrix_array[0][2] + mat.get_value_at(0,2), matrix_array[0][3] + mat.get_value_at(0,3),
                                    matrix_array[1][0] + mat.get_value_at(1,0), matrix_array[1][1] + mat.get_value_at(1,1),  matrix_array[1][2] + mat.get_value_at(1,2), matrix_array[1][3] + mat.get_value_at(1,3),
                                    matrix_array[2][0] + mat.get_value_at(2,0), matrix_array[2][1] + mat.get_value_at(2,1),  matrix_array[2][2] + mat.get_value_at(2,2), matrix_array[2][3] + mat.get_value_at(2,3), 
                                    matrix_array[3][0] + mat.get_value_at(3,0), matrix_array[3][1] + mat.get_value_at(3,1),  matrix_array[3][2] + mat.get_value_at(3,2), matrix_array[3][3] + mat.get_value_at(3,3));
            
            }

            Matrix4x4d operator-(const Matrix4x4d& mat) const
            {
                return Matrix4x4d(  matrix_array[0][0] - mat.get_value_at(0,0), matrix_array[0][1] - mat.get_value_at(0,1),  matrix_array[0][2] - mat.get_value_at(0,2), matrix_array[0][3] - mat.get_value_at(0,3),
                                    matrix_array[1][0] - mat.get_value_at(1,0), matrix_array[1][1] - mat.get_value_at(1,1),  matrix_array[1][2] - mat.get_value_at(1,2), matrix_array[1][3] - mat.get_value_at(1,3),
                                    matrix_array[2][0] - mat.get_value_at(2,0), matrix_array[2][1] - mat.get_value_at(2,1),  matrix_array[2][2] - mat.get_value_at(2,2), matrix_array[2][3] - mat.get_value_at(2,3), 
                                    matrix_array[3][0] - mat.get_value_at(3,0), matrix_array[3][1] - mat.get_value_at(3,1),  matrix_array[3][2] - mat.get_value_at(3,2), matrix_array[3][3] - mat.get_value_at(3,3));
            
            }

            Matrix4x4d operator*(const Matrix4x4d& mat) const
            {
               // std::cout<<matrix_array[0][0] * mat.get_value_at(0,0) <<" "<< matrix_array[0][1] * mat.get_value_at(1,0) <<" "<<  matrix_array[0][2] + mat.get_value_at(2,0) + matrix_array[0][3] + mat.get_value_at(3,0),<<std::endl;
                return Matrix4x4d(  matrix_array[0][0] * mat.get_value_at(0,0) + matrix_array[0][1] * mat.get_value_at(1,0) +  matrix_array[0][2] * mat.get_value_at(2,0) + matrix_array[0][3] * mat.get_value_at(3,0),
                                        matrix_array[0][0] * mat.get_value_at(0,1) + matrix_array[0][1] * mat.get_value_at(1,1) +  matrix_array[0][2] * mat.get_value_at(2,1) + matrix_array[0][3] * mat.get_value_at(3,1),
                                            matrix_array[0][0] * mat.get_value_at(0,2) + matrix_array[0][1] * mat.get_value_at(1,2) +  matrix_array[0][2] * mat.get_value_at(2,2) + matrix_array[0][3] * mat.get_value_at(3,2),
                                                matrix_array[0][0] * mat.get_value_at(0,3) + matrix_array[0][1] * mat.get_value_at(1,3) +  matrix_array[0][2] * mat.get_value_at(2,3) + matrix_array[0][3] * mat.get_value_at(3,3),
                                     
                                    matrix_array[1][0] * mat.get_value_at(0,0) + matrix_array[1][1] * mat.get_value_at(1,0) +  matrix_array[1][2] * mat.get_value_at(2,0) + matrix_array[1][3] * mat.get_value_at(3,0),
                                        matrix_array[1][0] * mat.get_value_at(0,1) + matrix_array[1][1] * mat.get_value_at(1,1) +  matrix_array[1][2] * mat.get_value_at(2,1) + matrix_array[1][3] * mat.get_value_at(3,1),
                                            matrix_array[1][0] * mat.get_value_at(0,2) + matrix_array[1][1] * mat.get_value_at(1,2) +  matrix_array[1][2] * mat.get_value_at(2,2) + matrix_array[1][3] * mat.get_value_at(3,2),
                                                matrix_array[1][0] * mat.get_value_at(0,3) + matrix_array[1][1] * mat.get_value_at(1,3) +  matrix_array[1][2] * mat.get_value_at(2,3) + matrix_array[1][3] * mat.get_value_at(3,3),
                                    
                                    matrix_array[2][0] * mat.get_value_at(0,0) + matrix_array[2][1] * mat.get_value_at(1,0) +  matrix_array[2][2] * mat.get_value_at(2,0) + matrix_array[2][3] * mat.get_value_at(3,0),
                                        matrix_array[2][0] * mat.get_value_at(0,1) + matrix_array[2][1] * mat.get_value_at(1,1) +  matrix_array[2][2] * mat.get_value_at(2,1) + matrix_array[2][3] * mat.get_value_at(3,1),
                                            matrix_array[2][0] * mat.get_value_at(0,2) + matrix_array[2][1] * mat.get_value_at(1,2) +  matrix_array[2][2] * mat.get_value_at(2,2) + matrix_array[2][3] * mat.get_value_at(3,2),
                                                matrix_array[2][0] * mat.get_value_at(0,3) + matrix_array[2][1] * mat.get_value_at(1,3) +  matrix_array[2][2] * mat.get_value_at(2,3) + matrix_array[2][3] * mat.get_value_at(3,3),
                                    
                                     matrix_array[3][0] * mat.get_value_at(0,0) + matrix_array[3][1] * mat.get_value_at(1,0) +  matrix_array[3][2] * mat.get_value_at(2,0) + matrix_array[3][3] * mat.get_value_at(3,0),
                                        matrix_array[3][0] * mat.get_value_at(0,1) + matrix_array[3][1] * mat.get_value_at(1,1) +  matrix_array[3][2] * mat.get_value_at(2,1) + matrix_array[3][3] * mat.get_value_at(3,1),
                                            matrix_array[3][0] * mat.get_value_at(0,2) + matrix_array[3][1] * mat.get_value_at(1,2) +  matrix_array[3][2] * mat.get_value_at(2,2) + matrix_array[3][3] * mat.get_value_at(3,2),
                                                matrix_array[3][0] * mat.get_value_at(0,3) + matrix_array[3][1] * mat.get_value_at(1,3) +  matrix_array[3][2] * mat.get_value_at(2,3) + matrix_array[3][3] * mat.get_value_at(3,3));

            }

            utils::Vector4d operator*(const utils::Vector4d& vec) const
            {
                utils::Vector4d _v(  matrix_array[0][0] * vec.x + matrix_array[0][1] * vec.y +  matrix_array[0][2] * vec.z + matrix_array[0][3] * vec.w,
                                     matrix_array[1][0] * vec.x + matrix_array[1][1] * vec.y +  matrix_array[1][2] * vec.z + matrix_array[1][3] * vec.w,
                                     matrix_array[2][0] * vec.x + matrix_array[2][1] * vec.y +  matrix_array[2][2] * vec.z + matrix_array[2][3] * vec.w,
                                     matrix_array[3][0] * vec.x + matrix_array[3][1] * vec.y +  matrix_array[3][2] * vec.z + matrix_array[3][3] * vec.w );
                return _v;              
            } 
            
            static Matrix4x4d get_identity()
            {
                return Matrix4x4d(  1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1 );
        
            }

    
            void print_matrix()
            {
                for(int i=0; i<4; i++)
                {
                        for(int j=0;j<4; j++){
                            std::cout<<get_value_at(i,j)<<" ";
                            }
                    std::cout<<std::endl;
                } 
            }

    };    
}

#endif  
