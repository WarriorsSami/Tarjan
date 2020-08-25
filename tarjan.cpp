#include <fstream>
#include <algorithm>
#include <vector>
#include <stack>
#include <set>
#include <utility>


std::ifstream fin ("componentebiconexe.in");
std::ofstream fout ("componentebiconexe.out");

int task, n, m, bi_comp;
std::vector <std::vector <int>> Graph;
std::vector <bool> Seen;
std::vector <int> Disc, Low, Parent;
std::vector <std::set <int>> Store_BCC;
std::stack <int> Stiva;
std::set <int> Store_AP;
std::set <std::pair <int, int>> Store_Bridges;


inline void DFS1 (int root, int depth)
{
    Disc[root] = Low[root] = depth;
    Seen[root] = true;
    Stiva.push (root);
    int children = 0;

    for (auto v : Graph[root])
    {
        if (Seen[v])
            Low[root] = std::min (Low[root], Disc[v]);
        else
        {
            ++ children;
            Parent[v] = root;
            DFS1 (v, depth + 1);

            Low[root] = std::min (Low[root], Low[v]);

            if ((!Parent[root] && children > 1) || (Parent[root] && Low[v] >= Disc[root]))
            {
                while (Stiva.top () != v)
                {
                    Store_BCC[bi_comp].insert (Stiva.top ());
                    Stiva.pop ();
                }
                Store_BCC[bi_comp].insert (Stiva.top ());
                Stiva.pop ();
                Store_BCC[bi_comp].insert (root);

                ++ bi_comp;
            }
        }
    }
}

inline void BCC ()
{
    for (int v = 1; v <= n; ++ v)
        if (!Seen[v])
            DFS1 (v, 1);

    if (!Stiva.empty ())
    {
        while (!Stiva.empty ())
        {
            Store_BCC[bi_comp].insert (Stiva.top ());
            Stiva.pop ();
        }

        ++ bi_comp;
    }

    fout << bi_comp << "\n";
    for (int i = 0; i < bi_comp; ++ i)
    {
        fout << Store_BCC[i].size () << " ";
        for (auto v : Store_BCC[i])
            fout << v << " ";
        fout << "\n";
    }
}

inline void DFS2 (int root, int depth)
{
    Disc[root] = Low[root] = depth;
    Seen[root] = true;
    int children = 0;

    for (auto v : Graph[root])
    {
        if (Seen[v])
            Low[root] = std::min (Low[root], Disc[v]);
        else
        {
            ++ children;
            Parent[v] = root;
            DFS2 (v, depth + 1);

            Low[root] = std::min (Low[root], Low[v]);

            if ((!Parent[root] && children > 1) || (Parent[root] && Low[v] >= Disc[root]))
                Store_AP.insert (root);
        }
    }
}

inline void Cut_Vertices ()
{
    for (int v = 1; v <= n; ++ v)
        if (!Seen[v])
            DFS2 (v, 1);

    fout << Store_AP.size () << "\n";
    for (auto v : Store_AP)
        fout << v << " ";
}

inline void DFS3 (int root, int depth)
{
    Disc[root] = Low[root] = depth;
    Seen[root] = true;

    for (auto v : Graph[root])
    {
        if (!Seen[v])
        {
            Parent[v] = root;
            DFS3 (v, depth + 1);

            Low[root] = std::min (Low[root], Low[v]);

            if (Low[v] > Disc[root])
                Store_Bridges.insert (std::make_pair (std::min (v, root), std::max(v, root)));
        }
        else if (Parent[root] != v)
            Low[root] = std::min (Low[root], Disc[v]);
    }
}

inline void Bridges ()
{
    for (int v = 1; v <= n; ++ v)
        if (!Seen[v])
            DFS3 (v, 1);

    fout << Store_Bridges.size () << "\n";
    for (auto edge : Store_Bridges)
        fout << edge.first << " " << edge.second << "\n";
}


int main ()
{
    fin >> task >> n >> m;

    Graph = std::vector <std::vector <int>> (n + 1);
    Seen = std::vector <bool> (n + 1, false);
    Disc = Low = Parent = std::vector <int> (n + 1, 0);
    Store_BCC = std::vector <std::set <int>> (n + 1);

    for (int i = 1, u, v; i <= m; ++ i)
    {
        fin >> u >> v;
        Graph[u].push_back (v);
        Graph[v].push_back (u);
    }

    switch (task)
    {
    case 1 : BCC ();
        break;
    case 2 : Cut_Vertices ();
        break;
    case 3 : Bridges ();
        break;
    }

    fin.close (); fout.close ();
    return 0;
}
