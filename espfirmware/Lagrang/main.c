#include <stdio.h>
double eitkin(double* X, double* Y,double Xin,double n)
{
    int i = 0;
    int a = 0;
    int b = 0;
    int yres = 0;
    for(i=0;i<n;i++)
    {
        if(Xin < X[i])
        {
            a=(Y[i] - Y[i-1])/(X[i] - X[i-1]);
            b=Y[i-1] - a*X[i-1] ;
            yres = a*Xin + b;
            return yres;
        }
    }
    return 0;
}
double lagranz(double* X, double* Y, double t,double n){
        double z,p1,p2; 
        z=0;
        for (int j=0; j<n; j++)
        {
            p1=1; p2=1;
            for (int i=0; i<n; i++){
                if (i!=j)
                {
                                p1=p1*(t-X[i]);
                                p2=p2*(X[j]-X[i]);
                }
            }
            z=z+Y[j]*p1/p2;
        }
        return z;
}
int main(int argc, char **argv)
{
    double X[] = {5,10,15,20,25,30,35,40};
    double Y[] = {1.710,2.154,2.466,2.714,2.924,3.107,3.271,3.420};
    //lagranz(X,Y,1,8);
	printf("%f\n",lagranz(X,Y,6,8));
    printf("%f\n",eitkin(X,Y,6,8));
	return 0;
}
