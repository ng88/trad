
class A

    public A(integer a)
    {
        prints("A::A(integer)\n");
    }
    
    public A()
    {
        prints("A::A()\n");
    }

end

class B
    inherit A
        
        
    public B()
    {
        prints("B::B()\n");
    }
    
    public B(integer b)
    {
        super(b);
        prints("B::B()\n");
    }
    
    public void main()
    {
        VAR A a;
        prints("test1\n");
        a := new B();
        prints("test2\n");
        a := new B(5);
    }

end
