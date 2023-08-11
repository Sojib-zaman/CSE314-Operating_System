using namespace std; 
class group
{

    int id ; 
    student *leader ;
    vector<student*>members ;
    

    public : 
    group() ; 
    group(int id, student* leader) ; 
    void setid(int id) ; 
    int getid() ; 
    void show_info() ; 
    void set_leader(student* leader) ; 
    void add_members(vector<student*>) ; 
};

group::group(){
    id=-1; 
    leader = nullptr;
}
group::group(int id , student* leader )
{
    this->id = id ; 
    this->leader = leader ; 
}
void group::setid(int id)
{
    this->id = id ; 
}
int group::getid()
{
    return id ; 
}
void group::set_leader(student* leader)
{
   
    this->leader = leader ; 
}
void group::show_info()
{
   
    cout<<"ID : "<<id<<" LEADER : "<<leader->getid()<<endl ; 
    
    cout<<"Members: \n" ; 
    for(student* s : members)
        cout<<"ID : "<<s->getid()<<endl ; 
}
void group::add_members(vector<student*>ls)
{
    members = ls ; 
}