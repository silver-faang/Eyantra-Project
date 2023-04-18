#include <bits/stdc++.h>
#define ll long long
using namespace std;
int main(){
    string d="AHO";
    char cs='S';
    char ns=d[0];
    int i=0;
    while(i!=d.length()){
        if(cs=='S'){
            if(ns=='A'){
                int cp=2;
                int d=1;
                int tv=1;
                string t="R";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
            else if(ns=='H'){
                int cp=4;
                int d=2;
                int tv=1;
                string t="R";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
            else if(ns=='O'){
                int cp=5;
                int d=3;
                int tv=2;
                string t="RR";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
            else if(ns=='E'){
                int cp=5;
                int d=3;
                int tv=2;
                string t="RL";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
        }
        else if(cs=='A'){
            if(ns=='H'){
                int cp=2;
                int d=1;
                int tv=0;
                string t="";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
            else if(ns=='O'){
                int cp=3;
                int d=2;
                int tv=1;
                string t="R";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
            else if(ns=='E'){
                int cp=3;
                int d=2;
                int tv=1;
                string t="L";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            } 
        }
        else if(cs=='H'){
            if(ns=='A'){
                int cp=3;
                int d=2;
                int tv=2;
                string t="LL";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
                cs=ns;
                i++;
            }
            else if(ns=='O'){
                int cp=1;
                int d=1;
                int tv=1;
                string t="R";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
            else if(ns=='E'){
                int cp=1;
                int d=1;
                int tv=1;
                string t="L";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            } 
        }
        else if(cs=='O'){
            if(ns=='A'){
                int cp=2;
                int d=1;
                int tv=1;
                string t="L";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
            else if(ns=='H'){
                int cp=4;
                int d=2;
                int tv=1;
                string t="L";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
            else if(ns=='E'){
                int cp=5;
                int d=3;
                int tv=2;
                string t="LL";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            } 
        }
        else if(cs=='E'){
            if(ns=='A'){
                int cp=2;
                int d=1;
                int tv=1;
                string t="L";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
            else if(ns=='H'){
                int cp=4;
                int d=2;
                int tv=1;
                string t="L";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            }
            else if(ns=='O'){
                int cp=5;
                int d=3;
                int tv=2;
                string t="LR";
                cout<<cp<<" "<<d<<" "<<t<<" "<<tv<<"\n";
                cs=ns;
                i++;
            } 
        }
        ns=d[i];
    }
    return 0;
}