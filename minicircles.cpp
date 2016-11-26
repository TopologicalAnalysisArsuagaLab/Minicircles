
//============================================================================
// Name        : minicircles.cpp
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

const long ntrials=10;
const float pi=3.14159265358979324;
const int digits=20;
const int Height=1000;
const int Width=1000;

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
	int linknumber;  //cluster number
	int ncircles;     // number of minicircles in the chain
	int checked;
	chain * linkedto;
};

struct circle {
	point center;
	point normalvector;
	chain * link;
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

int linked(circle * circle1, circle * circle2, float radius, int firsttime)
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

		if(distanc(intersect,circle1->center)<radius && distanc(intersect,circle2->center)<radius)
			return 1;
		else
		{
			return 0;
		}
}

int minlinknum(circle * circle1)  // finds link (cluster) number of a circle
{
	chain * currentchain;
	currentchain=circle1->link;
	while(currentchain->linknumber != (currentchain->linkedto)->linknumber)
	{
		currentchain=currentchain->linkedto;
	}
	return currentchain->linknumber;
}

struct chain * minlinkedchain(chain * currentchain)  // combines two chains
{
	chain * findmin;
	findmin=currentchain;

	while(findmin->linknumber != (findmin->linkedto)->linknumber)
	{
		findmin=findmin->linkedto;
	}
	return findmin;
}

int comparelinks(circle * circlesarray[Width][Height], int currentrow, int currentcol, int comparerow, int comparecol, float circleradius, int linkcount, int firsttime)
{   //updates clusters if two circles are linked
	if(linked(circlesarray[currentcol][currentrow],circlesarray[comparecol][comparerow],circleradius,firsttime)==1)
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

int percolation(circle * circlesarray[Width][Height])
{
	int test;

	for(int testcol=0;testcol<Width;testcol++)
	{
		if(minlinknum(circlesarray[testcol][0])>0)
		{
			test=minlinknum(circlesarray[testcol][0]);
			for(int comparecol=0;comparecol<Width;comparecol++)
			{
				if(minlinknum(circlesarray[comparecol][Height-1])==test)
					return 1;
			}
		}
	}

	for(int testrow=0;testrow<Width;testrow++)
	{
		if(minlinknum(circlesarray[0][testrow])>0)
		{
			test=minlinknum(circlesarray[0][testrow]);
			for(int comparerow=0;comparerow<Height;comparerow++)
			{
				if(minlinknum(circlesarray[Width-1][comparerow])==test)
					return 1;
			}
		}
	}

	return 0;
}

int main()
{
	circle * minicircles[Width][Height];
	float radius;  //radius of circles
	int linky;   //label for links
	int perc;
	int sat99;
	char runtype;
	float tiltrestrict=0;
	float azirestrict=0;
	float startradius;
	char filename[17];
	char qw;
	chain * unlinked;

	srand(time(0));

	unlinked=new chain;
	unlinked->linknumber=0;
	unlinked->ncircles=1;
	unlinked->checked=0;
	unlinked->linkedto=unlinked;

	cout<<"Check for percolation (p) or saturation (s)? ";
	cin>>runtype;
	while(runtype != 'p' && runtype !='s')
	{
		cout<<"Error -- Please enter 'p' or 's': ";
		cin>>runtype;
	}
	if(runtype=='p')
	{
		filename[0]='p';
		filename[1]='e';
		filename[2]='r';
		filename[3]='c';
		filename[4]='_';
	}
	if(runtype=='s')
	{
		filename[0]='s';
		filename[1]='a';
		filename[2]='t';
		filename[3]='u';
		filename[4]='_';
	}
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

	cout<<"Enter starting radius: ";
	cin>>startradius;

	filename[5]=(char)((int)(tiltrestrict/10)+48);
	filename[6]=(char)((int)tiltrestrict%10+48);
	filename[7]=(char)((int)(tiltrestrict*10)%10+48);
	filename[8]='_';
	filename[9]=(char)((int)(azirestrict/10)+48);
	filename[10]=(char)((int)azirestrict%10+48);
	filename[11]=(char)((int)(azirestrict*10)%10+48);
	filename[12]='.';
	filename[13]='t';
	filename[14]='x';
	filename[15]='t';
	filename[16]='\0';

    ofstream fsavec(filename,ios::out);

for(int trial=0;trial<ntrials;trial++)
{
	linky=1;

	if(runtype=='p')
	{
		sat99=1;
		perc=0;
	}
	if(runtype=='s')
	{
		sat99=0;
		perc=1;
	}

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

	radius=startradius;

	while(perc==0 || sat99==0)
	{
		for(int row=0;row<Height;row++)
		{
			for(int col=0;col<Width;col++)
			{
				if(row==Height-1)
				{
					if(col<Width-1)
					{
						if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row])->link)->linknumber==0))
							linky=comparelinks(minicircles, row, col, row, col+1, radius, linky, trial);
					}
				}
				else if(col==Width-1)
				{
					if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-1][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-1][row+1])->link)->linknumber==0))
						linky=comparelinks(minicircles, row, col, row+1, col-1, radius, linky, trial);
					if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col][row+1])->link)->linknumber==0))
						linky=comparelinks(minicircles, row, col, row+1, col, radius, linky, trial);
				}
				else if(col==0)
				{
					if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col][row+1])->link)->linknumber==0))
						linky=comparelinks(minicircles, row, col, row+1, col, radius, linky, trial);
					if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row+1])->link)->linknumber==0))
						linky=comparelinks(minicircles, row, col, row+1, col+1, radius, linky, trial);
					if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row])->link)->linknumber==0))
						linky=comparelinks(minicircles, row, col, row, col+1, radius, linky, trial);
				}
				else
				{
					if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-1][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-1][row+1])->link)->linknumber==0))
						linky=comparelinks(minicircles, row, col, row+1, col-1, radius, linky, trial);
					if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col][row+1])->link)->linknumber==0))
						linky=comparelinks(minicircles, row, col, row+1, col, radius, linky, trial);
					if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row+1])->link)->linknumber==0))
						linky=comparelinks(minicircles, row, col, row+1, col+1, radius, linky, trial);
					if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row])->link)->linknumber==0))
						linky=comparelinks(minicircles, row, col, row, col+1, radius, linky, trial);
				}

				if(radius>1)
				{
					if(row>=Height-2)
					{
						if(col<Width-2)
						{
							if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row])->link)->linknumber==0))
								linky=comparelinks(minicircles, row, col, row, col+2, radius, linky, trial);
						}
					}
					else if(col>=Width-2)
					{
						if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col][row+2])->link)->linknumber==0))
							linky=comparelinks(minicircles, row, col, row+2, col, radius, linky, trial);
					}
					else
					{
						if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row])->link)->linknumber==0))
							linky=comparelinks(minicircles, row, col, row, col+2, radius, linky, trial);
						if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col][row+2])->link)->linknumber==0))
							linky=comparelinks(minicircles, row, col, row+2, col, radius, linky, trial);
					}

					if(radius>sqrt(5)/2)
					{
						if(row>=Height-2)
						{
							if(row==Height-2)
							{
								if(col<Width-2)
								{
									if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row+1])->link)->linknumber==0))
										linky=comparelinks(minicircles, row, col, row+1, col+2, radius, linky, trial);
								}
							}
						}
						else if(col>=Width-2)
						{
							if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-2][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-2][row+1])->link)->linknumber==0))
								linky=comparelinks(minicircles, row, col, row+1, col-2, radius, linky, trial);
							if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-1][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-1][row+2])->link)->linknumber==0))
								linky=comparelinks(minicircles, row, col, row+2, col-1, radius, linky, trial);
							if(col==Width-2)
							{
								if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row+2])->link)->linknumber==0))
									linky=comparelinks(minicircles, row, col, row+2, col+1, radius, linky, trial);
							}
						}
						else if(col<=1)
						{
							if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row+2])->link)->linknumber==0))
								linky=comparelinks(minicircles, row, col, row+2, col+1, radius, linky, trial);
							if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row+1])->link)->linknumber==0))
								linky=comparelinks(minicircles, row, col, row+1, col+2, radius, linky, trial);
							if(col==1)
							{
								if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-1][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-1][row+2])->link)->linknumber==0))
									linky=comparelinks(minicircles, row, col, row+2, col-1, radius, linky, trial);
							}
						}
						else
						{
							if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-2][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-2][row+1])->link)->linknumber==0))
								linky=comparelinks(minicircles, row, col, row+1, col-2, radius, linky, trial);
							if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-1][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-1][row+2])->link)->linknumber==0))
								linky=comparelinks(minicircles, row, col, row+2, col-1, radius, linky, trial);
							if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row+2])->link)->linknumber==0))
								linky=comparelinks(minicircles, row, col, row+2, col+1, radius, linky, trial);
							if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row+1])->link)->linknumber==0))
								linky=comparelinks(minicircles, row, col, row+1, col+2, radius, linky, trial);
						}

						if(radius>sqrt(2))
						{
							if(row<Height-2)
							{
								if(col>=Width-2)
								{
									if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-2][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-2][row+2])->link)->linknumber==0))
										linky=comparelinks(minicircles, row, col, row+2, col-2, radius, linky, trial);
								}
								else if(col<=1)
								{
									if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row+2])->link)->linknumber==0))
										linky=comparelinks(minicircles, row, col, row+2, col+2, radius, linky, trial);
								}
								else
								{
									if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-2][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-2][row+2])->link)->linknumber==0))
										linky=comparelinks(minicircles, row, col, row+2, col-2, radius, linky, trial);
									if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row+2])->link)->linknumber==0))
										linky=comparelinks(minicircles, row, col, row+2, col+2, radius, linky, trial);
								}
							}

							if(radius>1.5)
							{
								if(row>=Height-3)
								{	if(col<Width-3)
								{
									if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row])->link)->linknumber==0))
										linky=comparelinks(minicircles, row, col, row, col+3, radius, linky, trial);
								}
								}
								else if(col>=Width-3)
								{
									if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col][row+3])->link)->linknumber==0))
										linky=comparelinks(minicircles, row, col, row+3, col, radius, linky, trial);
								}
								else
								{
									if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row])->link)->linknumber==0))
										linky=comparelinks(minicircles, row, col, row, col+3, radius, linky, trial);
									if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col][row+3])->link)->linknumber==0))
										linky=comparelinks(minicircles, row, col, row+3, col, radius, linky, trial);
								}

								if(radius>sqrt(10)/2)
								{
									if(row>=Height-3)
									{
										if(row<Height-1)
										{
											if(col<3)
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row+1])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+1, col+3, radius, linky, trial);
											}
											else if(col>Width-4)
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-3][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-3][row+1])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+1, col-3, radius, linky, trial);
											}
											else
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row+1])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+1, col+3, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-3][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-3][row+1])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+1, col-3, radius, linky, trial);
											}
										}
									}
									else
									{
										if(col==0)
										{
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row+1])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+1, col+3, radius, linky, trial);
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row+3])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+3, col+1, radius, linky, trial);
										}
										else if(col==1 || col==2)
										{
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row+1])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+1, col+3, radius, linky, trial);
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row+3])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+3, col+1, radius, linky, trial);
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-1][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-1][row+3])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+3, col-1, radius, linky, trial);
										}
										else if(col==Width-1)
										{
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-3][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-3][row+1])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+1, col-3, radius, linky, trial);
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-1][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-1][row+3])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+3, col-1, radius, linky, trial);
										}
										else if(col==Width-2 || col==Width-3)
										{
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-3][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-3][row+1])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+1, col-3, radius, linky, trial);
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-1][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-1][row+3])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+3, col-1, radius, linky, trial);
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row+3])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+3, col+1, radius, linky, trial);
										}
										else
										{
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-3][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-3][row+1])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+1, col-3, radius, linky, trial);
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-1][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-1][row+3])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+3, col-1, radius, linky, trial);
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+1][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+1][row+3])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+3, col+1, radius, linky, trial);
											if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row+1]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row+1])->link)->linknumber==0))
												linky=comparelinks(minicircles, row, col, row+1, col+3, radius, linky, trial);
										}
									}

									if(radius>sqrt(13)/2)
									{
										if(row>=Height-3)
										{
											if(row==Height-3)
											{
												if(col<3)
												{
													if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row+2])->link)->linknumber==0))
														linky=comparelinks(minicircles, row, col, row+2, col+3, radius, linky, trial);
												}
												else if(col>Width-4)
												{
													if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-3][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-3][row+2])->link)->linknumber==0))
														linky=comparelinks(minicircles, row, col, row+2, col-3, radius, linky, trial);
												}
												else
												{
													if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row+2])->link)->linknumber==0))
														linky=comparelinks(minicircles, row, col, row+2, col+3, radius, linky, trial);
													if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-3][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-3][row+2])->link)->linknumber==0))
														linky=comparelinks(minicircles, row, col, row+2, col-3, radius, linky, trial);
												}
											}
										}
										else
										{
											if(col==0 || col==1)
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row+2])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+2, col+3, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row+3])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+3, col+2, radius, linky, trial);
											}
											else if(col==2)
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row+2])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+2, col+3, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row+3])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+3, col+2, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-2][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-2][row+3])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+3, col-2, radius, linky, trial);
											}
											else if(col==Width-1 || col==Width-2)
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-3][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-3][row+2])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+2, col-3, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-2][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-2][row+3])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+3, col-2, radius, linky, trial);
											}
											else if(col==Width-3)
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-3][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-3][row+2])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+2, col-3, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-2][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-2][row+3])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+3, col-2, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row+3])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+3, col+2, radius, linky, trial);
											}
											else
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-3][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-3][row+2])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+2, col-3, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col-2][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col-2][row+3])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+3, col-2, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+2][row+3]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+2][row+3])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+3, col+2, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+3][row+2]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+3][row+2])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+2, col+3, radius, linky, trial);
											}
										}

										if(radius>2)
										{
											if(row>=Height-4)
											{	if(col<Width-4)
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+4][row]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+4][row])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row, col+4, radius, linky, trial);
											}
											}
											else if(col>=Width-4)
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col][row+4]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col][row+4])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+4, col, radius, linky, trial);
											}
											else
											{
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col+4][row]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col+4][row])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row, col+4, radius, linky, trial);
												if(minlinknum(minicircles[col][row]) != minlinknum(minicircles[col][row+4]) || (((minicircles[col][row])->link)->linknumber==0 && ((minicircles[col][row+4])->link)->linknumber==0))
													linky=comparelinks(minicircles, row, col, row+4, col, radius, linky, trial);
											}

											if(radius>sqrt(17)/2)
											{
												cout<<"Need to consider case radius > sqrt(17)/2\n";

												if(radius>3*sqrt(2)/2)
												{
													cout<<"Need to consider case radius > 3sqrt(2)/2\n";
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

/*		cout<<"First row: ";
		for(int firstrowcol=0;firstrowcol<Width;firstrowcol++)   //This section outputs only first and last rows and columns of link numbers
		{
			cout<<minlinknum(minicircles[firstrowcol][0])<<'\t';
		}
		cout<<endl;

		cout<<"Last row: ";
		for(int lastrowcol=0;lastrowcol<Width;lastrowcol++)
		{
			cout<<minlinknum(minicircles[lastrowcol][Height-1])<<'\t';
		}
		cout<<endl<<endl;

		cout<<"First column: ";
		for(int firstcolrow=0;firstcolrow<Width;firstcolrow++)
		{
			cout<<minlinknum(minicircles[0][firstcolrow])<<'\t';
		}
		cout<<endl;

		cout<<"Last column: ";
		for(int lastcolrow=0;lastcolrow<Width;lastcolrow++)
		{
			cout<<minlinknum(minicircles[Width-1][lastcolrow])<<'\t';
		}
		cout<<endl<<endl;*/

//  /*  begin PERCOLATION
		if(perc==0)
		{
			//		char qw;
			if(percolation(minicircles))
			{
				perc=1;
				cout<<"trial "<<trial<<" -- radius at which percolation is achieved: "<<radius<<endl;
//				fsavec<<radius<<endl;
				//			cin>>qw;
			}
			else
				radius+=0.001;
		} //  end PERCOLATION */

 /*		percolation chain distribution
		if(perc==1)
		{
			int countunlinked=0;
			for(int nrow=0;nrow<Height;nrow++)
			{
				for(int ncol=0;ncol<Width;ncol++)
				{
					if(minlinknum(minicircles[ncol][nrow])==0)
						countunlinked++;
					else if(minlinkedchain((minicircles[ncol][nrow])->link)->checked==0)
					{
						fsavec<<minlinkedchain((minicircles[ncol][nrow])->link)->linknumber<<'\t'<<minlinkedchain((minicircles[ncol][nrow])->link)->ncircles<<endl;
						minlinkedchain((minicircles[ncol][nrow])->link)->checked=1;
					}
				}
			}
			fsavec<<"0\t"<<countunlinked<<endl;
		} //  */

 /*                This section outputs the entire array of link numbers
	//	if(perc==1)
	//	{
//		char qw='y';
 	 	cout<<"Display entire array? (y/n) ";
		cin>>qw;
		if(qw=='y')
		{
			fsavec<<endl;
			fsavec<<radius<<endl;
			for(int outrow=0;outrow<Height;outrow++)              //
			{
				for(int outcol=0;outcol<Width;outcol++)
				{
					fsavec<<minlinknum(minicircles[outcol][outrow])<<'\t';
				}
				fsavec<<endl;
			}
			cout<<endl;
		} //}
//		*/

// /*  begin 99% SATURATION
		if(sat99==0)
		{
			//		cout<<"Trial "<<trial<<"  R = "<<radius<<":  Largest chain is chain number ";
			int largechainnum, largechaincount;
			largechaincount=0;
			for(int findlargerow=0;findlargerow<Height;findlargerow++)
			{
				for(int findlargecol=0;findlargecol<Width;findlargecol++)
				{
					if((minlinkedchain((minicircles[findlargecol][findlargerow])->link))->ncircles > largechaincount)
					{
						largechaincount=(minlinkedchain((minicircles[findlargecol][findlargerow])->link))->ncircles;
						largechainnum=(minlinkedchain((minicircles[findlargecol][findlargerow])->link))->linknumber;
					}
				}
			}
			//		cout<<largechainnum<<", containing "<<largechaincount<<" minicircles.\n\n";
			//		fsavec<<pow(radius,2)<<'\t'<<largechaincount<<endl;

			if(largechaincount>=0.99*Height*Width && sat99==0)
			{
				cout<<"trial "<<trial<<" -- radius at which 99% saturation is achieved: "<<radius<<endl;
				sat99=1;
//				fsavec<<radius<<endl;
			}
			else
				radius+=0.001;
//			if(largechaincount<0.985*Height*Width)
//				radius+=0.01;
//			else
//				radius+=0.001;
			//		cout<<radius<<endl;
		}
// end 99% SATURATION */

// /*  section to adjust startradius if started too large
		if(perc==1 && sat99==1)
		{
			if(radius==startradius)
			{
				if(runtype=='s')
					sat99=0;
				if(runtype=='p')
					perc=0;

				startradius-=0.002;
				radius=startradius;

				for(int row=0;row<Height;row++)
				{
					for(int col=0;col<Width;col++)
					{
						minicircles[col][row]->link=unlinked;
					}
				}
			}
			else
			{
				fsavec<<radius<<endl;  //check this
			}
		}
//		radius+=0.001;  // */
	}
//	cout<<trial<<'\t'<<radius<<endl;
}

	fsavec.close();
	return 0;
}
