
//============================================================================
// Name        : bigvalence.cpp
// Author      : Kenneth Edward Hinson
// Version     :
// Copyright   : Copyrights by Owner
// Description : C++, Ansi-style
//============================================================================

#include<iostream>
#include<fstream>
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

const long ntrials=1000;
const float pi=3.14159265358979324;
const int digits=20;
const int Height=7;
const int Width=7;

//structure definitions
struct point {
	float x;
	float y;
	float z;
};

struct vertex {
	struct point pt;
	struct vertex * prev;
	struct vertex * next;
};

struct segment {
	struct point start;
	struct point end;
};

struct chain {
	int linknumber;
	int ncircles;     // number of minicircles in the chain
	chain * linkedto;
};

struct circle {
	point center;
	point normalvector;
	chain * link;
	float dists[44];  //added for valence computation -- to store distances to 44 nearest neighbors
};

int operator==(point p1, point p2)   // to use == for equality of vectors
{
	if(p1.x==p2.x && p1.y==p2.y && p1.z==p2.z)
		return 1;
	else
		return 0;
}

float randdouble()  //random number generator - random float between 0 and 1
{
	int digit;
	int k;
	double numb=0;

	for(k=0;k<digits;k++)
	{
		digit=rand()%10;
		numb=numb*0.1+digit;
	}

	return numb*0.1;
}

float distanc(point pt1, point pt2)
{
	return sqrt(pow(pt1.x-pt2.x,2)+pow(pt1.y-pt2.y,2)+pow(pt1.z-pt2.z,2));
}

struct point randomunitvector(float tilting, float azimuthal)
{
	point vector;
	float theta;

	vector.z=(2*randdouble()-1)*cos(tilting*pi/180);

	theta=(2*randdouble()-1)*(0.5-azimuthal/180)*pi;

	vector.x=sqrt(1-pow(vector.z,2))*cos(theta);
	vector.y=sqrt(1-pow(vector.z,2))*sin(theta);

	return vector;
}

int linked(circle * circle1, circle * circle2, float radius, int nbr)
{
		float plane1a, plane1b, plane1c, plane1d;
		float plane2a, plane2b, plane2c, plane2d;
		float plane3a, plane3b, plane3c, plane3d;
		point intersect;

		if((circle1->center).x==(circle2->center).x)
		{
			plane2a=(circle1->center).x - (circle2->center).x;
			plane2b=(circle1->center).y - (circle2->center).y;
			plane2c=(circle1->center).z - (circle2->center).z;  //should be 0
			plane2d=plane2a*0.5*((circle1->center).x+(circle2->center).x)+plane2b*0.5*((circle1->center).y+(circle2->center).y)+plane2c*0.5*((circle1->center).z+(circle2->center).z);
			if((circle1->normalvector).x == 0)
			{
				plane3a=(circle1->normalvector).x;
				plane3b=(circle1->normalvector).y;
				plane3c=(circle1->normalvector).z;
				plane3d=(circle1->normalvector).x*(circle1->center).x+(circle1->normalvector).y*(circle1->center).y+(circle1->normalvector).z*(circle1->center).z;
				plane1a=(circle2->normalvector).x;
				plane1b=(circle2->normalvector).y;
				plane1c=(circle2->normalvector).z;
				plane1d=(circle2->normalvector).x*(circle2->center).x+(circle2->normalvector).y*(circle2->center).y+(circle2->normalvector).z*(circle2->center).z;
			}
			else
			{
				plane1a=(circle1->normalvector).x;
				plane1b=(circle1->normalvector).y;
				plane1c=(circle1->normalvector).z;
				plane1d=(circle1->normalvector).x*(circle1->center).x+(circle1->normalvector).y*(circle1->center).y+(circle1->normalvector).z*(circle1->center).z;
				plane3a=(circle2->normalvector).x;
				plane3b=(circle2->normalvector).y;
				plane3c=(circle2->normalvector).z;
				plane3d=(circle2->normalvector).x*(circle2->center).x+(circle2->normalvector).y*(circle2->center).y+(circle2->normalvector).z*(circle2->center).z;
			}
		}
		else
		{
			plane1a=(circle1->center).x - (circle2->center).x;
			plane1b=(circle1->center).y - (circle2->center).y;
			plane1c=(circle1->center).z - (circle2->center).z;  //should be 0
			plane1d=plane1a*0.5*((circle1->center).x+(circle2->center).x)+plane1b*0.5*((circle1->center).y+(circle2->center).y)+plane1c*0.5*((circle1->center).z+(circle2->center).z);
			if(plane1a*(circle1->normalvector).y == plane1b*(circle1->normalvector).x)
			{
				plane3a=(circle1->normalvector).x;
				plane3b=(circle1->normalvector).y;
				plane3c=(circle1->normalvector).z;
				plane3d=(circle1->normalvector).x*(circle1->center).x+(circle1->normalvector).y*(circle1->center).y+(circle1->normalvector).z*(circle1->center).z;
				plane2a=(circle2->normalvector).x;
				plane2b=(circle2->normalvector).y;
				plane2c=(circle2->normalvector).z;
				plane2d=(circle2->normalvector).x*(circle2->center).x+(circle2->normalvector).y*(circle2->center).y+(circle2->normalvector).z*(circle2->center).z;
			}
			else
			{
				plane2a=(circle1->normalvector).x;
				plane2b=(circle1->normalvector).y;
				plane2c=(circle1->normalvector).z;
				plane2d=(circle1->normalvector).x*(circle1->center).x+(circle1->normalvector).y*(circle1->center).y+(circle1->normalvector).z*(circle1->center).z;
				plane3a=(circle2->normalvector).x;
				plane3b=(circle2->normalvector).y;
				plane3c=(circle2->normalvector).z;
				plane3d=(circle2->normalvector).x*(circle2->center).x+(circle2->normalvector).y*(circle2->center).y+(circle2->normalvector).z*(circle2->center).z;
			}
		}

		intersect.z=((plane1a*plane2b-plane1b*plane2a)*(plane1a*plane3d-plane1d*plane3a)-(plane1a*plane3b-plane1b*plane3a)*(plane1a*plane2d-plane1d*plane2a))/((plane1a*plane2b-plane1b*plane2a)*(plane1a*plane3c-plane1c*plane3a)-(plane1a*plane3b-plane1b*plane3a)*(plane1a*plane2c-plane1c*plane2a));
		intersect.y=(plane1a*plane2d-plane2a*plane1d-(plane1a*plane2c-plane1c*plane2a)*intersect.z)/(plane1a*plane2b-plane1b*plane2a);
		intersect.x=(plane1d-plane1c*intersect.z-plane1b*intersect.y)/plane1a;

		circle1->dists[nbr]=distanc(intersect,circle1->center);

		if(distanc(intersect,circle1->center)<radius && distanc(intersect,circle2->center)<radius)
			return 1;
		else
		{
			return 0;
		}
}

int minlinknum(circle * circle1)
{
	chain * currentchain;
	currentchain=circle1->link;
	while(currentchain->linknumber != (currentchain->linkedto)->linknumber)
	{
		currentchain=currentchain->linkedto;
	}
	return currentchain->linknumber;
}

struct chain * minlinkedchain(chain * currentchain)
{
	chain * findmin;
	findmin=currentchain;

	while(findmin->linknumber != (findmin->linkedto)->linknumber)
	{
		findmin=findmin->linkedto;
	}
	return findmin;
}

int comparelinks(circle * circlesarray[Width][Height], int currentrow, int currentcol, int comparerow, int comparecol, float circleradius, int linkcount, int nbrindex)
{
	if(linked(circlesarray[currentcol][currentrow],circlesarray[comparecol][comparerow],circleradius,nbrindex)==1)
	{
		if(minlinknum(circlesarray[currentcol][currentrow]) != minlinknum(circlesarray[comparecol][comparerow]))
		{
			if(((circlesarray[currentcol][currentrow])->link)->linknumber == 0)   //i.e. unlinked
			{
				circlesarray[currentcol][currentrow]->link = circlesarray[comparecol][comparerow]->link;
				(minlinkedchain(circlesarray[comparecol][comparerow]->link))->ncircles++;
			}
			else if(((circlesarray[comparecol][comparerow])->link)->linknumber == 0)
			{
				circlesarray[comparecol][comparerow]->link = circlesarray[currentcol][currentrow]->link;
				(minlinkedchain(circlesarray[comparecol][comparerow]->link))->ncircles++;
			}
			else
			{
				if(minlinknum(circlesarray[comparecol][comparerow]) < minlinknum(circlesarray[currentcol][currentrow]))
				{
					(minlinkedchain(circlesarray[comparecol][comparerow]->link))->ncircles = (minlinkedchain(circlesarray[comparecol][comparerow]->link))->ncircles + (minlinkedchain(circlesarray[currentcol][currentrow]->link))->ncircles;
					(minlinkedchain(circlesarray[currentcol][currentrow]->link))->linkedto = minlinkedchain(circlesarray[comparecol][comparerow]->link);
				}
				else
				{
					(minlinkedchain(circlesarray[currentcol][currentrow]->link))->ncircles = (minlinkedchain(circlesarray[comparecol][comparerow]->link))->ncircles + (minlinkedchain(circlesarray[currentcol][currentrow]->link))->ncircles;
					(minlinkedchain(circlesarray[comparecol][comparerow]->link))->linkedto = minlinkedchain(circlesarray[currentcol][currentrow]->link);
				}
			}
		}
		else if((circlesarray[currentcol][currentrow]->link)->linknumber == 0)
		{
			chain * newlink;
			newlink = new chain;
			newlink->linknumber=linkcount;
			newlink->ncircles = (circlesarray[currentcol][currentrow]->link)->ncircles + (circlesarray[comparecol][comparerow]->link)->ncircles;
			newlink->linkedto=newlink;
			circlesarray[currentcol][currentrow]->link = newlink;
			circlesarray[comparecol][comparerow]->link = newlink;
			linkcount++;
		}
	}
	return linkcount;
}

int main()
{
	circle * minicircles[Width][Height];
	float radius;  //radius of circles
	float tiltrestrict=0;
	float azirestrict=0;
	int linky;   //label for links
	int perc;
	chain * unlinked;
//char qw;

	srand(time(0));

	unlinked=new chain;
	unlinked->linknumber=0;
	unlinked->ncircles=1;
	unlinked->linkedto=unlinked;


	cout<<"Enter amount of tilting restriction (in degrees, 0 to 90): ";
	cin>>tiltrestrict;
	while(tiltrestrict<0 || tiltrestrict>90)
	{
		cout<<"Error -- This entry must be between 0 and 90.  Please try again: ";
		cin>>tiltrestrict;
	}

	cout<<"Enter amount of azimuthal restriction (in degrees, 0 to 90): ";
	cin>>azirestrict;
	while(azirestrict<0 || azirestrict>90)
	{
		cout<<"Error -- This entry must be between 0 and 90.  Please try again: ";
		cin>>azirestrict;
	}

	int valencecounts[44][500];
	for(int initval=0;initval<44;initval++)
	{
		for(int initrange=0;initrange<500;initrange++)
			valencecounts[initval][initrange]=0;
	}

    ofstream fsavec("valencedata001.txt",ios::out);

for(int trial=0;trial<ntrials;trial++)
{
//	first=1;
	linky=1;
	perc=0;

	for(int row=0;row<Height;row++)
	{
		for(int col=0;col<Width;col++)
		{
			minicircles[col][row]=new circle;
			(minicircles[col][row]->center).x=(float)col;
			(minicircles[col][row]->center).y=(float)row;
			(minicircles[col][row]->center).z=0;
			minicircles[col][row]->normalvector=randomunitvector(tiltrestrict,azirestrict);
			minicircles[col][row]->link=unlinked;
		}
	}

	radius=0.5;

	while(radius<0.51)   //only going through one time to get distances
	{
		linky=comparelinks(minicircles, 3,3, 4,3, radius, linky, 0);
		linky=comparelinks(minicircles, 3,3, 4,4, radius, linky, 1);
		linky=comparelinks(minicircles, 3,3, 3,4, radius, linky, 2);
		linky=comparelinks(minicircles, 3,3, 2,4, radius, linky, 3);
		linky=comparelinks(minicircles, 3,3, 2,3, radius, linky, 4);
		linky=comparelinks(minicircles, 3,3, 2,2, radius, linky, 5);
		linky=comparelinks(minicircles, 3,3, 3,2, radius, linky, 6);
		linky=comparelinks(minicircles, 3,3, 4,2, radius, linky, 7);
		linky=comparelinks(minicircles, 3,3, 5,3, radius, linky, 8);
		linky=comparelinks(minicircles, 3,3, 5,4, radius, linky, 9);
		linky=comparelinks(minicircles, 3,3, 5,5, radius, linky, 10);
		linky=comparelinks(minicircles, 3,3, 4,5, radius, linky, 11);
		linky=comparelinks(minicircles, 3,3, 3,5, radius, linky, 12);
		linky=comparelinks(minicircles, 3,3, 2,5, radius, linky, 13);
		linky=comparelinks(minicircles, 3,3, 1,5, radius, linky, 14);
		linky=comparelinks(minicircles, 3,3, 1,4, radius, linky, 15);
		linky=comparelinks(minicircles, 3,3, 1,3, radius, linky, 16);
		linky=comparelinks(minicircles, 3,3, 1,2, radius, linky, 17);
		linky=comparelinks(minicircles, 3,3, 1,1, radius, linky, 18);
		linky=comparelinks(minicircles, 3,3, 2,1, radius, linky, 19);
		linky=comparelinks(minicircles, 3,3, 3,1, radius, linky, 20);
		linky=comparelinks(minicircles, 3,3, 4,1, radius, linky, 21);
		linky=comparelinks(minicircles, 3,3, 5,1, radius, linky, 22);
		linky=comparelinks(minicircles, 3,3, 5,2, radius, linky, 23);
		linky=comparelinks(minicircles, 3,3, 6,3, radius, linky, 24);
		linky=comparelinks(minicircles, 3,3, 6,4, radius, linky, 25);
		linky=comparelinks(minicircles, 3,3, 6,5, radius, linky, 26);
		linky=comparelinks(minicircles, 3,3, 5,6, radius, linky, 27);
		linky=comparelinks(minicircles, 3,3, 4,6, radius, linky, 28);
		linky=comparelinks(minicircles, 3,3, 3,6, radius, linky, 29);
		linky=comparelinks(minicircles, 3,3, 2,6, radius, linky, 30);
		linky=comparelinks(minicircles, 3,3, 1,6, radius, linky, 31);
		linky=comparelinks(minicircles, 3,3, 0,5, radius, linky, 32);
		linky=comparelinks(minicircles, 3,3, 0,4, radius, linky, 33);
		linky=comparelinks(minicircles, 3,3, 0,3, radius, linky, 34);
		linky=comparelinks(minicircles, 3,3, 0,2, radius, linky, 35);
		linky=comparelinks(minicircles, 3,3, 0,1, radius, linky, 36);
		linky=comparelinks(minicircles, 3,3, 1,0, radius, linky, 37);
		linky=comparelinks(minicircles, 3,3, 2,0, radius, linky, 38);
		linky=comparelinks(minicircles, 3,3, 3,0, radius, linky, 39);
		linky=comparelinks(minicircles, 3,3, 4,0, radius, linky, 40);
		linky=comparelinks(minicircles, 3,3, 5,0, radius, linky, 41);
		linky=comparelinks(minicircles, 3,3, 6,1, radius, linky, 42);
		linky=comparelinks(minicircles, 3,3, 6,2, radius, linky, 43);

		float density[44];  // for sorting density values
		float temp;
		int sorted=1;  // to track number of items sorted so far

		for(int item=0;item<44;item++)
		{
			if(minicircles[3][3]->dists[item]>0)  //testing for nans
				density[item]=pow(minicircles[3][3]->dists[item],2);
			else
				density[item]=pow(10,20);  //to get rid of nans
		}

		while(sorted<44)
		{
			int sort=0;
			while(sort<sorted)
			{
				if(density[sorted-sort]<density[sorted-1-sort])
				{
					temp=density[sorted-sort];
					density[sorted-sort]=density[sorted-1-sort];
					density[sorted-1-sort]=temp;
					sort++;
				}
				else
					sort=sorted;
			}
			sorted++;
		}

		for(int val=0;val<44;val++)
		{
			for(int range=0;range<500;range++)
			{
				if(density[val]<=0.25+(float)range/100)
					valencecounts[val][range]++;
			}
		}

		cout<<trial<<endl;

		radius+=0.1;
	}
}

for(int outrange=0;outrange<500;outrange++)
{
	for(int outdist=0;outdist<44;outdist++)
	{
		if(outdist==43)
			fsavec<<valencecounts[outdist][outrange];
		else
			fsavec<<valencecounts[outdist][outrange]<<'\t';
	}
	fsavec<<endl;
}

	fsavec.close();
	return 0;
}


