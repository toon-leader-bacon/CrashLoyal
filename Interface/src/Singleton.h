// MIT License
// 
// Copyright(c) 2020 Arthur Bacon and Kevin Dill
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <assert.h>
#include <iostream>

// To make a class a singleton, you need to (a) have it inherit from this 
// class, and (b) define its s_Obj in its .cpp file.  Then you can call the
// static get() function from anywhere to get it.
template<class T>
class Singleton
{
protected:
    explicit Singleton() 
    {
        if (!!s_Obj)
        {
            assert(s_Obj != this);
            std::cout << "Duplicate singleton creation!  Deleting the old one..." << std::endl;
            delete s_Obj;
        }
        s_Obj = (T*)this; 
    }

    virtual ~Singleton() {}

public:
    static T& get() 
    { 
        if (!s_Obj) 
        { 
            new T;
        }; 
        assert(!!s_Obj);  
        return *s_Obj; 
    }

    static bool exists() { return !!s_Obj; }

private:
    static T* s_Obj;

private:
    // DELIBERATELY UNDEFINED
    Singleton(const Singleton& rhs);
    Singleton& operator=(const Singleton& rhs);
    bool operator==(const Singleton& rhs) const;
    bool operator<(const Singleton& rhs) const;
};

