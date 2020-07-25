#include <bits/stdc++.h>
using namespace std;

#define pb push_back 
#define nl cout<<"\n"

int no_of_var, no_of_clauses, maxit, maxv, found, no_of_steps;
double prob;
vector<vector<int>> clause;
vector<bool> curr;
set<int> s;
map<int,int> freq;

void modifiedWalkSAT();

void print_assignment()
{
	for(int i=0; i<no_of_var; i++)
		cout<<i+1<<"  \t"<<curr[i+1],nl;
	nl;
}

// randomly generates sat problems 
void solve_random(int n, int ksat, int clauses_of_each_type)
{
	freopen("output.txt","w",stdout);
	// generates k-CNF problems
	maxit = 100;
	maxv = n;
	int m = n/4;
	no_of_clauses = m;
	no_of_var = n;
	prob = 0.5;

	cout<<"m/n \t,log(Average iterations)",nl;

	for(int i=0; i<40; i++)
	{
		int count = clauses_of_each_type;
		int sat = 0, total_iter = 0;
		while(count--)
		{
			clause.resize(no_of_clauses);
			for(int j = 0; j<no_of_clauses; j++)
			{
				clause[j].clear();
				for(int k=0; k<ksat; k++)
				{
					int num = rand()%no_of_var + 1;
					int sign = rand()%2;
					if(sign) num *= -1;
					clause[j].pb(num);
				}
			}
			modifiedWalkSAT();
			// cout<<"m/n : "<<(double)no_of_clauses/(double)no_of_var;
			if(found)
			{
				sat++;
				total_iter += no_of_steps;
				// cout<<" iter : "<<no_of_steps,nl;
			}
		}

		if(sat)
		{
			cout<<(double)no_of_clauses/(double)no_of_var;
			cout<<"\t,"<<log((double)total_iter/(double)sat),nl;
			fflush(stdout);
		}
		no_of_clauses += m;
	}
}

// to solve SAT from .cnf file
void solve()
{
	string filename;
	cin>>filename;
	nl;
	cout<<"Max v : "; cin>>maxv;
	cout<<"Max it : "; cin>>maxit;
	cout<<"Prob : "; cin>>prob;
	nl;

	freopen(filename.c_str(),"r",stdin);

	no_of_clauses = -2;
	int f = -1;
	while(f!=no_of_clauses)
	{
		if(f<0)
		{
			string str; getline(cin,str);
			if(str[0]=='p')
			{
				stringstream ss(str);
				string temp;
				ss>>temp; ss>>temp;
				ss>>no_of_var; ss>>no_of_clauses;
				clause.resize(no_of_clauses);
				f++;
			}
		}
		else
		{
			clause[f].clear();
			int temp; cin>>temp;
			while(temp!=0)
			{
				clause[f].pb(temp);
				cin>>temp;
			}
			f++;
		}
	}

	int max_tries = 5;
	while(!found && max_tries--)
		modifiedWalkSAT();

	if(found)
	{
		cout<<"iterations needed : "<<no_of_steps,nl,nl;
		cout<<"solution - ",nl;
		print_assignment();
	}
	else
		cout<<"Couldn't find solution ",nl;
}

bool clause_sat(int i)
{
	for(int j=0; j<clause[i].size(); j++)
	{
		if(clause[i][j]<0 && curr[-clause[i][j]]==0)
			return 1;
		if(clause[i][j]>0 && curr[clause[i][j]]==1)
			return 1;
	}
	return 0;
}

bool satisfies()
{
	for(int i=0; i<no_of_clauses; i++)
		if(!clause_sat(i))
			return 0;
	return 1;
}

void find_var_in_false_clauses()
{
	s.clear();
	freq.clear();

	for(int i=0; i<no_of_clauses; i++)
	{
		if(!clause_sat(i))
		{
			set<int> now;
			for(int j=0; j<clause[i].size(); j++)
			{
				s.insert(abs(clause[i][j]));
				now.insert(abs(clause[i][j]));
			}
			for(auto j:now)
				freq[j]++;
		}
	}
}

void flip_var(int v)
{
	v = min(v, (int)s.size());
	vector<int> temp;
	for(int i=0; i<s.size(); i++)
		temp.pb(i+1);
	random_shuffle(temp.begin(), temp.end());

	for(int i=0; i<s.size(); i++)
	{
		if(temp[i]<=v)
		{
			auto iter = s.begin();
			for(int add=0; add<i; add++) iter++;
			int element = *iter;
			curr[element] = 1 - curr[element];
		}
	}
}

void find_min_conflicting_vvars(int v)
{
	v = min(v, (int)s.size());
	set<pair<int,int>> now;
	for(auto i: freq)
		now.insert({-i.second,i.first});

	auto iter = now.begin();
	for(int i=0; i<v; i++)
	{
		int element = (*iter).second;
		curr[element] = 1 - curr[element];
		iter++;
	}
}

void modifiedWalkSAT()
{
	found = 0;
	no_of_steps = 0;
	curr.clear();
	curr.resize(no_of_var+1);

	for(int i=1; i<=no_of_var; i++)
		curr[i] = rand()%2;
	int v = 0, it;

	while(v<=maxv)
	{
		v++;
		it = 0;
		while(it<maxit)
		{
			it++;
			no_of_steps++;
			if(satisfies())
			{
				found = 1;
				return;
			}
			find_var_in_false_clauses();
			int num = rand()%10000;
			double random = (double)num / (double)10000;

			if(random>=prob) flip_var(v); 
			else find_min_conflicting_vvars(v); 
		}	 
	}
}

int main()
{
	srand(time(NULL));

	// solve_random(no_of_vars,k-CNF,no_of_problems_per_m/n);
	solve_random(10,3,50);

	return 0;
}