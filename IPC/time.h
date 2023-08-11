#include <chrono>
using namespace std::chrono;
class timer
{
    static high_resolution_clock::time_point start_point; 
    int printDelay;
    int bindDelay;
    int libraryDelay;
    public:
    void start() ; 
    int current_time() ;
    void set_print_delay(int value) ; 
    void set_bind_delay(int value) ; 
    void set_library_delay(int value) ;  

    int  get_print_delay() ; 
    int  get_bind_delay() ; 
    int get_library_delay() ;  
};
high_resolution_clock::time_point timer::start_point;
void timer::start()
{
    start_point=high_resolution_clock::now();
}
int timer::current_time()
{
    return duration_cast<seconds>(high_resolution_clock::now()-start_point).count() ; 
}
void timer :: set_print_delay(int value) {
        printDelay = value;
    }

void  timer ::set_bind_delay(int value) {
        bindDelay = value;
    }

void  timer ::set_library_delay(int value) {
        libraryDelay = value;
    }

int timer :: get_print_delay()  {
        return printDelay;
    }

int timer :: get_bind_delay()  {
        return bindDelay;
    }

int timer :: get_library_delay()  {
        return libraryDelay;
    }