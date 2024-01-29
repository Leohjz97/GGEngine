#include <GGEngine/Inc/GGEngine.h>

using namespace GGEngine;
using namespace GGEngine::Core;

class Student
{
public:
    Student() = default;
    Student(const std::string& name)
        :mName(name)
    {
    }

    Student(const std::string& name, int classmark)
        :mName(name), mClassMark(classmark)
    {
    }

    void SetName(const std::string& name)
    {
        mName = name;
    }

    void SetGrade(int classMark)
    {
        mClassMark = classMark;
    }

private:
    std::string mName;
    int mClassMark = 0;
};


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    TypedAllocter studentPool = TypedAllocter<Student>("StudentPool", 100);

    Student* a = studentPool.New("Frank");
    Student* b = studentPool.New("Leon",100);
    Student* c = studentPool.New("Leon", 100);
    Student* d = studentPool.New("Leon", 100);
    //a, b, c, d
    
    studentPool.Delete(c);
    //a, b, _, d

    Student* e = studentPool.New("Leon", 100);
    //a, b, e, d

    Student* f = studentPool.New("Leon", 100);
    //a, b, e, d, f

    return 0;
}