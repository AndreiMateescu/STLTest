//Mateescu Andrei

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <fstream>
#include <set>
#include <limits.h>
#include <istream>
#include <unordered_map>

using namespace std;

struct text{
    string cuvant;
    float x,y,w,h;
};

struct temp{
    int id;
    string ct; float pt;
    string cr; float pr;
    string cb; float pb;
    string cl; float pl;
};

//returnez suma si id-ul dupa ce compar fiecare template cu "text" (structura text)
struct ID_suma{
    int ID;
    float suma;
};

ostream& operator<<(ostream& os, const text& a)
{
    os << a.cuvant << " " << a.x << " " << a.y << " " << a.w << " " << a.h <<endl;
    return os;
}

ostream& operator<<(ostream& os, const temp& a)
{
    os << a.id << " " << a.ct << " " << a.pt << " " << a.cr << " " << a.pr << " " << a.cb << " " <<a.pb << " " << a.cl << " " << a.pl<<endl;
    return os;
}

istream& operator>>(istream& is, text& a)
{
    is >> a.cuvant >> a.x >> a.y >> a.w >> a.h;
    return is;
}

istream& operator>>(istream& f, temp& a)
{
    f>>a.id;
    f>>a.ct;
    if (a.ct != "-")
        f>>a.pt;
    else
        a.pt = 0; //daca cuvantul nu exista, am ales sa ii atribui ponderea cu 0

    f>>a.cr;
    if (a.cr != "-")
        f>>a.pr;
    else
        a.pr = 0;

    f>>a.cb;
    if (a.cb != "-")
        f>>a.pb;
    else
        a.pb = 0;

    f>>a.cl;
    if (a.cl != "-")
        f>>a.pl;
    else
        a.pl = 0;

    return f;
}

bool operator==(const text& a, const text& b)
{
    return a == b;
}

bool operator==(const temp& a, const temp& b)
{
    return a == b;
}

void afisare_vector_text(const vector<text>& a)
{
    vector<text>::const_iterator it;
    for(it=a.begin();it!=a.end();++it)
        cout<<*it;
    cout<<endl;
}

void afisare_vector_temp(const vector<temp>& a)
{
    vector<temp>::const_iterator it;
    for(it=a.begin();it!=a.end();++it)
        cout<<*it;
    cout<<endl;
}

void afisare_map(map<float,string>& a)
{
    for(map<float,string>::iterator it = a.begin(); it != a.end(); ++it)
        cout<<it->first<<" => "<<it->second<<endl;
    cout<<endl;
}

//map ordoneaza. Le gasesc ordonate. Gasesc mai usor pe cel de deasupra si pe cel de jos
map<float,string> ordonare_y(vector<text>& b, const text& a)
{
    map<float,string> salvare;
    for(int i=0;i<b.size();i++){
        if(b[i].x == a.x)
            salvare.insert(pair<float,string>(b[i].y,b[i].cuvant));
    }
    return salvare;
}

//map ordoneaza. Le gasesc ordonate. Gasesc mai usor pe cel din stanga si pe cel din dreapta
map<float,string> ordonare_x(vector<text>& b, const text& a)
{
    map<float,string> salvare;
    for(int i=0;i<b.size();i++){
        if(b[i].y == a.y)
            salvare.insert(pair<float,string>(b[i].x,b[i].cuvant));
    }
    return salvare;
}

//ok = true, compar pe axa x (pentru sus si jos), altfel pe axa y (pentru dreapta si stanga)
//in cazul in care un element nu exista (elementul principal nu are vecin in dreapta/stanga/sus/jos returnez un element cu proprietati de INT_MAX
//nu se va lua in calcul
text cautare(vector<text>& a, int pozitie, text& auxiliar, bool ok)
{
    if (pozitie != INT_MAX){
        if(ok == true){
            for (int i=0;i<a.size();i++)
                if(a[i].x == auxiliar.x && a[i].y == pozitie && a[i].cuvant != auxiliar.cuvant)
                    return a[i];
        }
        else{
            for (int i=0;i<a.size();i++)
                if(a[i].y == auxiliar.y && a[i].x == pozitie && a[i].cuvant != auxiliar.cuvant)
                    return a[i];
        }
    }
    else{
        text gresit; //pozitia nu exista
        gresit.cuvant = "-";
        gresit.x = gresit.y = gresit.w = gresit.h = INT_MAX;
        return gresit;
    }
}

ID_suma rezolvare_temp(const temp& a, vector<text>& b)
{
    ID_suma c;
    c.suma = 0.0;
    c.ID = a.id;

    if(b[0].cuvant == a.ct)//0-> sus
        c.suma = c.suma + a.pt;
    if(b[1].cuvant == a.cr)//1-> dreapta
        c.suma = c.suma + a.pr;
    if(b[2].cuvant == a.cb)//2->jos
        c.suma = c.suma + a.pb;
    if(b[3].cuvant == a.cl)//3->stanga
        c.suma = c.suma + a.pl;

    return c;
}

int main()
{
    int n; //numar cuvinte
    ifstream f("input.txt");

    vector<text> vector_cuvinte;
    vector<temp> vector_template;

    f>>n;
    for(int i=2;i<=n+1;i++){
        text a;
        f>>a;

        vector_cuvinte.push_back(a);
    }

    int t; //numar template-uri
    f>>t;

    for(int i=n+3;i<n+t+3;i++){

        temp a;
        f>>a;

        vector_template.push_back(a);
    }

    afisare_vector_text(vector_cuvinte);
    afisare_vector_temp(vector_template);

    int indice;
    f>>indice;
    text prelucrare = vector_cuvinte[indice];//pentru identificare. acest element va fi cel principal

    map<float, string> mymap = ordonare_y(vector_cuvinte,prelucrare); //pun toate elementele de pe axa x in map, le ordoneaza
    afisare_map(mymap);

    int sus = INT_MAX;
    auto it_sus = mymap.upper_bound(prelucrare.y);

    if(it_sus != mymap.end())
        sus = it_sus->first;

    int jos = INT_MAX;
    auto it_jos = mymap.lower_bound(prelucrare.y);
    it_jos--;

    if (it_jos!=mymap.end())
        jos = it_jos->first;

    map<float, string> mymap2 = ordonare_x(vector_cuvinte,prelucrare); //pun toate elementele de pe axa y in map, le ordoneaza
    afisare_map(mymap2);

    int dreapta = INT_MAX;
    auto it_d = mymap2.upper_bound(prelucrare.x);

    if(it_d != mymap2.end())
        dreapta = it_d->first;

    int stanga = INT_MAX;
    auto it_s = mymap2.lower_bound(prelucrare.x);
    it_s--;

    if(it_s != mymap2.end())
        stanga = it_s->first;

    if (sus != INT_MAX)
        cout<<endl<<"Pozitie sus "<<sus<<endl;
    else
        cout<<"Nu exista"<<endl;

    if (dreapta != INT_MAX)
        cout<<"Pozitie dreapta "<<dreapta<<endl;
    else
        cout<<"Nu exista"<<endl;

    if (jos != INT_MAX)
        cout<<"Pozitie jos "<<jos<<endl;
    else
        cout<<"Nu exista"<<endl;

    if (stanga != INT_MAX)
        cout<<"Pozitie stanga "<<stanga<<endl<<endl;
    else
        cout<<"Nu exista"<<endl<<endl;


    vector<text> vector_final; //vectorul care are vecinii unui element de tip text. Mereu vor fi 4. De aceea in functie, compar de pe pozitiile 0,1,2,3
    text a;
    a = cautare(vector_cuvinte,sus,prelucrare,true);
    vector_final.push_back(a);
    a = cautare(vector_cuvinte,dreapta,prelucrare,false);
    vector_final.push_back(a);
    a = cautare(vector_cuvinte,jos,prelucrare,true);
    vector_final.push_back(a);
    a = cautare(vector_cuvinte,stanga,prelucrare,false);
    vector_final.push_back(a);

    afisare_vector_text(vector_final);

    multimap<float,int>final_map; //multimap care va retine suma si id-ul template-ului
    ID_suma el;
    for(int i=0;i<vector_template.size();i++){
        el = rezolvare_temp(vector_template[i],vector_final);
        if (el.suma != 0)
            final_map.insert(pair<float,int>(el.suma,el.ID));
    }
    //cand gaseste element cu aceeasi suma (ca unul de dinainte) il pune in multimap. se pot repeta (doar in multimap, nu si in map)
    for(multimap<float,int>::iterator it = final_map.begin(); it != final_map.end(); ++it)
        cout<<it->first<<" => "<<it->second<<endl;
    cout<<endl;

    pair<float, int> rezultat;
    rezultat = *final_map.rbegin(); //ultimul element are cea mai mare suma. Ordoneaza dupa suma

    auto raspuns = final_map.find(rezultat.first);
    cout<<"Raspuns: "<<raspuns->second;
    return 0;
}
