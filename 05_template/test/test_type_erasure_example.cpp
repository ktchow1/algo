#include<iostream>
#include<memory>
#include<vector>

class animal
{
public:
    virtual void see() const = 0;    
    virtual void say() const = 0;
    virtual void eat() const = 0;    
};

class dog : public animal
{
public:
    explicit dog(const std::string&){}
    void see() const{ std::cout << "\ndog see"; }
    void say() const{ std::cout << "\ndog say"; }
    void eat() const{ std::cout << "\ndog eat"; }  
};

class cow : public animal
{
public:
    cow(const std::string&, int){}
    void see() const{ std::cout << "\ncow see"; }
    void say() const{ std::cout << "\ncow say"; }
    void eat() const{ std::cout << "\ncow eat"; }  
};

class pig : public animal
{
public:
    pig(const std::string&, int, int){}
    void see() const{ std::cout << "\npig see"; }
    void say() const{ std::cout << "\npig say"; }
    void eat() const{ std::cout << "\npig eat"; }  
};

class any_dog
{
public:
    explicit any_dog(const std::string&){}
    void see() const{ std::cout << "\nany dog see"; }
    void say() const{ std::cout << "\nany dog say"; }
    void eat() const{ std::cout << "\nany dog eat"; }  
};

class any_cow
{
public:
    any_cow(const std::string&, int){}
    void see() const{ std::cout << "\nany cow see"; }
    void say() const{ std::cout << "\nany cow say"; }
    void eat() const{ std::cout << "\nany cow eat"; }  
};

class any_pig
{
public:
    any_pig(const std::string&, int, int){}
    void see() const{ std::cout << "\nany pig see"; }
    void say() const{ std::cout << "\nany pig say"; }
    void eat() const{ std::cout << "\nany pig eat"; }  
};



// *******************************
// *** Method 1 : Polymorphism ***
// *******************************
// The problem with polymorphism :
// * classes that we are working with must be derived from same base class

void test_type_erasure_example1()
{
    std::vector<animal*> animals = 
    {
        new dog("dog"),
        new cow("cow",123),
        new pig("pig",44,55),
    };
    for (const auto* x : animals)
    {
        x->see();
        x->say();
        x->eat();
    }
}  



// ***************************
// *** Method 2 : Template ***
// ***************************
// T in template = base class in polymorphism
//
// Problem with template :
// 1. cannot declare vector<T*>
// 2. class / function depending on invoke(const T&) must be a template as well

template<typename T> void invoke(const T& x)
{
    x.see();
    x.say();
    x.eat();
}

void test_type_erasure_example2()
{
    any_dog d("dog");
    any_cow c("cow",123);
    any_pig p("pig",44,55);

    invoke(d);
    invoke(c);
    invoke(p);
}



// ***************************************
// *** Method 3 : Type erasure pattern ***
// ***************************************
// create a type erasure basic class
// create a type erasure derived class which is a template

class my_animal_base
{
public:
    virtual ~my_animal_base() = default;
    virtual void see() const = 0;    
    virtual void say() const = 0;
    virtual void eat() const = 0;    
};

template<typename T>
class my_animal_wrapper : public my_animal_base
{
public:
    explicit my_animal_wrapper(const T& animal) : _animal(animal) 
    {
    }

    // Unfortunately, not sure how to use this in my_animal
/*  template<typename... ARGS> 
    explicit my_animal_wrapper(ARGS&&... args) : _animal(std::forward<ARGS>(args)...) 
    {
    } */

    void see() const override { _animal.see(); }
    void say() const override { _animal.say(); }
    void eat() const override { _animal.eat(); }
    
private:
    T _animal; // Remark 1 : instance of T
};

class my_animal
{
public:
    my_animal() = default; // default constructor is for std::vector<my_anmimal>

    template<typename U>   
    my_animal(U&& animal) : _animal_ptr(new my_animal_wrapper<U>(std::forward<U>(animal))) 
    {
    }

    void see() const { if (_animal_ptr) _animal_ptr->see(); else throw std::runtime_error("no animal"); }
    void say() const { if (_animal_ptr) _animal_ptr->say(); else throw std::runtime_error("no animal"); }
    void eat() const { if (_animal_ptr) _animal_ptr->eat(); else throw std::runtime_error("no animal"); }

private:
    std::shared_ptr<my_animal_base> _animal_ptr; // Remark 2 : pointer to base 
};

void test_type_erasure_example3()
{
    any_dog d("dog");
    any_cow c("cow",123);
    any_pig p("pig",44,55);
    my_animal my_d(any_dog{"dog"});
    my_animal my_c(any_cow{"cow",123});
    my_animal my_p(any_pig{"pig",44,55});
    std::vector<my_animal> animals; 

    // lvalue my_animal
    animals.push_back(my_d); 
    animals.push_back(my_c); 
    animals.push_back(my_p); 

    // rvalue my_animal
    animals.push_back(std::move(my_d));
    animals.push_back(std::move(my_c));
    animals.push_back(std::move(my_p));

    // lvalue dog/cow/pig
    animals.push_back(d); // implicit conversion from dog to my_animals
    animals.push_back(c); // implicit conversion from cow to my_animals
    animals.push_back(p); // implicit conversion from pig to my_animals

    // rvalue dog/cow/pig
    animals.push_back(std::move(d));
    animals.push_back(std::move(c));
    animals.push_back(std::move(p));

    // We put non-inheritance animals into a vector.
    for(const auto&x : animals)
    {
        x.see();
        x.say();
        x.eat();
    }
}  

void test_type_erasure_example()
{
    test_type_erasure_example1();
    test_type_erasure_example2();
    test_type_erasure_example3();
}
