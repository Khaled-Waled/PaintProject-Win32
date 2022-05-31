#pragma once
#include <stack>
#include <vector>
#include <list>
#include "Header.h"
#include "LineAlgos.h"
using namsespace std;

const int N = 1e5;
typedef list<EdgeRec> EdgeList;

int dx[]{0,0,1,-1};
int dy[]{1,-1,0,0};

struct point{
    int x;
    int y;
};

struct EdgeRec
{
    double x;
    double minv;
    int ymax;
    bool operator<(EdgeRec r)
    {
        return x<r.x;
    }
};


//////////////////////////////////////////////////////////////////////////////////////////

 void fillingCircleWithLines(HDC hdc,int xc,int yc, int r,COLORREF color, int which)
{
    int x=0;
    int y=(r-1);
    int d=1-r;
    int c1=3, c2=5-(2*r);
    FillQuatrerLines(hdc,x,y,xc,yc,color,which);
    while(x<y)
    {
        if(d<0)
        {
            d+=c1;
            c2+=2;
        }
        else
        {
            d+=c2;
            c2+=4;
            y--;
        }
        c1+=2;
        x++;
        FillQuatrerLines(hdc,x,y,xc,yc,color,which);
    }
}

 void FillQuatrerLines(HDC hdc,int x,int y,int xc,int yc,COLORREF color,int which)
{
    /*

     4 | 1
     -----
     3 | 2

    */

    if(which==1) // top right
    {
        DrawLine1(hdc,xc+y,yc-x,xc,yc,color);
        DrawLine1(hdc,xc+x,yc-y,xc,yc,color);
    }
    if(which==2) // down right
    {
        DrawLine1(hdc,xc+x,yc+y,xc,yc,color);
        DrawLine1(hdc,xc+y,yc+x,xc,yc,color);
    }
    if(which==3) // down left
    {
        DrawLine1(hdc,xc-y,yc+x,xc,yc,color);
        DrawLine1(hdc,xc-x,yc+y,xc,yc,color);
    }
    if(which==4) // top left
    {
        DrawLine1(hdc,xc-x,yc-y,xc,yc,color);
        DrawLine1(hdc,xc-y,yc-x,xc,yc,color);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

 void fillingCircleWithCircles(HDC hdc,int xc,int yc, int r,COLORREF color, int which)
{
    while(r>0)
    {
        r--;
        filling(hdc,xc,yc, r, color, which);
    }
}

 void filling(HDC hdc,int xc,int yc, int r,COLORREF color, int which)
{
    int x=0;
    int y=r;
    int d=1-r;
    int c1=3, c2=5-(2*r);
    FillQuatrerCircles(hdc,x,y,xc,yc,color,which);
    while(x<y)
    {
        if(d<0)
        {
            d+=c1;
            c2+=2;
        }
        else
        {
            d+=c2;
            c2+=4;
            y--;
        }
        c1+=2;
        x++;
        FillQuatrerCircles(hdc,x,y,xc,yc,color,which);
    }
}

 void FillQuatrerCircles(HDC hdc,int x,int y,int xc,int yc,COLORREF color,int which)
{
    /*

     4 | 1
     -----
     3 | 2

    */

    if(which==1) // top right
    {
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc+x,yc-y,color);
    }
    if(which==2) // down right
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc+y,yc+x,color);
    }
    if(which==3) // down left
    {
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc-x,yc+y,color);
    }
    if(which==4) // top left
    {
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc-y,yc-x,color);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

 void myFloodFillRec(HDC hdc,int x , int y , COLORREF border , COLORREF filling)
{
    COLORREF color = GetPixel(hdc,x,y);
    if(color == border or color == filling)
    {
        return;
    }
    SetPixel(hdc,x,y,filling);
    myFloodFillRec(hdc,x+1,y,border,filling);
    myFloodFillRec(hdc,x-1,y,border,filling);
    myFloodFillRec(hdc,x,y+1,border,filling);
    myFloodFillRec(hdc,x,y-1,border,filling);
}

//////////////////////////////////////////////////////////////////////////////////////////

 void myFloodFillNonRec(HDC hdc , int x , int y , COLORREF border , COLORREF filling)
{
    stack<point>st;
    point p;
    p.x=x;
    p.y=y;
    st.push(p);

    while(!st.empty())
    {
        point p1 = st.top();
        st.pop();
        COLORREF color = GetPixel(hdc,p1.x,p1.y);
        if(color == border or color == filling)
        {
            continue;
        }
        SetPixel(hdc,p1.x,p1.y,filling);
        point pp1,pp2,pp3,pp4;
        pp1.x=p1.x+1;pp1.y=p1.y;
        pp2.x=p1.x-1;pp2.y=p1.y;
        pp3.x=p1.x;pp3.y=p1.y+1;
        pp4.x=p1.x;pp4.y=p1.y-1;
        st.push(pp1);
        st.push(pp2);
        st.push(pp3);
        st.push(pp4);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

 EdgeRec InitEdgeRec(point& v1,point& v2)
{
    if(v1.y > v2.y)
    {
        point v3=v1;
        v1=v2;
        v2=v3;
    }
    EdgeRec rec;
    rec.x=v1.x;
    rec.ymax=v2.y;
    rec.minv=(double)(v2.x-v1.x)/(v2.y-v1.y);
    return rec;
}

 void InitEdgeTable(vector<point>&polygon,int n,EdgeList table[])
{
    point v1=polygon[n-1];
    for(int i=0;i<n;i++)
    {
        point v2=polygon[i];
        if(v1.y==v2.y){v1=v2;continue;}
        EdgeRec rec=InitEdgeRec(v1, v2);
        table[v1.y].push_back(rec);
        v1=polygon[i];
    }
}


 void GeneralPolygonFill(HDC hdc,vector<point>&polygon,int n,COLORREF c)
{
    EdgeList *table=new EdgeList [N];
    InitEdgeTable(polygon,n,table);
    int y=0;
    while(y<N and table[y].size()==0)y++;
    if(y==N)return;
    EdgeList ActiveList=table[y];
    while(ActiveList.size()>0)
    {
        ActiveList.sort();
        for(EdgeList::iterator it=ActiveList.begin();it!=ActiveList.end();it++)
        {
            int x1=(int)ceil(it->x);
            it++;
            int x2=(int)floor(it->x);
            for(int x=x1;x<=x2;x++)SetPixel(hdc,x,y,c);
        }
        y++;
        EdgeList::iterator it=ActiveList.begin();
        while(it!=ActiveList.end())
        if(y==it->ymax) it=ActiveList.erase(it);
        else it++;
        for(EdgeList::iterator it=ActiveList.begin();it!=ActiveList.end();it++)
            it->x+=it->minv;
        ActiveList.insert(ActiveList.end(),table[y].begin(),table[y].end());
    }
    delete[] table;
}
