#ifndef H_TASK
#define H_TASK
class Task {
public:
    Task(){}
    virtual ~Task();

public:
    virtual void run() = 0;
};
#endif
