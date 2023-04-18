#include <bits/stdc++.h>
using namespace std;

struct destination {
   char c; 
   unordered_map <int,int> m;
   int prev;
   int next;
   int order;
   int i;//number according to arena
   int n;
};

bool static comparator(struct destination d1, destination d2) {
         int l=0;
          //for
         while(l<3){ 
         if(d1.m.find(l)==d1.m.end() && d2.m.find(l)!=d2.m.end())
          return false;
         else if(d1.m.find(l)!=d1.m.end() && d2.m.find(l)==d2.m.end())
          return true;
         else if(d1.m.find(l)==d1.m.end() && d2.m.find(l)==d2.m.end())
          continue;
         else if (d1.m[l]>d2.m[l])
          return true;
         else if(d1.[l]<d2.m[l])
          return false;   
         l++;}
         else if(d1.n>d2.n)
          return true;
         else
          return false;
      }
int main(){
    int n;
    struct destination dest[n];
    for(int i=0;i<n;i++){
        dest[i].c=i+'0';//input deliver lloc allocator 
    }
    sort(dest,dest+n,comparator);
    return dest[0].n;//return 1st destination's no of items to be delivered for no reason
}      