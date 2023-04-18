#include <bits/stdc++.h>
using namespace std;

struct path{
    int cp;
    string s;//is cp(no of checkpoint) length string with each element L/R/S left right straight representing 
             //turn in each checkpoint just call their turning functions
}
//structure for feeding paths d1->d2 d2->d3 info for bot to traverse

struct destination {
   char c; 
   unordered_map <int,int> m;
   struct destination *next;
   int cp;
   int order;
   int i;//path number according to arena
   int n;
   int 
};

int main(){
    int l;
    int track[l];
    struct destination d1,d2;
    d1.next=d2;
    int cpc=0;
    for(int i=0;i<path[d1.i].cp;i++){
        //straight line following
        if(track[l]==cp)//checkpoint encountered
        {
         cpc++;   
        if(path[d1.i].s[cpc]=='L')
         //call left turn function
        else if(path[d1.i].s[cpc]=='R')
         //call right turn function 
        else if(path[d1.i].s[cpc]=='S')
         //call straighth run function
        } 
    }
}