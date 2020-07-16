#ifndef SWEEP_HPP
#define SWEEP_HPP

#ifdef  __cplusplus
extern "C" {
#endif


//#push_macro("T")

#define SWEEP_T float
class Sweep
{
private:
    SWEEP_T value;
    SWEEP_T step;
    SWEEP_T start;
    unsigned int step_idx;
    unsigned int step_count;
    unsigned int prescale_idx;
    unsigned int prescale_count;
public:
    Sweep()
    {
        reset();
        value =  0;
        step =  0;
        start =  0;
        step_count = 0;
        prescale_count = 0;
    }
    ~Sweep(){};
    void reset()
    {
        prescale_idx = 0;
        step_idx = 0;
        value = start;
    }
    bool sweep_step()
    {
        ++prescale_idx;
        if(prescale_idx>=prescale_count)
        {
            prescale_idx = 0;
            ++step_idx;
            value += step;
            if (step_idx>=step_count)
                reset();
            return (step_count>1);
        }
        return false;
    }
    SWEEP_T get_step(){return step;}
    SWEEP_T get_value(){return value;}
    SWEEP_T get_start(){return start;}
    //T get_next_value(){return step_count value+step;}
    unsigned int get_step_idx(){return step_idx;}
    unsigned int get_prescale(){return prescale_idx;}
    unsigned int get_prescale_counter(){return prescale_count;}
    unsigned int get_step_count(){return step_count;}
    void set_step(SWEEP_T step_)        {step = step_; reset();}
    void set_start(SWEEP_T start_)      {start = start_; reset();}
    void set_step_idx(unsigned int step_idx_)        {if (step_idx_<step_count) step_idx = step_idx_; value = start+step_idx*step; prescale_idx=0;}
    void set_step_count(unsigned int step_count_)        {step_count = step_count_; reset();}
    void set_prescale(unsigned int prescale_count_)        {prescale_count=prescale_count_; reset();}

};
//#pop_macro("T")


#ifdef  __cplusplus
}
#endif


#endif
