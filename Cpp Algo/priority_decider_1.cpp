#include <bits/stdc++.h>
#define ll long long
using namespace std;
int main(){
    //Input variables declration
    int n;//total deliveries
    int m;//no of delivery locations
    cin>>n>>m; 
    vector <int> f (m);//vector for number of items in each delivery location
    vector <int> fi (3);//vector for frequency of total number of each item
    vector<char> dlname (m);//vector for name of delivery location
    vector <vector<int>> v (m);//vector for all delivery locations items storage (with items stored in int 
                               // such that highest priority item value=0, subsequently 1,2)
    
    
    //taking input frm user
    for(int i=0;i<m;i++)
     cin>>f[i];
    for(int i=0;i<3;i++)
     cin>>fi[i];
    for(int i=0;i<m;i++)
     cin>>dlname[i];
    for(int i=0;i<m;i++)
     for(int j=0;j<f[i];j++)
      {
          int t;
          cin>>t;
          v[i].push_back(t);
      }   



    vector <char> d (n);//vector for sorted deliver locations
    for(int i=0;i<m;i++){
        sort(v[i].begin(),v[i].end());
    }

    // for(int i=0;i<m;i++)
    //  for(int j=0;j<f[i];j++)
    //   cout<<v[i][j]<<" ";
    // for(int i=0;i<3;i++)
    //  cout<<fi[i]<<" ";
    // cout<<"\nhere\n";
    int i=0;
    vector <int> c (m,0);
    while(i<3){
        int j=0;
        int cnt=0;
        while(j<m){
            if(v[j][c[j]]==i){
                d.push_back(dlname[j]);
                //cout<<dlname[j]<<" ";
                c[j]++;
                cnt++;
            }
            j++;
        }
        if(fi[i]==cnt)
         i++;
    }
    cout<<"Final Order:";
    for (int l=0;l<d.size();l++)
     cout<<d[l]<<" ";
    return 0;

}