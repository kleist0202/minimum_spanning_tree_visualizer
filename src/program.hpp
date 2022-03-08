#ifndef PROGRAM_HPP 
#define PROGRAM_HPP

class Program
{
public:
    virtual void init() = 0;
    virtual void run() = 0;
    virtual void keyboardInput() = 0;

};

#endif // PROGRAM_HPP
