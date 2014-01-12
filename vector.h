class Vector3d
{
public:
    float x;
    float y;
    float z;

    Vector3d()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    Vector3d(float vx,float vy,float vz)
    {
        x = vx;
        y = vy;
        z = vz;
    }

    Vector3d(const Vector3d& V)
    {
        x = V.x;
        y = V.y;
        z = V.z;
    }

    void normal()
    {
        float mod = sqrt(x*x + y*y + z*z);
        x /= mod;
        y /= mod;
        z /= mod;
    }

    Vector3d operator-(const Vector3d& V)
    {
        Vector3d result = *this;
        result.x = result.x - V.x;
        result.y = result.y - V.y;
        result.z = result.z - V.z;
        return result;
    }

    Vector3d operator+(const Vector3d& V)
    {
        Vector3d result = *this;
        result.x = result.x + V.x;
        result.y = result.y + V.y;
        result.z = result.z + V.z;
        return result;
    }

    Vector3d operator*(const Vector3d& V)
    {
        Vector3d result;
        result.x = y*V.z - z*V.y;
        result.y = z*V.x - x*V.z;
        result.z = x*V.y - y*V.x;
        return result;
    }

    Vector3d& operator=(const Vector3d& V)
    {
        x = V.x;
        y = V.y;
        z = V.z;
    }
};
