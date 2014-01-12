void normalize(float *x,float *y,float *z)
{
    float mod=sqrt((*x)*(*x) + (*y)*(*y) + (*z)*(*z));
    *x /= mod;
    *y /= mod;
    *z /= mod;
}

void sglModTranslate(float transX,float transY,float transZ)
{
    Matrix T;
    T.identity();
    T.matrix[0][3] = transX;
    T.matrix[1][3] = transY;
    T.matrix[2][3] = transZ;

    modelMatrix = modelMatrix * T;
}

void sglModScale(float scaleX,float scaleY,float scaleZ)
{
    Matrix S;
    S.identity();
    S.matrix[0][0] = scaleX;
    S.matrix[1][1] = scaleY;
    S.matrix[2][2] = scaleZ;

    modelMatrix = modelMatrix * S;
}

void sglModRotate(float angle,float x,float y,float z)
{
    float c = cos(DEG2RAD(angle));
    float s = sin(DEG2RAD(angle));
    normalize(&x,&y,&z);
    Matrix R;
    R.identity();
    R.matrix[0][0] = x*x*(1-c) + c;
    R.matrix[0][1] = x*y*(1-c) - z*s;
    R.matrix[0][2] = x*z*(1-c) + y*s;
    R.matrix[1][0] = x*y*(1-c) + z*s;
    R.matrix[1][1] = y*y*(1-c) + c;
    R.matrix[1][2] = y*z*(1-c) - x*s;
    R.matrix[2][0] = x*z*(1-c) - y*s;
    R.matrix[2][1] = y*z*(1-c) + x*s;
    R.matrix[2][2] = z*z*(1-c) + c;

    modelMatrix = modelMatrix * R;
}

void sglModMatrix(float mat[16])
{
    Matrix M;
    int i,j;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            M.matrix[i][j] = mat[4*i + j];

    modelMatrix = M;
}

void sglModLoadIdentity()
{
    Matrix I;
    I.identity();

    modelMatrix = I;
}

void sglModPushMatrix()
{
    modelStack.push(modelMatrix);
}

void sglModPopMatrix()
{
    modelMatrix = modelStack.top();
    modelStack.pop();
}

void sglViewRotate(float angle,float x,float y,float z)
{
    float c = cos(DEG2RAD(angle));
    float s = sin(DEG2RAD(angle));
    normalize(&x,&y,&z);
    Matrix VR;
    VR.identity();
    VR.matrix[0][0] = x*x*(1-c) + c;
    VR.matrix[0][1] = x*y*(1-c) - z*s;
    VR.matrix[0][2] = x*z*(1-c) + y*s;
    VR.matrix[1][0] = x*y*(1-c) + z*s;
    VR.matrix[1][1] = y*y*(1-c) + c;
    VR.matrix[1][2] = y*z*(1-c) - x*s;
    VR.matrix[2][0] = x*z*(1-c) - y*s;
    VR.matrix[2][1] = y*z*(1-c) + x*s;
    VR.matrix[2][2] = z*z*(1-c) + c;


    viewMatrix = viewMatrix * VR;


}

void sglViewTranslate(float transX,float transY,float transZ)
{
    Matrix VT;
    VT.identity();
    VT.matrix[0][3] = transX;
    VT.matrix[1][3] = transY;
    VT.matrix[2][3] = transZ;

    viewMatrix = viewMatrix * VT;
}

void sglLookAt(float camX,float camY,float camZ,float lookX,float lookY,float lookZ,float upX,float upY,float upZ)
{
    Vector3d cam(camX,camY,camZ);
    Vector3d look(lookX,lookY,lookZ);
    Vector3d up(upX,upY,upZ);

    Vector3d zaxis = look - cam;
    zaxis.normal();
    Vector3d xaxis = up * zaxis;
    xaxis.normal();
    Vector3d yaxis = zaxis * xaxis;

    Matrix lookAt;
    lookAt.identity();
    lookAt.matrix[0][0] = xaxis.x;
    lookAt.matrix[0][1] = yaxis.x;
    lookAt.matrix[0][2] = zaxis.x;
    lookAt.matrix[1][0] = xaxis.y;
    lookAt.matrix[1][1] = yaxis.y;
    lookAt.matrix[1][2] = zaxis.y;
    lookAt.matrix[2][0] = xaxis.z;
    lookAt.matrix[2][1] = yaxis.z;
    lookAt.matrix[2][2] = zaxis.z;

    Matrix T;
    T.identity();
    T.matrix[3][0] = -camX;
    T.matrix[3][1] = -camY;
    T.matrix[3][2] = -camZ;

    viewMatrix = viewMatrix * (T);
    viewMatrix = viewMatrix * (lookAt);

}

void sglViewMatrix(float mat[16])
{
    Matrix VM;
    int i,j;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
            VM.matrix[i][j] = mat[4*i + j];

    viewMatrix = VM;
}

void sglViewLoadIdentity()
{
    Matrix VI;
    VI.identity();

    viewMatrix = VI;
}

void sglProjOrtho(float left,float right,float bottom,float top,float near,float far)
{
    Matrix orthoProj;
    orthoProj.identity();
    orthoProj.matrix[0][0] = 2.0/(right - left);
    orthoProj.matrix[1][1] = 2.0/(top - bottom);
    orthoProj.matrix[2][2] = -2.0/(far - near);
    orthoProj.matrix[0][3] = -(right + left)/(right - left);
    orthoProj.matrix[1][3] = -(top + bottom)/(top - bottom);
    orthoProj.matrix[2][3] = -(far + near)/(far - near);

    projectionMatrix = orthoProj;
}

void sglProjFrustum(float left,float right,float bottom,float top,float near,float far)
{
    Matrix frustumProj;
    frustumProj.identity();
    frustumProj.matrix[0][0] = (2.0*near)/(right - left);
    frustumProj.matrix[0][2] = (right + left)/(right - left);
    frustumProj.matrix[1][1] = (2.0*near)/(top - bottom);
    frustumProj.matrix[1][2] = (top + bottom)/(top - bottom);
    frustumProj.matrix[2][2] = -(far + near)/(far - near);
    frustumProj.matrix[2][3] = -(2*far*near)/(far - near);
    frustumProj.matrix[3][2] = -1.0;
    frustumProj.matrix[3][3] = 0.0;

    projectionMatrix = frustumProj;
}

void sglProjPerspective(float fovY,float aspect,float near,float far)
{
    float rad = tan(DEG2RAD(fovY/2.0));
    float height = near * rad;
    float width = height * aspect;

    sglProjFrustum(-width,width,-height,height,near,far);
}

void sglViewport(float startX,float startY,float width,float height)
{
    windowWidth = width;
    windowHeight = height;

    Matrix viewPort;
    viewPort.identity();
    viewPort.matrix[0][0] = width/2.0;
    viewPort.matrix[1][1] = -height/2.0;
    viewPort.matrix[3][0] = startX ;
    viewPort.matrix[3][1] = startY ;
    viewPort.matrix[2][2] = 0.5;
    viewPort.matrix[3][2] = 0.5;

    viewportMatrix = viewPort;
}

void sglColor(float r,float g,float b)
{
    currentColor.x = r;
    currentColor.y = g;
    currentColor.z = b;

    ply(CURRENT_COLOR);
}

void sglClear(float r,float g,float b)
{
    clearColor.x = r;
    clearColor.y = g;
    clearColor.z = b;

    ply(CLEAR_COLOR);
}

void sglVertex(float x,float y,float z)
{

    float mat[4] = {x,y,z,1.0};

    viewportMatrix.transpose();
    viewMatrix.transpose();
    //modelMatrix.transpose();

    coordinateMatrix = viewportMatrix * projectionMatrix * viewMatrix * modelMatrix;
    //coordinateMatrix.transpose();
    /*cout<<modelMatrix;
    cout<<viewMatrix;
    cout<<projectionMatrix;
    cout<<viewportMatrix;
    cout<<coordinateMatrix;*/

    viewportMatrix.transpose();
    viewMatrix.transpose();
    //modelMatrix.transpose();

    //cout<<coordinateMatrix;
    coordinateMatrix = coordinateMatrix * mat;
    //cout<<coordinateMatrix;


    //coordinateMatrix = coordinateMatrix;


    currentCoordinates.x = coordinateMatrix.matrix[0][0];
    currentCoordinates.y = coordinateMatrix.matrix[0][1];
    currentCoordinates.z = coordinateMatrix.matrix[0][2];

    /*currentCoordinates.x = (currentCoordinates.x) * (windowWidth / 2.0);
    currentCoordinates.y = (currentCoordinates.y) * (windowHeight / 2.0);
    currentCoordinates.z = (currentCoordinates.z + 1.0) / 2.0;*/

    ply(VERTEX);

}

void sglBegin(int type)
{
    if(type != SGL_TRIANGLES)
        return;

    ply(SGL_TRIANGLES);
}

void sglEnd()
{
    ply(END_SGL_TRIANGLES);
}

void sglShow()
{
    print_ply();
}










