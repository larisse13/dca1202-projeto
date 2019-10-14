#include "sculptor.h"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    Sculptor forma(20,20,20);

    forma.setColor(0,0,1,1);
    forma.putVoxel(5,5,5);
    forma.putBox(0,4,0,4,0,4);
    forma.cutBox(1,2,1,2,1,2);
    forma.cutVoxel(0,0,0);
    forma.putSphere(10,10,10,3);
    forma.cutSphere(11,11,11,2);
    forma.putEllipsoid(15,15,15,2,3,1);
    forma.cutEllipsoid(16,16,16,1,1,1);
    forma.writeOFF("write.off");
    forma.writeVECT("write.vect");
    return 0;
}
