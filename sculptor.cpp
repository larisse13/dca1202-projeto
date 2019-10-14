#include <iostream>
#include <fstream>
#include "sculptor.h"

using namespace std;

//Construtor
Sculptor::Sculptor(int _nx, int _ny, int _nz)
{
    //Número de linhas, colunas e planos
    nx=_nx;
    ny=_ny;
    nz=_nz;

    //Alocação dinâmica
    v = new Voxel**[nx];
    v[0] = new Voxel*[nx*ny];
    v[0][0]= new Voxel[nx*ny*nz];
}


//Destrutor
Sculptor::~Sculptor(){
    if(nx==0||ny==0||nz==0){
        return;
    }
    delete [] v[0][0];
    delete [] v[0];
    delete [] v;
}


//Coloca as cores no voxel
void Sculptor::setColor(float r, float g, float b, float alpha)
{
    rc=r;
    gc=g;
    bc=b;
    ac=alpha;
}

//Ativa o voxel e atribui sua cor
void Sculptor::putVoxel(int x, int y, int z)
{
    v[x][y][z].isOn = true;
    v[x][y][z].r = rc;
    v[x][y][z].g = gc;
    v[x][y][z].b = bc;
    v[x][y][z].a = ac;
}

//Desativa o voxel
void Sculptor::cutVoxel(int x, int y, int z)
{
    v[x][y][z].isOn =false;
}

//Ativa o voxel de acordo com os parametros passados para o elipsoide e atribui sua cor
void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){
    for(int i=0; i< rx; i++)
    {
        for(int j=0; j< ry; j++)
        {
            for(int k=0; k< rz; k++)
            {
                //Equação do elipsoide
                if ((i-xcenter)*(i-xcenter)/(rx*rx) + ((j-ycenter)*(j-ycenter))/(ry*ry) + ((k-zcenter)*(k-zcenter))/(rz*rz) <=1.0)
                {
                    v[i][j][k].isOn = true;
                    v[i][j][k].r = rc;
                    v[i][j][k].g = gc;
                    v[i][j][k].b = bc;
                    v[i][j][k].a = ac;
                }
            }
        }
    }
}

//Desativa o voxel
void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){
    for(int i=0; i< rx; i++)
    {
        for(int j=0; j< ry; j++)
        {
            for(int k=0; k< rz; k++)
            {
                if ((i-xcenter)*(i-xcenter)/(rx*rx) + ((j-ycenter)*(j-ycenter))/(ry*ry) + ((k-zcenter)*(k-zcenter))/(rz*rz) <=1.0)
                {
                    v[i][j][k].isOn = false;
                }
            }
        }
    }
}

//Ativa o voxel de acordo com os parametros passados para a esfera e atribui sua cor
void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius){
    for(int i=0; i< xcenter; i++)
    {
        for(int j=0; j< ycenter; j++)
        {
            for(int k=0; k< zcenter; k++)
            {
                //Equação da esfera
                if (((i-xcenter)*(i-xcenter) + (j-ycenter)*(j-ycenter) + (k-zcenter)*(k-zcenter)) <= (radius*radius))
                {
                    v[i][j][k].isOn = true;
                    v[i][j][k].r = rc;
                    v[i][j][k].g = gc;
                    v[i][j][k].b = bc;
                    v[i][j][k].a = ac;
                }
            }
        }
    }
}

//Desativa o voxel
void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius){
    for(int i=0; i< xcenter; i++)
    {
        for(int j=0; j< ycenter; j++)
        {
            for(int k=0; k< zcenter; k++)
            {
                if (((i-xcenter)*(i-xcenter) + (j-ycenter)*(j-ycenter) + (k-zcenter)*(k-zcenter)) <= (radius*radius))
                {
                    v[i][j][k].isOn = false;
                    v[i][j][k].r = rc;
                    v[i][j][k].g = gc;
                    v[i][j][k].b = bc;
                    v[i][j][k].a = ac;
                }
            }
        }
    }
}

//Ativa o voxel de acordo com os parametros passados para o cubo e atribui sua cor
void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1){
    for (int i = x0; i<=x1; i++){
        for (int j = y0; j<=y1; j++){
            for (int k = z0; k<=z1; k++){
                v[i][j][k].isOn = true;
            }
        }
    }
}

//Desativa o voxel
void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1){
    for (int i = x0; i<=x1; i++){
        for (int j = y0; j<=y1; j++){
            for (int k = z0; k<=z1; k++){
                v[i][j][k].isOn = false;
            }
        }
    }
}

void Sculptor::writeOFF(string filename){

    ofstream fout(filename);
    //Primeira linha do arquivo
    fout<<"OFF"<<endl;

    int voxel=0;

    //O loop está contando o número de voxels ativos
    for(int i=0;i<nx;i++){
        for(int j=0;j<ny;j++){
            for (int k=0;k<nz;k++) {
                if(v[i][j][k].isOn)
                {
                    voxel++;
                }
            }
        }
    }

    /*Segunda linha do arquivo. Mútiplica a quantidade total de voxels
     * pelo número de vértices e faces de um único cubo para obter a
     * quantidade total de vértices e faces presentes no desenho.
    */
    fout<<voxel*8<<" "<<voxel*6<<" "<<endl;

    /*Para todo voxel ativo, o laço calcula a posição dos seus 8
     * vértices. Olhar resumo (Figura 3).
    */
    for (int i=0;i<nx;i++) {
        for (int j=0;j<ny;j++) {
            for (int k=0;k<nz;k++) {
                if(v[i][j][k].isOn){
                    fout<<-0.5+i<<" "<<0.5+j<<" "<<-0.5+k<<endl;
                    fout<<-0.5+i<<" "<<-0.5+j<<" "<<-0.5+k<<endl;
                    fout<<0.5+i<<" "<<-0.5+j<<" "<<-0.5+k<<endl;
                    fout<<0.5+i<<" "<<0.5+j<<" "<<-0.5+k<<endl;
                    fout<<-0.5+i<<" "<<0.5+j<<" "<<0.5+k<<endl;
                    fout<<-0.5+i<<" "<<-0.5+j<<" "<<0.5+k<<endl;
                    fout<<0.5+i<<" "<<-0.5+j<<" "<<0.5+k<<endl;
                    fout<<0.5+i<<" "<<0.5+j<<" "<<0.5+k<<endl;
                }
            }
        }
    }

    /*O primeiro número de cada linha simboliza quantos vértices são necessários
     *para formar uma face, os quatro números em seguida representam os vértices
     * que formam uma determinada face. Os quatro números finais de cada linha
     * simbolizam a cor e a tranaparência de cada voxel. Ao incrementar o valor
     * vértice por 8 toda vez que um voxel ativo for encontrado, impede-se que
     * existam linhas com os mesmos valores. Cada voxel terá seus próprios valores
     * de vértice.
    */

    int vertice=0;
    for (int i=0;i<nx;i++) {
        for (int j=0;j<ny;j++) {
            for (int k=0;k<nz;k++) {
                if(v[i][j][k].isOn){
                    fout<<"4 "<<vertice+0<<" "<<vertice+3<<" "<<vertice+2<<" "<<vertice+1<<" "<<v[i][j][k].r<<" "<<v[i][j][k].g<<" "<<v[i][j][k].b<<" "<<v[i][j][k].a<<endl;
                    fout<<"4 "<<vertice+4<<" "<<vertice+5<<" "<<vertice+6<<" "<<vertice+7<<" "<<v[i][j][k].r<<" "<<v[i][j][k].g<<" "<<v[i][j][k].b<<" "<<v[i][j][k].a<<endl;
                    fout<<"4 "<<vertice+0<<" "<<vertice+1<<" "<<vertice+5<<" "<<vertice+4<<" "<<v[i][j][k].r<<" "<<v[i][j][k].g<<" "<<v[i][j][k].b<<" "<<v[i][j][k].a<<endl;
                    fout<<"4 "<<vertice+0<<" "<<vertice+4<<" "<<vertice+7<<" "<<vertice+3<<" "<<v[i][j][k].r<<" "<<v[i][j][k].g<<" "<<v[i][j][k].b<<" "<<v[i][j][k].a<<endl;
                    fout<<"4 "<<vertice+3<<" "<<vertice+7<<" "<<vertice+6<<" "<<vertice+2<<" "<<v[i][j][k].r<<" "<<v[i][j][k].g<<" "<<v[i][j][k].b<<" "<<v[i][j][k].a<<endl;
                    fout<<"4 "<<vertice+1<<" "<<vertice+2<<" "<<vertice+6<<" "<<vertice+5<<" "<<v[i][j][k].r<<" "<<v[i][j][k].g<<" "<<v[i][j][k].b<<" "<<v[i][j][k].a<<endl;

                    vertice+=8;
                }
            }
        }
    }


}

void Sculptor::writeVECT(string filename){
    ofstream fout(filename);
    //Primeira linha do arquivo
    fout<<"VECT"<<endl;

    int voxel=0;
    //O loop está contando o número de voxels ativos
    for(int i=0;i<nx;i++){
        for(int j=0;j<ny;j++){
            for (int k=0;k<nz;k++) {
                if(v[i][j][k].isOn)
                {
                    voxel++;
                }
            }
        }
    }

    //Segunda linha do arquivo. Está imprimindo a quantidade de voxels três vezes
    fout<<voxel<<" "<<voxel<<" "<<voxel<<endl;

    /*Na terceira e quarta linha do arquivo são impressas uma sequência de números
     * 1. A quantidade de números será igual a quantidade de voxels.
    */
    for(int i=0;i<voxel;i++){
        fout<<"1 ";
    }
    fout<<endl;
    for(int i=0;i<voxel;i++){
        fout<<"1 ";
    }
    fout<<endl;

    //Para cada voxel ativo, imprime os valores da sua cor e da sua transparência.
    for(int i = 0; i<nx; i++){
        for(int j = 0; j<ny; j++){
            for(int k = 0; k<nz; k++){
                if(v[i][j][k].isOn){
                    fout<<v[i][j][k].r<<" "<<v[i][j][k].g<<" "<<v[i][j][k].b<<" "<<v[i][j][k].a<<endl;
                }
            }
        }
    }
}


