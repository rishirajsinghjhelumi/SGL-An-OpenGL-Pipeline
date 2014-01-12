void sglBegin(int type);
void sglVertex(float ,float ,float);
void sglEnd();
void sglColor(float ,float,float);


void ply(int mode)
{
    points[point_count].type = mode;
    if(mode == CLEAR_COLOR)
        points[point_count].C = clearColor;
    else if(mode == CURRENT_COLOR)
        points[point_count].C = currentColor;
    else if(mode == VERTEX)
    {
        points[point_count].V = currentCoordinates;
        points[point_count].C = currentColor;
    }
    point_count++;
}

void print_ply()
{
    int i;
    for(i=0;i<point_count;i++)
    {
        switch(points[i].type)
        {
            case SGL_TRIANGLES:
                glBegin(GL_TRIANGLES);
                break;
            case END_SGL_TRIANGLES:
                glEnd();
                break;
            case VERTEX:
                glVertex2f(points[i].V.x,points[i].V.y);
                //glVertex2f(points[i].V.x,points[i].V.y);
                break;
            case CURRENT_COLOR:
                glColor3f(points[i].C.x,points[i].C.y,points[i].C.z);
                break;
            case CLEAR_COLOR:
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(points[i].C.x,points[i].C.y,points[i].C.z,1.0);
                break;
            default:
                break;
        }
    }
    point_count = 0;
}

void save_ply()
{
    FILE *f = fopen("object.ply","w");
    fprintf(f,"ply\n");
    int vertex_count = 0,i;
    for(i=0;i<point_count;i++)
        if(points[i].type == VERTEX)
            vertex_count++;
    fprintf(f,"element vertex %d\n",vertex_count);

    for(i=0;i<point_count;i++)
        if(points[i].type == VERTEX)
            fprintf(f,"%f %f %f\n",points[i].V.x,points[i].V.y,points[i].V.z,points[i].C.x,points[i].C.y,points[i].C.z);

    int triangles[point_count][3];
    int count = 0,check = 0,tcount = 0,triangle_count = 0;
    for(i=0;i<point_count;i++)
    {
        if(points[i].type == VERTEX)
        {
            if(check)
            {
                triangles[triangle_count][tcount++] = count;
                if(tcount == 3)
                {
                    triangle_count++;
                    tcount = 0;
                }
            }
            count++;
        }
        else if(points[i].type == SGL_TRIANGLES)
            check = 1;
        else if(points[i].type == END_SGL_TRIANGLES)
            check = 0;
    }
    fprintf(f,"element face %d\n",triangle_count);
    for(i=0;i<triangle_count;i++)
        fprintf(f,"3 %d %d %d\n",triangles[i][0],triangles[i][1],triangles[i][2]);
}

double rand_num()
{
    return (double)rand() / (double)RAND_MAX ;
}

Model load_ply(char *filename,Model model)
{
    FILE *f = fopen(filename,"r");
    int i,j;
    fscanf(f,"%*s%*s%*s");

    fscanf(f,"%d",&model.num_vertices);
    for(i=0;i<model.num_vertices;i++)
        fscanf(f,"%f%f%f",&model.vertice[i].x,&model.vertice[i].y,&model.vertice[i].z);

    fscanf(f,"%*s%*s");

    fscanf(f,"%d",&model.num_triangles);

    int num_color = model.num_triangles / 3;
    for(i=0;i<model.num_triangles;i++)
    {
        if(i < num_color)
            model.color[i].x = rand_num(),model.color[i].y = 0.0,model.color[i].z = 0.0;
        else if(i >= num_color && i < 2*num_color)
            model.color[i].x = 0.0,model.color[i].y = rand_num(),model.color[i].z = 0.0;
        else
            model.color[i].x = 0.0,model.color[i].y = 0.0,model.color[i].z = rand_num();
        fscanf(f,"%*d%d%d%d",&model.triangle[i][0],&model.triangle[i][1],&model.triangle[i][2]);
    }

    return model;
}

void display_ply_model(Model model)
{
    int i,j;
    //sglColor(0,0,0);
    sglBegin(SGL_TRIANGLES);
    for(i=0;i<model.num_triangles;i++)
    {
        sglColor(model.color[i].x,model.color[i].y,model.color[i].z);
        for(j=0;j<3;j++)
            sglVertex(model.vertice[model.triangle[i][j]].x,model.vertice[model.triangle[i][j]].y,model.vertice[model.triangle[i][j]].z);
    }
    sglEnd();
}








