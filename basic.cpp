#include <bits/stdc++.h>

using namespace std;

using DO = double;
using pii = pair<int, int>;
const DO pi = acosl(-1);
const DO eps = 1e-10;
const DO inf = 1e18;
const int NN = 1000111;
int n;
char tmp[111];

struct point {
	DO x, y;
	point(DO x=0, DO y=0):x(x), y(y) {}
	point operator + (const point& a){return {x+a.x, y+a.y};}
	point operator - (const point& a){return {x-a.x, y-a.y};}
	point operator * (const DO& a){return {x*a, y*a};}
	point operator / (const DO& a){return {x/a, y/a};}
	DO operator ^ (const point& a){return x*a.y-y*a.x;}
	DO operator * (const point& a){return x*a.x+y*a.y;}
	bool operator < (const point& a)const{if(fabsl(x-a.x)<eps) return y+eps<a.y; return x+eps<a.x;}
	bool operator == (const point& a)const{return fabsl(x-a.x)<eps && fabsl(y-a.y)<eps;}
	DO nor(){return sqrtl(x*x+y*y);}
	DO al(){return atan2l(y, x);}
	void input(){
		scanf(" \"x\": %lf, ", &x);
		scanf(" \"y\": %lf, ", &y);
	};
	void print(){cout<<x<<' '<<y<<endl;};
} src, tar;

struct segment {
	point pt[2];
	segment(point st, point ed) {
		pt[0] = st;
		pt[1] = ed;
	}
};

DO sgn(DO a){ return (a>eps)-(a<-eps); }

point inter(point a, point b, point c, point d) {
  	return a+(b-a)*((d-c^c-a)/(d-c^b-a));
}

bool isOn(point a, point b, point c) {
	return a == b or b == c or (abs(b-a ^ c-a) < eps and (b-a).nor() < (c-a).nor());
}

bool isIntersect(point a, point b, point c, point d) {
	if(isOn(a, c, d) or isOn(b, c, d)) return true;
	if(isOn(c, a, b) or isOn(d, a, b)) return true;
	if(abs(b-a ^ d-c) < eps) return false;
	point m = inter(a, b, c, d);
	return isOn(a, m, b) and isOn(c, m, d);
}

DO dist(point a, point b) {
	return (b-a).nor();	
}

bool smin(DO &u, DO v) {
	if(u > v) {
		u = v;
		return true;
	}
	return false;
}


vector<point> points;
vector<segment> gateways;
DO dp[NN][2];
pii prevState[NN][2];

void input() {
	cin >> n;
	scanf(" [ ");
		
	for(int i=0; i<n; i++) {
		scanf(" { \"label\": %s ", tmp);
		point now;
		now.input();
		points.push_back(now);
		scanf(" }, ");
	}
	scanf(" ] ");
	
	scanf(" [ ");
	scanf(" { \"label\": %s ", tmp);
	src.input();
	scanf(" }, ");
	
	scanf(" { \"label\": %s ", tmp);
	tar.input();
	scanf(" }, ");
	scanf(" ] ");
	
	gateways.push_back({src, src});
	for(int i=0; i<n; i+=2) {
		gateways.push_back({points[i], points[i+1]});
	}
	gateways.push_back({tar, tar});
}

void solve() {
	dp[0][0] = 0;
	dp[0][1] = 0;
	int m = gateways.size();
	for(int i=1; i<m; i++) {
		for(int j=0; j<2; j++) {
			dp[i][j] = inf;
			point now = gateways[i].pt[j];
			for(int k=i-1; k>=0; k--) {
				for(int l=0; l<2; l++) {
					point pre = gateways[k].pt[l];
					bool passAll = true;
					for(int p=i-1; p>=k; p--) {
						
						if(isIntersect(now, pre, gateways[p].pt[0], gateways[p].pt[1]) == false) {
							passAll = false;
							break;
						}
					}
					if(passAll) {
						if(smin(dp[i][j], dp[k][l] + dist(pre, now))) {
							prevState[i][j] = {k, l};
						} 
					} else break;
				}
			}
		}
	}
	vector<point> path;
	pii cur = {m-1, 0};
	while(cur.first > 0) {
		int x = cur.first;
		int y = cur.second;
		point q = gateways[x].pt[y];
		path.push_back(q);
		cur = prevState[x][y];
	}
	path.push_back(src);
	cout << "Minimal distance: " << dp[m-1][0] << endl;
	reverse(path.begin(), path.end());
	for(auto p : path) {
		p.print();
		
	}
}

int main() {
	freopen("input.txt", "r", stdin);
    input();
	solve();
}



