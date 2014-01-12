class Matrix
{
    int num_rows;
    int num_cols;
    int i,j,k;

public:
    float matrix[4][4];
    float linear_matrix[16];

    Matrix()
    {
        num_rows = 4;
        num_cols = 4;
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                matrix[i][j] = 0.0;
    }

    Matrix(int rows,int cols)
    {
        num_rows = rows;
        num_cols = cols;
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                matrix[i][j] = 0.0;
    }

    Matrix(const Matrix& mat)
    {
        num_rows = mat.num_rows;
        num_cols = mat.num_cols;
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                matrix[i][j] = mat.matrix[i][j];
    }

    Matrix(float *mat)
    {
        num_rows = 4;
        num_cols = 4;
        for(i=0; i<4; i++)
            for(j=0; j<4; j++)
                matrix[i][j] = mat[4*i + j];
    }

    void identity()
    {
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                matrix[i][j] = (i==j)?1.0:0.0;
    }

    Matrix operator+(const Matrix& mat)
    {
        Matrix result = *this;
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                result.matrix[i][j] += mat.matrix[i][j];
        return result;
    }

    Matrix operator-(const Matrix& mat)
    {
        Matrix result = *this;
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                result.matrix[i][j] -= mat.matrix[i][j];
        return result;
    }

    Matrix& operator=(const Matrix& mat)
    {
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                matrix[i][j] = mat.matrix[i][j];
    }

    Matrix operator*(const Matrix& mat)
    {
        Matrix result;
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                for(k=0; k<num_rows; k++)
                    result.matrix[i][j] = result.matrix[i][j] + (matrix[i][k] * mat.matrix[k][j]);
        return result;
    }

    Matrix operator*(float scale_factor)
    {
        Matrix result = *this;
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                result.matrix[i][j] *= scale_factor;
        return result;
    }

    Matrix operator*(float mat[4])
    {
        Matrix result;
        for(i=0;i<4;i++)
            result.matrix[0][i] = matrix[i][0]*mat[0] + matrix[i][1]*mat[1] + matrix[i][2]*mat[2] + matrix[i][3]*mat[3];
        for(i=0;i<3;i++)
            result.matrix[0][i] /= result.matrix[0][3];
        return result;
    }

    void transpose()
    {
        Matrix T;
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                T.matrix[j][i] = matrix[i][j];
        for(i=0; i<num_rows; i++)
            for(j=0; j<num_cols; j++)
                matrix[i][j] = T.matrix[i][j];
    }

    friend std::ostream& operator<<(std::ostream& output,const Matrix& mat)
    {
        int i,j;
        for(i=0; i<mat.num_rows; i++)
        {
            for(j=0; j<mat.num_cols; j++)
                output<<mat.matrix[i][j]<<" ";
            output<<"\n";
        }
        return output;
    }

    void linear()
    {
        for(i=0; i<4; i++)
            for(j=0; j<4; j++)
                linear_matrix[i + 4*j] = matrix[i][j];
    }

};
