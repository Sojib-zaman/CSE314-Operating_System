#include<pthread.h>
#include<bits/stdc++.h>
#include<semaphore.h>
#include "param.h"
#include "student.h"
#include "group.h"
#include "time.h"
#include<fstream>
#include<unistd.h>
#include <random>



using namespace std ;

random_device rd ; 
mt19937 gen(rd());


int student_count; 
int grp_std ; 

timer our_clock ; 






ifstream inputfile ; 
ofstream outputfile ; 
ofstream demofile ; 

int state[100] ; // creating for 100 students at first 
int priting_station[5] ; // will be using 1-4

pthread_mutex_t extra_mutex ;  
pthread_mutex_t ct_mutex ; 
sem_t library_mutex ; 
sem_t db ; 
int read_count = 0 ; 

sem_t print_mutex[PRINT_STATION] ; 
sem_t st_sem[100] ; // for every student creating a semaphore 
sem_t binding_duty ;
sem_t checkprintdone[15] ; // for each group , checking if all done 

int submission_count = 0 ; 
int get_print_station(int id)
{
    return (id%4)+1 ; 
}
void Locked_Printer(string str1, string color) {
    pthread_mutex_lock(&ct_mutex);
    cout << color <<  str1 << RESET << endl;
    outputfile<< str1 <<endl ; 
    pthread_mutex_unlock(&ct_mutex);
}
int rand_delay()
{
    double lamda = 10 ; 
    poisson_distribution<int> Poisson_rand(lamda) ; 
    int random_delay = Poisson_rand(gen) ;
    return random_delay ; 
}
void test(int std_id)
{
   
    if(state[std_id] == WAITING_FOR_PRINT && priting_station[get_print_station(std_id)] != BUSY )
    {
        state[std_id] = PRINTING ; 
        priting_station[get_print_station(std_id)] = PRINTING ;
        sem_post(&st_sem[std_id]) ; 
    }

    
} 
void use_printing_station(int std_id)
{
    sem_wait(&print_mutex[get_print_station(std_id)-1]) ; 
    Locked_Printer("Student : "+ to_string(std_id) +" has arrived at the printing station at time "+to_string(our_clock.current_time())+" printing station : "+to_string(get_print_station(std_id)),BLUE) ; 
    state[std_id] = WAITING_FOR_PRINT ; 
    test(std_id) ; 
    sleep(our_clock.get_print_delay()) ; 
    sem_post(&print_mutex[get_print_station(std_id)-1]) ; 
    sem_wait(&st_sem[std_id]) ; 
  
}
void call_test(int lower_bound , int upper_bound , int std_id,bool verbose = false ) 
{
    
     for (int i = lower_bound+1; i <= upper_bound; i++)
    {
        if(state[i]!=DONE_PRINTING && get_print_station(i)==get_print_station(std_id))
        {
            state[i] = WAITING_FOR_PRINT ; 
            if(verbose)
            {
                demofile<<i<<endl ; 
            }
            
            test(i) ; 
            break;
        }
       
    }
}
void leave_printing_station(int std_id)
{
    sem_wait(&print_mutex[get_print_station(std_id)-1]) ; 
    state[std_id] = DONE_PRINTING ; 
    priting_station[get_print_station(std_id)] = EMPTY ;
    Locked_Printer("Student : "+ to_string(std_id) +" has done printing at time "+to_string(our_clock.current_time())+" printing station : "+to_string(get_print_station(std_id)),RED) ;  
   

    int group_no_minone= (std_id - 1) / grp_std ; 
    int upper_bound = (group_no_minone+1)*grp_std ; 
    int lower_bound = group_no_minone*grp_std ; 
   
    
    call_test(lower_bound , upper_bound , std_id,true) ; 
    
    call_test(0,lower_bound,std_id) ;
    call_test(upper_bound,student_count,std_id) ;  
    
    sem_post(&checkprintdone[group_no_minone+1]) ; 
    sem_post(&print_mutex[get_print_station(std_id)-1]) ; 

}
void reader(int n)
{
    while(true)
    {   
        sleep(rand_delay()) ; 
        Locked_Printer("Staff   : "+ to_string(n) +" started reading from the entry book at time "+to_string(our_clock.current_time()) + ". No. of submissions = "+to_string(submission_count),RESET) ;  
        sleep(our_clock.get_library_delay()) ; 
        Locked_Printer("Staff   : "+ to_string(n) +" finished reading from the entry book at time "+to_string(our_clock.current_time()) + ". No. of submissions = "+to_string(submission_count),MAGENTA) ;  
        sem_wait(&library_mutex) ; 
        read_count+=1; 
        if(read_count==1)sem_wait(&db) ; 
        sem_post(&library_mutex) ; 
       
        sem_wait(&library_mutex) ; 
        read_count-- ; 
        if(read_count==0) sem_post(&db) ; 
        sem_post(&library_mutex) ; 

        if(submission_count==(student_count/grp_std)) break ; 



    }
    Locked_Printer("Staff   : "+ to_string(n) +" is done reading the entry book at time "+to_string(our_clock.current_time())+ ". No. of submissions = "+to_string(submission_count),RESET) ;  

}
void writer(int student_id , int group_id)
{
    
    sem_wait(&db) ; 
    Locked_Printer("Group   : "+ to_string(group_id) +" is now writing in the entry book at time "+to_string(our_clock.current_time()),CYAN) ;  
   
    sem_post(&db) ; 
    submission_count++ ; 
   
}



void* staff_routine(void* arg)
{
    sleep(rand_delay()) ; 
    int* val = (int*)arg ; 
     
    //cout<<"Staff "<<*val<<" is now in staff routine"<<endl ;   
    reader(*val) ;
}
void* routine(void* arg)
{
  
    int* val = (int*)arg ; 
   

    //cout<<GREEN<<"Student id "<<*val<<" is now going to check if he can print"<<RESET<<endl ; 

    sleep(rand_delay()) ; 
    use_printing_station(*val) ; 
    leave_printing_station(*val) ;

    int group_no = ((*val - 1) / grp_std) +1; 
    //leader 
    if(*val%grp_std!=0)  return (void*)arg;  
    
    for(int i = 0 ; i<grp_std  ; i++)
    {
        sem_wait(&checkprintdone[group_no]) ; 
    }
    Locked_Printer("Group   : "+ to_string(group_no) +" has finished it's printing at time "+to_string(our_clock.current_time()),YELLOW) ;  

    sem_wait(&binding_duty) ; 
    Locked_Printer("Group   : "+ to_string(group_no) +" has started binding at time "+to_string(our_clock.current_time()),GREEN) ;     
    sleep(our_clock.get_bind_delay());
    sem_post(&binding_duty) ; 


    Locked_Printer("Group   : "+ to_string(group_no) +" has finished binding at time "+to_string(our_clock.current_time()),CYAN) ;  
   
    writer(*val , group_no) ; 
    Locked_Printer("Group   : "+ to_string(group_no) +" has  submitted at time "+to_string(our_clock.current_time()),MAGENTA) ;  
  

}




void init_student_id(student students[])
{
     for(int i=0 ; i<student_count ; i++)
        {
            students[i].setid(i+1) ; 
            students[i].set_ps(((i+1)%4)+1) ; 
        }
}
void init_group_id(group groups[],int group_count)
{
     for(int i=0 ; i<group_count ; i++)
        groups[i].setid(i+1) ; 
}
void init_student_group(student students[],int group_count,group groups[])
{
    vector<student*>ls; 
    int cnt = 0 , gc=0 ; 
    int mem_gc = student_count/group_count ; 
    for(int i=0 ; i<student_count ; i++)
    {
        students[i].setgroup( (i/group_count)+1) ; 
        ls.push_back(&students[i]) ;
        cnt++ ;
        if( (i+1)%mem_gc==0 && i!=0)
        {
            
            cnt = 0 ; 
            groups[gc].add_members(ls) ; 
            ls.clear() ; 
            groups[gc].set_leader(&students[i]) ; 
            gc++;  
        } 
      
    } 
}

int main()
{
   
    inputfile.open("input.txt" , ios::in) ; 
   
    outputfile.open("output.txt" , ios::out); 
    demofile.open("demo.txt" , ios::out); 
    our_clock.start() ; 
    inputfile>>student_count ; 
    inputfile>>grp_std ; 
    int printing_delay ; 
    int binding_delay ; 
    int r_w_delay ; 
    inputfile>>printing_delay>>binding_delay>>r_w_delay  ; 
    our_clock.set_bind_delay(binding_delay) ; 
    our_clock.set_library_delay(r_w_delay) ; 
    our_clock.set_print_delay(printing_delay) ; 

    int group_count = student_count/grp_std ; 

    // initialize students and groups 
    student students[student_count] ;
    group groups[group_count] ; 
    init_student_id(students) ; 
    init_group_id(groups , group_count) ; 
    init_student_group(students,group_count,groups); 


    pthread_t student_thread[student_count] ;  
    pthread_t staff_thread[staff_count] ; 

    for(int i = 0 ; i<PRINT_STATION ; i++)
        sem_init(&print_mutex[i] , 0  , 1 ) ; 
  
    sem_init(&binding_duty,0,2) ;


    sem_init(&library_mutex ,0, 1) ; 
    sem_init(&db,0,1)  ; 
    pthread_mutex_init(&extra_mutex,NULL)  ; 
    pthread_mutex_init(&ct_mutex,NULL)  ; 


    for(int i = 0 ; i<PRINT_STATION; i++)
        priting_station[i] = EMPTY ; 
    
    for(int i = 0 ; i<student_count ; i++)
        sem_init(&st_sem[i] , 0 , 0 )  ; 
        
    for(int i = 1 ; i<=group_count ; i++)
        sem_init(&checkprintdone[i] , 0 , 0 )  ; 
      
    int indices[student_count]  ; 
    for(int i=1; i<=student_count ; i++)
    {
        indices[i-1] = i ; 
        pthread_create(student_thread+i-1, NULL , &routine , &indices[i-1]) ; 
    }
    
    int staff_indices[staff_count] ; 
    for(int k=1 ; k<=staff_count;k++)
    {
        staff_indices[k-1]=k;
        pthread_create(staff_thread+k-1 , NULL , &staff_routine , &staff_indices[k-1]) ; 
       
    }

    
    for(int j=1 ; j<=student_count ; j++)
    {
        pthread_join(student_thread[j-1] , NULL) ; 
    }
    for(int k=1 ; k<=staff_count ; k++)
    {
        pthread_join(staff_thread[k-1] , NULL) ; 
    }
        
    
   
    pthread_mutex_destroy(&extra_mutex) ; 


    
    
}