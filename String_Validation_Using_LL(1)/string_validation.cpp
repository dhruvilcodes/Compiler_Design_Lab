#include <bits/stdc++.h>
using namespace std;
multimap<char, string> productions; //stores productions
map<char, set<char>> first;         //stores first
map<char, set<char>> follow;        //stores follow
set<char> terminals;                //stores terminals
set<char> variables;                //stores variables
int choice, flag = 0;
char variable, starting_production;
string temp;
map<pair<char, char>, string> table;

set<char> find_first(char variable)
{
    //If variable is terminal, then it will return terminal as it is.
    if (terminals.find(variable) != terminals.end())
    {
        set<char> ans;
        ans.insert(variable);
        return ans;
    }

    //If variable's first is already found then it will escape below condition and first will be returned.
    if (first.find(variable) == first.end())
    {
        //below both iterator is to  iterate over productions of particular variable
        auto itr1 = productions.lower_bound(variable);
        auto itr2 = productions.upper_bound(variable);
        while (itr1 != itr2)
        {
            string utility = itr1->second;
            //if first character of variable is terminal it will be counted as first.
            if (terminals.find(utility[0]) != terminals.end())
            {
                first[variable].insert(utility[0]);
            }
            else
            {
                // A->Ab condition check
                if (utility[0] == variable)
                {
                    cout << "Input is invalid\n";
                    exit(1);
                    //break;
                }
                int i = 0;

                //epsilon condition check

                while (i < utility.length())
                {
                    if (first.find(utility[i]) == first.end())
                    {
                        set<char> temp = find_first(utility[i]);
                    }

                    if (first[utility[i]].find('@') != first[utility[i]].end())
                    {
                        for (auto x : first[utility[i]])
                        {
                            first[variable].insert(x);
                        }
                        i++;
                    }
                    else
                    {
                        break;
                    }
                }
                //if epsilon is not present
                if (i < utility.length())
                {
                    for (auto x : find_first(utility[i]))
                    {
                        first[variable].insert(x);
                    }
                }
            }
            itr1++;
        }
    }
    return first[variable];
}

set<char> find_follow(char variable)
{
    //If variable's follow is already found then it will escape below condition and follow will be returned.
    for (auto x : productions)
    {
        char utility1 = x.first;
        string utility2 = x.second;
        for (int i = 0; i < utility2.length(); i++)
        {
            //finding particular varible in productions
            if (utility2[i] == variable)
            {

                //cout<<utility2<<endl;
                i++;
                //if variable is last character of prodution condition
                if (i == utility2.length() and utility1 != variable)
                {
                    set<char> temp = find_follow(utility1);
                    for (auto y : temp)
                    {
                        follow[variable].insert(y);
                    }
                }
                else
                {
                    //epsilon condition check
                    while (i < utility2.length() and first[utility2[i]].find('@') != first[utility2[i]].end())
                    {
                        //cout<<variable<<" " <<utility2<<" "<<utility2[i]<<endl;

                        set<char> temp = find_first(utility2[i]);
                        for (auto y : temp)
                        {
                            follow[variable].insert(y);
                        }
                        i++;
                    }
                    //Don't have epsilon condition check
                    if (i < utility2.length())
                    {

                        set<char> temp = find_first(utility2[i]);
                        for (auto y : temp)
                        {
                            follow[variable].insert(y);
                        }
                    }
                    //last character condition check
                    if (i == utility2.length() and utility1 != variable)
                    {
                        set<char> temp = find_follow(utility1);
                        for (auto y : temp)
                        {
                            follow[variable].insert(y);
                        }
                    }
                }
            }
        }
    }
    //elimination of '$' from follow of particular varible.
    follow[variable].erase('@');
    return follow[variable];
}
int main()
{

    //taking input from input file
    ifstream MyReadFile("input.txt");
    while (getline(MyReadFile, temp))
    {
        //condition for storing starting productions
        if (flag == 0)
        {
            starting_production = temp[0];
            follow[starting_production].insert('$');
            flag == 1;
        }

        //Storing variables
        variable = temp[0];
        variables.insert(variable);
        string utility = "";
        flag = 0;
        for (int i = 0; i < temp.size(); i++)
        {
            if (temp[i] == '>')
            {
                flag = 1;
                continue;
            }
            if (temp[i] == '|' or temp[i] == '/')
            {
                productions.insert({variable, utility});
                utility = "";
                continue;
            }
            if (flag and temp[i] != ' ')
            {
                utility += temp[i];
            }
            //storing terminals
            if (flag and (temp[i] < 65 or temp[i] > 90) and temp[i] != ' ')
            {
                terminals.insert(temp[i]);
            }
        }
        productions.insert({variable, utility});
    }

    //Printing the results

    cout << "--------------Productions-------------" << endl;
    for (auto x : productions)
    {
        cout << x.first << "------> " << x.second << endl;
    }
    // for (auto x : terminals)
    // {
    //     cout << x << endl;
    // }
    // for (auto x : variables)
    // {
    //     cout << x << endl;
    // }

    cout << endl;
    cout << "--------------First-------------" << endl;

    for (auto x : variables)
    {
        cout << x << "------> { ";
        set<char> temp1 = find_first(x);
        vector<char> temp(temp1.begin(), temp1.end());
        for (int i = 0; i < temp.size() - 1; i++)
        {
            cout << "'" << temp[i] << "', ";
        }
        cout << "'" << temp[temp.size() - 1] << "' }";

        cout << endl;
    }

    cout << endl;
    cout << "--------------Follow-------------" << endl;

    for (auto x : variables)
    {
        cout << x << "------> { ";
        set<char> temp1 = find_follow(x);
        vector<char> temp(temp1.begin(), temp1.end());
        for (int i = 0; i < temp.size() - 1; i++)
        {
            cout << "'" << temp[i] << "', ";
        }
        cout << "'" << temp[temp.size() - 1] << "' }";

        cout << endl;
    }

    for (auto x : productions)
    {
        string temp = x.second;

        for (auto y : find_first(temp[0]))
        {
            if (y == '@')
            {
                for (auto z : follow[x.first])
                {
                    table[{x.first, z}] = x.second;
                }
            }
            else
            {
                table[{x.first, y}] = x.second;
            }
        }
    }
    set<char> terminals_new = terminals;
    terminals_new.insert('$');
    terminals_new.erase('@');
    for (auto x : productions)
    {
        for (auto y : terminals_new)
        {
            if (table.find({x.first, y}) == table.end())
            {
                table[{x.first, y}] = "";
            }
        }
    }
    cout << endl;
    cout << endl;
    cout << "                 <--------------  LL(1) Parsing Table  -------------->";
    cout << endl;
    cout << endl;
    const int nameWidth = 10;
    const int name1Width = 6;
    cout << setw(7) << "|";
    for (auto x : terminals_new)
    {
        cout << setw(8) << x << setw(8) << "|";
    }
    cout << endl;
    cout << endl;
    for (auto x : variables)
    {
        cout << x << setw(name1Width) << "|";
        for (auto y : terminals_new)
        {
            string temp = "";
            if (table[{x, y}] != "")
            {
                temp += x;
                temp += " -> ";
                temp += table[{x, y}];
            }
            cout << setw(nameWidth) << temp << setw(name1Width) << "|";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
    string validate;
    stack<char> val;
    cout << "Please Enter A String to Validate:" << endl;
    cin >> validate;
    cout << endl;
    validate += '$';
    val.push('$');
    val.push(starting_production);
    for (int i = 0; i < validate.length(); i++)
    {
        if (validate[i] == val.top())
        {
            val.pop();
        }
        else
        {
            char temp = val.top();
            string utility;
            if (table.find({temp, validate[i]}) != table.end())
            {
                utility = table[{temp, validate[i]}];
                reverse(utility.begin(), utility.end());
                val.pop();
                for (auto x : utility)
                {
                    val.push(x);
                }
                i--;
            }
            else
            {
                cout << "String is Not Accepted by the Grammar";
                return 0;
            }
        }
    }
    cout << "String is Accepted by the Grammar";
}