#include <bits/stdc++.h>
#define ll long long
using namespace std;
int main(){
    //Input variables declration
    int n=6;//total deliveries
    int m=4;//no of delivery locations
    int f[n];//vector for number of items in each delivery location
    int fi[3];//vector for frequency of total number of each item
    char dlname[m];//vector for name of delivery location
    int v[m][2];//vector for all delivery locations items storage (with items stored in int 
                               // such that highest priority item value=0, subsequently 1,2)
    
    
    //taking input frm user
    for(int i=0;i<m;i++)
     cin>>f[i];
    for(int i=0;i<3;i++)
     cin>>fi[i];
    for(int i=0;i<m;i++)
     cin>>dlname[i];
    for(int i=0;i<m;i++)
     for(int j=0;j<2;j++)
      {
          int t;
          cin>>t;
          v[i][j]=t;
      } 


    string d="";//vector for sorted deliver locations
    for(int i=0;i<m;i++){
        sort(v[i],v[i]+2);
    }

    // for(int i=0;i<m;i++)
    //  for(int j=0;j<f[i];j++)
    //   cout<<v[i][j]<<" ";
    // for(int i=0;i<3;i++)
    //  cout<<fi[i]<<" ";
    // cout<<"\nhere\n";
    int i=0;
    int c [m];
    for(int i=0;i<m;i++)
     c[i]=0;
    int j=0; 
    int cnt=0;
    while(i<3){
        cout<<j<<" initial "<<i<<"\n"; 
        if(j==m)
         j=0;
        cout<<j<<" final "<<i<<"\n"; 
        
        while(j<m){
            int pathflag=0;
            if(v[j][c[j]]==i && c[j]<2){
                d+=dlname[j];
                cout<<dlname[j]<<" "<<c[j]<<" "<<j<<"\n";
                c[j]++;
                cnt++;
                pathflag=1;
            }
            if(j!=2)
            j++;
            else if(pathflag)
            j=4; 
            else
             j++;
            if(fi[i]==cnt)
             {i++;
              cnt=0;}
        }
        if(i>2)
         break;
        if(fi[i]==cnt)
             {i++;
              cnt=0;}
    }
    cout<<"Final Order:";
    for (int l=0;l<d.size();l++)
     cout<<d[l]<<" ";
    return 0;

}
