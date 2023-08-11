using namespace std; 
class student
{

    int id ; 
    int group ; 
    int printing_station ; 


public :
    student() ; 
    student(int id, int group=0) ; 
    void setid(int id) ; 
    void setgroup(int group) ; 
    int getid() ; 
    int getgroup(); 
    void set_ps(int ps) ; 
    int get_ps() ; 
    void show_info() ; 
};
student::student()
{
    this->id = -1 ; 
}
student::student(int id , int group )
{
    this->id = id ;
    this->group = group ;
}
void student::setid(int id)
{
    this->id = id ; 
}
void student::setgroup(int id)
{
    this->group = id ; 
}
int student::getid()
{
    return id ; 
}
int student::getgroup()
{
    return group ; 
}
void student::set_ps(int ps)
{
    this->printing_station = ps  ;    
}
int student::get_ps()
{
    return printing_station ; 
}
void student::show_info()
{
    cout<<"ID : "<<id<<" GROUP : "<<group<<" Printing station :  "<<printing_station<<endl ; 
}
