#include <bits/stdc++.h>
using namespace std;

#define pb push_back 
#define nl cout<<endl

vector<int> final_state = {1,2,3,4,5,6,7,8,9};
map<vector<int>, bool> vis;
map<vector<int>, bool> in_queue;		// 1 if in queue, 2 if children expanded
int d, h, expanded, fringe;

int dx[] = {1,-1,0,0};
int dy[] = {0,0,1,-1};
priority_queue <tuple <int, int, vector<int>, vector<int> >> pq; //  { f(n) + h(n) , f(n) , current state , prev state}

vector<vector<int>> moves;
map <vector<int>, vector<int>> backtrack;

void print(vector<int> v)
{
	for(int i=0; i<9; i++)
	{
		if(i%3==0) cout<<endl;
		if(v[i]==9)	cout<<"_  ";
		else cout<<v[i]<<"  ";
	}
	cout<<endl;
}

void next_state(vector<int> state)
{
	int emptyx, emptyy;
	moves.clear();

	for(int i=0; i<9; i++)
		if(state[i] == 9)
			emptyx = i/3, emptyy = i%3;

	for(int i=0; i<4; i++)
	{
		int nextx = emptyx+dx[i], nexty = emptyy+dy[i];
		if(nextx>=0 && nexty>=0 && nextx<3 && nexty<3)
		{
			vector<int> temp = state;
			swap(temp[emptyy + emptyx*3], temp[nexty + nextx*3]);
			moves.pb(temp);
		}
	}
}	

int heuristic(vector<int> &state)
{
	int ans = 0;

	if(h==1)	// mismatch count
	{
		for(int i=0; i<9; i++)
			if(state[i]!=final_state[i])
				ans++;
	}
	else 		// manhattan distance
	{
		for(int i=0; i<9; i++)
		{
			int num = state[i]-1;
			int realrow = num/3, realcol = num%3;
			int row = i/3, col = i%3;
			ans += (abs(row - realrow) + abs(col - realcol));
		}
	}
	return ans;
}

bool equal(vector<int> &state, vector<int> &state2)
{
	for(int i=0; i<9; i++)
		if(state[i]!=state2[i])
			return false;
	return true;
}

void expand(vector<int> &state, int len)
{
	vector<vector<int>> last_expand;
	vector<vector<int>> temp;
	int curr_len = len;
	int depth = d;
	last_expand.pb(state);
	expanded++;
	while(depth--)
	{
		curr_len++;
		for(int i=0; i<last_expand.size(); i++)
		{
			next_state(last_expand[i]);
			for(int j=0; j<moves.size(); j++)
			{
				vector<int> next = moves[j];

				if(vis.find(next)==vis.end())
				{
					temp.pb(next);
					pq.push({-heuristic(next)-curr_len, -curr_len, next, state});
					if(in_queue.find(next)==in_queue.end())
					{
						if(depth)
							expanded++;
					}
					in_queue[next] = 1;
				}
			}
		}
		last_expand.clear();
		for(int i = 0; i<temp.size(); i++)
		{
			vector<int> x = temp[i];
			last_expand.pb(x);
		}
		temp.clear();
		// cout<<in_queue.size()<<" ";
	}
}

int a_star(vector<int> &state, int heu, int depth)
{
	while(!pq.empty())
		pq.pop();

	d = depth;
	h = heu;
	expanded = 0;
	fringe = 0;

	in_queue.clear();
	vis.clear();
	backtrack.clear();
	vector<int> prev(9,0);
	in_queue[state] = 1;
	pq.push({heuristic(state) , 0, state, prev});

	while(!pq.empty())
	{	
		vector<int> curr_state, prev_state;
		int steps;
		tie(ignore, steps, curr_state, prev_state) = pq.top();
		// print(curr_state);
		steps *= -1;
		pq.pop();

		if(vis[curr_state]==1)
			continue;
		vis[curr_state] = 1;

		backtrack[curr_state]  = prev_state;
		if(equal(curr_state, final_state))
			return steps;

		expand(curr_state, steps);
		fringe = max(fringe, (int)in_queue.size() - (int)vis.size());
		// cout<<steps<<" "; 
	}
	return -1;
}

vector<int> random_state()
{
	vector<int> init = final_state;
	int swaps = rand()%40 + 20;

	while(swaps--)
	{
		next_state(init);
		// if(moves.size()==0)
		// 	break;
		int i = rand()%moves.size();
		init = moves[i];
	}
	return init;
}

int main()
{
	// #ifndef ONLINE_JUDGE
	// freopen("input.txt","r",stdin);
	// freopen("output.txt","w",stdout);
	// #endif

	// hue = 1 , mismatch count
	// hue = 2 , manhattan distance
	cout<<"Enter depth : ";
	int depth; cin>>depth;
	nl;

	srand(time(NULL));

	cout<<"The numbers signify (1) steps needed, (2) fringe size, (3) node expanded ",nl;
	for(int i = 0; i<10; i++)
	{
		vector <int> initial_state = random_state();
		// vector <int> initial_state = {4,1,2,5,3,6,7,8,9};
		print(initial_state);
		nl;

		cout<<"depth 1 (mismatch count):\t";
		cout<<""<<a_star(initial_state,1,1)<<"\t"<<fringe<<"\t"<<expanded,nl;

		cout<<"depth 1 (manhattan dist):\t";
		cout<<""<<a_star(initial_state,2,1)<<"\t"<<fringe<<"\t"<<expanded,nl;

		cout<<"depth "<<depth<<" (mismatch count):\t";
		cout<<""<<a_star(initial_state,1,depth)<<"\t"<<fringe<<"\t"<<expanded,nl;

		cout<<"depth "<<depth<<" (manhattan dist):\t";
		cout<<""<<a_star(initial_state,2,depth)<<"\t"<<fringe<<"\t"<<expanded,nl;
		nl;nl;
	}
	cout<<"The results show that manhattan distance is better than mismatch count when solving the puzzle. \n";
	// cerr << "\nTime elapsed : " << 1000 * clock() / CLOCKS_PER_SEC << " ms\n";
	return 0;
}