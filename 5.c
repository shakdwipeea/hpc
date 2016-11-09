#include<stdio.h>
#include<error.h>
#include<gd.h>
#include<string.h>
#include<omp.h>

int main(int argc,char **argv)
{
	int nt;
	printf("Enter the number of threads :");
	scanf("%d",&nt);
	printf("\n");
	FILE *fp={0};
	gdImagePtr img;
	char *iname=NULL;
	char *oname=NULL;
	int  color,x,y,w,h,i=0;
	int red,green,blue;
	color=x=y=w=h=0;
	red=green=blue=0;
	if(argc!=3)
		error(1,0,"Usage:gdnegat input.png output.png");
	else
	{
		iname=argv[1];
		oname=argv[2];
	}
	if((fp=fopen(iname,"r"))==NULL)
		error(1,0,"Error-fopen():%s",iname);
	else
		img=gdImageCreateFromPng(fp);
		w=gdImageSX(img);
		h=gdImageSY(img);

	double t1, t = omp_get_wtime();

	#pragma omp parallel for private(y,color,green,red,blue) num_threads(nt)
	for(x=0;x<w;x++)
	{
		for(y=0;y<h;y++)
		{
		    color=x+0;
		    color=gdImageGetPixel(img,x,y);
		    red=255-gdImageRed(img,color);
		    green=255-gdImageGreen(img,color);
		    blue=255-gdImageBlue(img,color);
		    color=gdImageColorAllocate(img,red,green,blue);
		    gdImageSetPixel(img,x,y,color);
		}
	}

	t=omp_get_wtime()-t;
	if((fp=fopen(oname,"w"))==NULL)
		error(1,0,"Error-fopen():%s",oname);
	else
	{
		gdImagePng(img,fp);
		fclose(fp);
	}
	
	t1 = omp_get_wtime()-t;
	gdImageDestroy(img);
	printf("Time Taken=%lf",t);
	return 0;
}
